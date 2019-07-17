//
// Created by Adam Hedges on 2019-07-11.
//

#include <stdlib.h>
#include <string.h>
#include "comparers.h"
#include "types.h"
#include "list.h"


List* list_init(size_t bytelen, int capacity) {
	List* ret = malloc(sizeof(List));
	ret->capacity = capacity;
	ret->count = 0;
	ret->bytelen = bytelen;
	ret->items = malloc(ret->bytelen * capacity);
	return ret;
}

void list_add_item(List *list, int index, void *item) {
	memcpy(list->items + (list->bytelen * index), item, list->bytelen);
	list->count++;
}

void* list_get_item(List* list, int index) {
	return list->items + (list->bytelen * index);
}

void list_append(List** list, void* item) {
	// auto-grow by 10%

	if ((*list)->count == (*list)->capacity) {
		int newcap = (int)((*list)->capacity * 1.1);
		if (newcap < 10) newcap = 10;

		List* newlist = list_init((*list)->bytelen, newcap);
		newlist->count = (*list)->count;
		newlist->bytelen = (*list)->bytelen;
		newlist->items = malloc(newlist->bytelen * newlist->count);

		memcpy(newlist->items, (*list)->items, (*list)->bytelen * (*list)->count);

		free((*list)->items);
		free(*list);

		(*list) = newlist;
	}

	list_add_item(*list, (*list)->count, item);
}