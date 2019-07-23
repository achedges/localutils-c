//
// Created by Adam Hedges on 2019-07-16.
//

#include "tests.h"


int test_list_int(int verbose) {
	int failcnt = 0;

	int num = 4;
	List* list = list_init(sizeof(int), num);
	for (int i = 0; i < num; i++)
		list_add_item(list, i, &i);

	int appendint = num;
	list = list_append(list, &appendint);

	if (list->count != num + 1) {
		if (verbose)
			printf("%s Incorrect int list size %d (epected %d)\n", ERR_PREFIX, list->count, num + 1);
		failcnt++;
	}
	if (list->capacity != 6) {
		if (verbose)
			printf("%s Incorrect int list capacity %d (expected 6)\n", ERR_PREFIX, list->capacity);
		failcnt++;
	}

	for (int i = 0; i < num + 1; i++) {
		int _val = *(int*)list_get_item(list, i);
		if (_val != i) {
			if (verbose)
				printf("%s Incorrect list item value %d (expected %d)\n", ERR_PREFIX, _val, i);
			failcnt++;
		}
	}

	if (verbose)
		printf("test_list_int() %s\n", !failcnt ? "passed" : "FAILED");

	return failcnt;
}

int test_list_string(int verbose) {
	int failcnt = 0;

	string strings[4] = { "A", "B", "C", "D" };

	List* list = list_init(sizeof(string), 4);
	for (int i = 0; i < 4; i++) {
		list_add_item(list, i, &strings[i]);
	}

	string appendstr = "E";
	list = list_append(list, appendstr);

	if (list->count != 5) {
		if (verbose)
			printf("%s Incorrect string list size %d (expected %d)\n", ERR_PREFIX, list->count, 5);
		failcnt++;
	}

	if (list->capacity != 6) {
		if (verbose)
			printf("%s Incorrect string list capacity %d (expected 6)\n", ERR_PREFIX, list->capacity);
		failcnt++;
	}

	for (int i = 0; i < 4; i++) {
		string _val = *(string*)list_get_item(list, i);
		if (strcmp(_val, strings[i]) != 0) {
			if (verbose)
				printf("%s Incorrect list item val %s (expected %s)\n", ERR_PREFIX, _val, strings[i]);
			failcnt++;
		}
	}

	string _val = (string)list_get_item(list, 4);
	if (strcmp(_val, "E") != 0) {
		if (verbose)
			printf("%s Incorrect list item val %s (expected %s)\n", ERR_PREFIX, _val, "E");
		failcnt++;
	}

	if (verbose)
		printf("test_list_string() %s\n", !failcnt ? "passed" : "FAILED");

	return failcnt;
}

int test_list_empty_append(int verbose) {
	int failcnt = 0;

	int appenditems = 120;
	List* list = list_init(sizeof(int), 0);
	for (int i = 0; i < appenditems; i++) {
		list = list_append(list, &i);
	}

	for (int i = 0; i < list->count; i++) {
		int item = *(int*)list_get_item(list, i);
		if (item != i) {
			if (verbose)
				printf("%s Incorrect appended list item %d (expected %d)\n", ERR_PREFIX, item, i);
			failcnt++;
		}
	}

	if (verbose)
		printf("test_list_empty_append() %s\n", !failcnt ? "passed" : "FAILED");

	return failcnt;
}