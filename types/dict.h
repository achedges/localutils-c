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

typedef struct Dictionary {
	int count;
	KeyTypes keyType;
	void* key;
	void* value;
	struct Dictionary* parent;
	struct Dictionary* left;
	struct Dictionary* right;
	int (*compare)(const void* a, const void* b);
} Dict;

Dict* dict_init(KeyTypes keytype);
void dict_add_item(Dict* dict, void* key, void* value);
void* dict_get_item(Dict* dict, void* key);
int dict_contains(Dict* dict, void* key);
List* dict_get_key_list(Dict* dict);


#endif //LOCALUTILS_DICT_H
