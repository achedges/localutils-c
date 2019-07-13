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


DictNode* dict_init(KeyTypes keytype) {
	DictNode* d = malloc(sizeof(DictNode));
	d->count = 0;
	d->keyType = keytype;
	d->key = NULL;
	d->value = NULL;
	d->parent = NULL;
	d->left = NULL;
	d->right = NULL;

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

void dict_add_item(DictNode* root, void* key, void* value) {
	if (root->key == NULL) {
		root->key = key;
		root->value = value;
		root->count++;
		return;
	}

	DictNode** parent = NULL;
	DictNode** newnode = &root;
	while (*newnode != NULL) {
		parent = newnode;
		newnode = (root->compare(key, (*newnode)->key) < 0) ? &(*newnode)->left : &(*newnode)->right;
	}

	*newnode = dict_init(root->keyType);
	(*newnode)->key = key;
	(*newnode)->value = value;
	if (parent != NULL)
		(*newnode)->parent = *parent;

	root->count++;
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