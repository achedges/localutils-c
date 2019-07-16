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

typedef enum {
	IN,
	PRE,
	POST
} TreeWalkOrder;

typedef struct _dict_node {
	int avl_height;
	void* key;
	void* value;
	struct _dict_node* left;
	struct _dict_node* right;
} DictNode;

typedef struct {
	int size;
	KeyTypes keytype;
	int (*compare)(const void* a, const void* b);
	DictNode* root;
} Dictionary;

Dictionary* dict_init_dictionary(KeyTypes keytype);
DictNode* dict_init_node();
void dict_add_item(Dictionary** dict, void* key, void* value);
void dict_del_item(Dictionary** dict, void* key);
void* dict_get_item(Dictionary* dict, void* key);
int dict_contains(Dictionary* dict, void* key);
List* dict_get_key_list(Dictionary* dict, TreeWalkOrder walkOrder);


#endif //LOCALUTILS_DICT_H
