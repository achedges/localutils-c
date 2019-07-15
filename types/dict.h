//
// Created by Adam Hedges on 2019-07-11.
//

#ifndef LOCALUTILS_DICT_H
#define LOCALUTILS_DICT_H


#include "list.h"


typedef enum {
	INT,
	STRING
} KeyTypes;

typedef struct _dict_node {
	int count;
	KeyTypes keyType;
	void* key;
	void* value;
	struct _dict_node* parent;
	struct _dict_node* left;
	struct _dict_node* right;
	int (*compare)(const void* a, const void* b);

	// AVL tree properties
	int height;
} DictNode;

DictNode* dict_init_node(KeyTypes keytype);
DictNode* dict_add_item(DictNode* root, void* key, void* value);
void* dict_get_item(DictNode* root, void* key);
int dict_contains(DictNode* root, void* key);
List* dict_get_key_list(DictNode* root);


#endif //LOCALUTILS_DICT_H
