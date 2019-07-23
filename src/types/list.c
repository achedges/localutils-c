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

List* list_append(List* list, void* item) {
	// auto-grow by 50%
	List* workinglist;

	if (list->count < list->capacity) {
		workinglist = list;
	} else {
		int newcap = (int)(list->capacity * 1.5);
		workinglist = list_init(list->bytelen, newcap);
		workinglist->count = list->count;
		workinglist->bytelen = list->bytelen;
		workinglist->items = malloc(workinglist->bytelen * workinglist->capacity);
		memcpy(workinglist->items, list->items, list->bytelen * list->count);
		free(list->items);
		free(list);
	}

	list_add_item(workinglist, workinglist->count, item);
	return workinglist;
}