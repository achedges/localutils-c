//
// Created by Adam Hedges on 2019-07-11.
// The dictionary data structure is implemented as an AVL tree
//

#include <stdlib.h>
#include <string.h>
#include "comparers.h"
#include "types.h"
#include "dict.h"
#include "list.h"


// AVL tree implementation - private functions

int _max(int a, int b) {
	return (a > b) ? a : b;
}

int _get_subtree_height(DictNode* node) {
	return (node == NULL) ? 0 : node->height;
}

int _get_subtree_balance(DictNode* node) {
	return (node == NULL) ? 0 : _get_subtree_height(node->left) - _get_subtree_height(node->right);
}

DictNode* _right_rotate(DictNode* node) {
	DictNode* newroot = node->left;
	DictNode* tmpnode = newroot->right;

	newroot->right = node;
	node->left = tmpnode;

	node->height = _max(_get_subtree_height(node->left), _get_subtree_height(node->right)) + 1;
	newroot->height = _max(_get_subtree_height(newroot->left), _get_subtree_height(newroot->right)) + 1;

	return newroot;
}

DictNode* _left_rotate(DictNode* node) {
	DictNode* newroot = node->right;
	DictNode* tmpnode = newroot->left;

	newroot->left = node;
	node->right = tmpnode;

	node->height = _max(_get_subtree_height(node->left), _get_subtree_height(node->right)) + 1;
	newroot->height = _max(_get_subtree_height(newroot->left), _get_subtree_height(newroot->right)) + 1;

	return newroot;
}

// Dictionary implementation - public functions

DictNode* dict_init_node(KeyTypes keytype) {
	DictNode* d = malloc(sizeof(DictNode));
	d->count = 0;
	d->keyType = keytype;
	d->key = NULL;
	d->value = NULL;
	d->parent = NULL;
	d->left = NULL;
	d->right = NULL;
	d->height = 0;

	switch (d->keyType) {
		case INT:
			d->compare = &int_comparer;
			break;
		case STRING:
			d->compare = &string_comparer;
			break;
	}

	return d;
}

DictNode* dict_add_item(DictNode* root, void* key, void* value) {
	if (root->key == NULL) {
		root->key = key;
		root->value = value;
		root->count++;
		return root;
	}

	int comparison = root->compare(key, root->key);
	if (comparison < 0) {
		if (root->left == NULL)	root->left = dict_init_node(root->keyType);
		root->left = dict_add_item(root->left, key, value);
	} else if (comparison > 0) {
		if (root->right == NULL) root->right = dict_init_node(root->keyType);
		root->right = dict_add_item(root->right, key, value);
	} else {
		return root; // no duplicates
	}

	root->height = _max(_get_subtree_height(root->left), _get_subtree_height(root->right)) + 1;
	int balance = _get_subtree_balance(root);

	if (balance > 1 && root->compare(key, root->left->key) < 0)
		return _right_rotate(root);

	if (balance < -1 && root->compare(key, root->right->key) > 0)
		return _left_rotate(root);

	if (balance > 1 && root->compare(key, root->left->key) > 0) {
		root->left = _left_rotate(root->left);
		return _right_rotate(root);
	}

	if (balance < -1 && root->compare(key, root->right->key) < 0) {
		root->right = _right_rotate(root->right);
		return _left_rotate(root);
	}

	return root;
}

void* dict_get_item(DictNode* root, void* key) {
	if (root == NULL)
		return NULL;

	if (root->key == NULL)
		return NULL;

	if (root->compare(root->key, key) == 0)
		return root->value;

	DictNode* n = (root->compare(key, root->key) < 0) ? root->left : root->right;
	while (n != NULL) {
		if (root->compare(key, n->key) == 0)
			return n->value;
		else
			n = (root->compare(key, n->key) < 0) ? n->left : n->right;
	}

	return NULL;
}

int dict_contains(DictNode* root, void* key) {
	return dict_get_item(root, key) != NULL;
}

void _dict_walk_key_list_inorder(DictNode* dict, List* list) {
	if (dict->left != NULL) _dict_walk_key_list_inorder(dict->left, list);
	list_add_item(list, list->count, dict->key);
	if (dict->right != NULL) _dict_walk_key_list_inorder(dict->right, list);
}

List* dict_get_key_list(DictNode* root) {
	size_t bytelen = 0;
	switch (root->keyType) {
		case INT:
			bytelen = sizeof(int);
			break;
		case STRING:
			bytelen = sizeof(string);
			break;
	}

	List* list = list_init(bytelen, root->count);
	_dict_walk_key_list_inorder(root, list);

	return list;
}