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
	return (node == NULL) ? 0 : node->avl_height;
}

int _get_subtree_balance(DictNode* node) {
	return (node == NULL) ? 0 : _get_subtree_height(node->left) - _get_subtree_height(node->right);
}

DictNode* _right_rotate(DictNode* node) {
	DictNode* newroot = node->left;
	DictNode* tmpnode = newroot->right;

	newroot->right = node;
	node->left = tmpnode;

	node->avl_height = _max(_get_subtree_height(node->left), _get_subtree_height(node->right)) + 1;
	newroot->avl_height = _max(_get_subtree_height(newroot->left), _get_subtree_height(newroot->right)) + 1;

	return newroot;
}

DictNode* _left_rotate(DictNode* node) {
	DictNode* newroot = node->right;
	DictNode* tmpnode = newroot->left;

	newroot->left = node;
	node->right = tmpnode;

	node->avl_height = _max(_get_subtree_height(node->left), _get_subtree_height(node->right)) + 1;
	newroot->avl_height = _max(_get_subtree_height(newroot->left), _get_subtree_height(newroot->right)) + 1;

	return newroot;
}

DictNode* _insert(Dictionary* dict, DictNode* root, void* key, void* value) {
	if (root == NULL) {
		root = dict_init_node();
		root->key = key;
		root->value = value;
		return root;
	}

	int comparison = dict->compare(key, root->key);
	if (comparison <  0) {
		root->left = _insert(dict, root->left, key, value);
		root->left->parent = root;
	} else if (comparison > 0) {
		root->right = _insert(dict, root->right, key, value);
		root->right->parent = root;
	} else {
		return root;
	}

	root->avl_height = _max(_get_subtree_height(root->left), _get_subtree_height(root->right)) + 1;
	int balance = _get_subtree_balance(root);

	if (balance > 1 && dict->compare(key, root->left->key) < 0) {
		DictNode* r = _right_rotate(root);
		if (r->left != NULL) r->left->parent = r;
		if (r->right != NULL) r->right->parent = r;
		return r;
	}

	if (balance < -1 && dict->compare(key, root->right->key) > 0) {
		DictNode* r = _left_rotate(root);
		if (r->left != NULL) r->left->parent = r;
		if (r->right != NULL) r->right->parent = r;
		return r;
	}

	if (balance > 1 && dict->compare(key, root->left->key) > 0) {
		root->left = _left_rotate(root->left);
		DictNode* r = _right_rotate(root);
		if (r->left != NULL) r->left->parent = r;
		if (r->right != NULL) r->right->parent = r;
		return r;
	}

	if (balance < -1 && dict->compare(key, root->right->key) < 0) {
		root->right = _right_rotate(root->right);
		DictNode* r = _left_rotate(root);
		if (r->left != NULL) r->left->parent = r;
		if (r->right != NULL) r->right->parent = r;
		return r;
	}

	return root;
}

DictNode* _delete(Dictionary* dict, DictNode* root, void* key) {
	if (root == NULL)
		return root;

	int comparison = dict->compare(key, root->key);
	if (comparison < 0) {
		root->left = _delete(dict, root->left, key);
	} else if (comparison > 0) {
		root->right = _delete(dict, root->right, key);
	} else {
		if (root->left == NULL || root->right == NULL) {
			DictNode* tmp = (root->left == NULL) ? root->right : root->left;
			if (tmp == NULL) {
				tmp = root;
				root = NULL;
			} else {
				*root = *tmp;
			}

			free(tmp);
		} else {
			DictNode* tmp = dict_get_max(root->left);
			root->key = tmp->key;
			root->value = tmp->value;
			root->left = _delete(dict, root->left, tmp->key);
		}
	}

	if (root == NULL)
		return root;

	root->avl_height = _max(_get_subtree_height(root->left), _get_subtree_height(root->right)) + 1;
	int balance = _get_subtree_balance(root);

	if (balance > 1 && _get_subtree_balance(root->left) >= 0)
		return _right_rotate(root);

	if (balance > 1 && _get_subtree_balance(root->left) < 0) {
		root->left = _left_rotate(root->left);
		return _right_rotate(root);
	}

	if (balance < -1 && _get_subtree_balance(root->right) <= 0)
		return _left_rotate(root);

	if (balance < -1 && _get_subtree_balance(root->right) > 0) {
		root->right = _right_rotate(root->right);
		return _left_rotate(root);
	}

	return root;
}

// Dictionary implementation - public functions

Dictionary* dict_init_dictionary(KeyTypes keytype) {
	Dictionary* d = malloc(sizeof(Dictionary));
	d->size = 0;
	d->keytype = keytype;
	d->root = NULL;

	switch (d->keytype) {
		case INT:
			d->compare = &int_comparer;
			break;
		case STRING:
			d->compare = &string_comparer;
			break;
	}

	return d;
}

DictNode* dict_init_node() {
	DictNode* d = malloc(sizeof(DictNode));
	d->key = NULL;
	d->value = NULL;
	d->left = NULL;
	d->right = NULL;
	d->parent = NULL;
	d->avl_height = 1;

	return d;
}

void dict_add_item(Dictionary** dict, void* key, void* value) {
	(*dict)->root = _insert((*dict), (*dict)->root, key, value);
	(*dict)->root->parent = NULL;
	(*dict)->size++;
}

DictNode** _dict_find_node_address(Dictionary *dict, void *key) {
	if (dict->root == NULL)
		return NULL;

	if (dict->root->key == NULL)
		return NULL;

	if (dict->compare(key, dict->root->key) == 0)
		return &(dict->root);

	DictNode** n = (dict->compare(key, dict->root->key) < 0) ? &(dict->root->left) : &(dict->root->right);
	while (*n != NULL) {
		if (dict->compare(key, (*n)->key) == 0)
			return n;
		else
			n = (dict->compare(key, (*n)->key) < 0) ? &((*n)->left) : &((*n)->right);
	}

	return NULL;
}

void dict_update_item(Dictionary* dict, void* key, void* value) {
	DictNode** node_adr = _dict_find_node_address(dict, key);
	if (node_adr == NULL) return;
	(*node_adr)->value = value;
}

DictNode* dict_get_node(Dictionary* dict, void* key) {
	DictNode** node = _dict_find_node_address(dict, key);
	if (node == NULL)
		return NULL;
	return *node;
}

void* dict_get_node_value(Dictionary *dict, void *key) {
	DictNode* node = dict_get_node(dict, key);
	if (node == NULL)
		return NULL;
	return node->value;
}

void dict_del_item(Dictionary** dict, void* key) {
	(*dict)->root = _delete((*dict), (*dict)->root, key);
	(*dict)->size--;
}

int dict_contains(Dictionary* dict, void* key) {
	return dict_get_node(dict, key) != NULL;
}

void _dict_walk_key_list_inorder(DictNode* node, List* list) {
	if (node->left != NULL) _dict_walk_key_list_inorder(node->left, list);
	list_add_item(list, list->count, node->key);
	if (node->right != NULL) _dict_walk_key_list_inorder(node->right, list);
}

void _dict_walk_key_list_preorder(DictNode* node, List* list) {
	list_add_item(list, list->count, node->key);
	if (node->left != NULL) _dict_walk_key_list_preorder(node->left, list);
	if (node->right != NULL) _dict_walk_key_list_preorder(node->right, list);
}

void _dict_walk_key_list_postorder(DictNode* node, List* list) {
	if (node->left != NULL) _dict_walk_key_list_postorder(node->left, list);
	if (node->right != NULL) _dict_walk_key_list_postorder(node->right, list);
	list_add_item(list, list->count, node->key);
}

List* dict_get_key_list(Dictionary* dict, TreeWalkOrder walkOrder) {
	size_t bytelen = 0;
	switch (dict->keytype) {
		case INT:
			bytelen = sizeof(int);
			break;
		case STRING:
			bytelen = sizeof(string);
			break;
	}

	List* list = list_init(bytelen, dict->size);

	switch (walkOrder) {
		case IN:
			_dict_walk_key_list_inorder(dict->root, list);
			break;
		case PRE:
			_dict_walk_key_list_preorder(dict->root, list);
			break;
		case POST:
			_dict_walk_key_list_postorder(dict->root, list);
			break;
		default:
			break;
	}

	return list;
}

void _dict_reset_node(DictNode* node) {
	if (node->left != NULL) {
		_dict_reset_node(node->left);
		node->left = NULL;
	}

	if (node->right != NULL) {
		_dict_reset_node(node->right);
		node->right = NULL;
	}

	free(node);
}

void dict_reset(Dictionary* dict) {
	if (dict->root != NULL)
		_dict_reset_node(dict->root);
	dict->size = 0;
	dict->root = NULL;
}

DictNode* dict_get_min(DictNode *node) {
	DictNode* current = node;
	while (current->left != NULL)
		current = current->left;
	return current;
}

DictNode* dict_get_max(DictNode *node) {
	DictNode* current = node;
	while (current->right != NULL)
		current = current->right;
	return current;
}

DictNode* dict_get_next(Dictionary* dict, void* key) {
	DictNode* node = dict_get_node(dict, key);
	if (node == NULL)
		return NULL;

	if (node->right != NULL)
		return dict_get_min(node->right);

	DictNode* parent = node->parent;
	while (parent != NULL && parent->right != NULL && dict->compare(node->key, parent->right->key) == 0) {
		node = parent;
		parent = parent->parent;
	}

	return parent;
}

DictNode* dict_get_prev(Dictionary* dict, void* key) {
	DictNode* node = dict_get_node(dict, key);
	if (node == NULL)
		return NULL;

	if (node->left != NULL)
		return dict_get_max(node->left);

	DictNode* parent = node->parent;
	while (parent != NULL && parent->left != NULL && dict->compare(node->key, parent->left->key) == 0) {
		node = parent;
		parent = parent->parent;
	}

	return parent;
}