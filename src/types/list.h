//
// Created by Adam Hedges on 2019-07-11.
//

#ifndef LOCALUTILS_LIST_H
#define LOCALUTILS_LIST_H


typedef struct {
	int capacity;
	int count;
	int bytelen;
	void* items;
} List;

List* list_init(size_t bytelen, size_t capacity);
void list_add_item(List *list, int index, void *item);
void* list_get_item(List* list, int index);
List* list_append(List* list, void* item);


#endif //LOCALUTILS_LIST_H
