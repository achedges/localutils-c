//
// Created by Adam Hedges on 2019-07-11.
//

#include <stdlib.h>
#include <string.h>
#include "comparers.h"
#include "types.h"
#include "dict.h"
#include "list.h"


Dict* dict_init(KeyTypes keytype) {
	Dict* d = malloc(sizeof(Dict));
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

void dict_add_item(Dict* dict, void* key, void* value) {
	if (dict->key == NULL) {
		dict->key = key;
		dict->value = value;
		dict->count++;
		return;
	}

	Dict** parent = NULL;
	Dict** d = &dict;
	while (*d != NULL) {
		parent = d;
		d = (dict->compare(key, (*d)->key) < 0) ? &(*d)->left : &(*d)->right;
	}

	*d = dict_init(dict->keyType);
	(*d)->key = key;
	(*d)->value = value;
	if (parent != NULL)
		(*d)->parent = *parent;

	dict->count++;
}

void* dict_get_item(Dict* dict, void* key) {
	if (dict == NULL)
		return NULL;

	if (dict->key == NULL)
		return NULL;

	if (dict->compare(dict->key, key) == 0)
		return dict->value;

	Dict* n = (dict->compare(key, dict->key) < 0) ? dict->left : dict->right;
	while (n != NULL) {
		if (dict->compare(key, n->key) == 0)
			return n->value;
		else
			n = (dict->compare(key, n->key) < 0) ? n->left : n->right;
	}

	return NULL;
}

int dict_contains(Dict* dict, void* key) {
	return dict_get_item(dict, key) != NULL;
}

void _dict_walk_key_list_inorder(Dict* dict, List* list) {
	if (dict->left != NULL) _dict_walk_key_list_inorder(dict->left, list);
	list_add_item(list, list->count, dict->key);
	if (dict->right != NULL) _dict_walk_key_list_inorder(dict->right, list);
}

List* dict_get_key_list(Dict* dict) {
	size_t bytelen = 0;
	switch (dict->keyType) {
		case INT:
			bytelen = sizeof(int);
			break;
		case STRING:
			bytelen = sizeof(string);
			break;
	}

	List* list = list_init(bytelen, dict->count);
	_dict_walk_key_list_inorder(dict, list);

	return list;
}