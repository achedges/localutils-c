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
	struct _dict_node* parent;
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
void dict_update_item(Dictionary* dict, void* key, void* value);
void dict_del_item(Dictionary** dict, void* key);
DictNode* dict_get_node(Dictionary* dict, void* key);
void* dict_get_node_value(Dictionary *dict, void *key);
int dict_contains(Dictionary* dict, void* key);
List* dict_get_key_list(Dictionary* dict, TreeWalkOrder walkOrder);
void dict_reset(Dictionary* dict);
DictNode* dict_get_max(DictNode* node);
DictNode* dict_get_min(DictNode* node);
DictNode* dict_get_next(Dictionary* dict, void* key);
DictNode* dict_get_prev(Dictionary* dict, void* key);


#endif //LOCALUTILS_DICT_H
