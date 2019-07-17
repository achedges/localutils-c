//
// Created by Adam Hedges on 2019-07-11.
//

#include <string.h>
#include "types.h"

int string_comparer(const void* a, const void * b) {
	return strcmp((string)a, (string)b);
}

int int_comparer(const void* a, const void* b) {
	int _a = *(int*)a;
	int _b = *(int*)b;
	if (_a < _b)
		return -1;
	else if (_a > _b)
		return 1;
	else
		return 0;
}