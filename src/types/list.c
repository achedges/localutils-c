//
// Created by Adam Hedges on 2019-07-11.
//

#include <stdlib.h>
#include <string.h>
#include "comparers.h"
#include "types.h"
#include "list.h"


List* list_init(size_t bytelen, size_t capacity) {
	List* ret = malloc(sizeof(List));

	ret->capacity = (capacity == 0) ? 100 : capacity; // initialize an empty list with a capacity of 100
	ret->count = 0;
	ret->bytelen = bytelen;
	ret->items = malloc(ret->bytelen * ret->capacity);

	return ret;
}

void list_add_item(List *list, int index, void *item) {
	memcpy(list->items + (list->bytelen * index), item, list->bytelen);
	list->count++;
}

void* list_get_item(List* list, int index) {
	return list->items + (list->bytelen * index);
}

void list_append(List* list, void* item) {
	// auto-grow by 50%

	if (list->count == list->capacity)  {
		list->capacity = (int)(list->capacity * 1.5);
		void* items = malloc(list->bytelen * list->capacity);
		memcpy(items, list->items, list->bytelen * list->count);
		free(list->items);
		list->items = items;
	}

	list_add_item(list, list->count, item);
}