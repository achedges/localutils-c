//
// Created by Adam Hedges on 2019-07-11.
//

#include <stdio.h>
#include <string.h>
#include "../types/types.h"
#include "../types/list.h"
#include "../types/dict.h"


const char* ERR_PREFIX = "   *** ERROR ***";

int test_list_int(int verbose) {
	int failcnt = 0;

	int num = 4;
	List* list = list_init(sizeof(int), num);
	for (int i = 0; i < num; i++)
		list_add_item(list, i, &i);

	int appendint = num;
	list_append(&list, &appendint);

	if (list->count != num + 1) {
		if (verbose)
			printf("%s Incorrect int list size %d (epected %d)\n", ERR_PREFIX, list->count, num + 1);
		failcnt++;
	}
	if (list->capacity != 10) {
		if (verbose)
			printf("%s Incorrect int list capacity %d (expected 10)\n", ERR_PREFIX, list->capacity);
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
	list_append(&list, appendstr);

	if (list->count != 5) {
		if (verbose)
			printf("%s Incorrect string list size %d (expected %d)\n", ERR_PREFIX, list->count, 5);
		failcnt++;
	}

	if (list->capacity != 10) {
		if (verbose)
			printf("%s Incorrect string list capacity %d (expected %d)\n", ERR_PREFIX, list->capacity, 10);
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

int test_dict_int(int verbose) {
	int failcnt = 0;

	int keys[] = { 1, 2, 3, 4 };
	string values[] = { "A", "B", "C", "D" };

	Dictionary* dict = dict_init_dictionary(INT);
	for (int i = 0; i < (sizeof(keys) / sizeof(int)); i++)
		dict_add_item(&dict, &keys[i], values[i]);

	for (int i = 0; i < (sizeof(keys) / sizeof(int)); i++) {
		if (dict_contains(dict, &keys[i]) == 0) {
			if (verbose)
				printf("%s Key %d not found\n", ERR_PREFIX, keys[i]);
			failcnt++;

		} else {
			string val = (string)dict_get_item(dict, &keys[i]);
			if (strcmp(val, values[i]) != 0) {
				if (verbose)
					printf("%s Incorrect value %s for key %d (expected %s)\n", ERR_PREFIX, val, keys[i], values[i]);
				failcnt++;
			}
		}
	}

	if (verbose)
		printf("test_dict_int() %s\n", !failcnt ? "passed" : "FAILED");

	return failcnt;
}

int test_dict_string(int verbose) {
	int failcnt = 0;

	string keys[] = { "A", "B", "C", "D" };
	int values[] = { 1, 2, 3, 4 };

	Dictionary* dict = dict_init_dictionary(STRING);
	for (int i = 0; i < (sizeof(keys) / sizeof(string)); i++)
		dict_add_item(&dict, keys[i], &values[i]);

	for (int i = 0; i < (sizeof(keys) / sizeof(string)); i++) {
		if (dict_contains(dict, keys[i]) == 0) {
			if (verbose)
				printf("%s Key %s not found\n", ERR_PREFIX, keys[i]);
			failcnt++;
		} else {
			int val = *(int*)dict_get_item(dict, keys[i]);
			if (val != values[i]) {
				if (verbose)
					printf("%s Incorrect value %d for key %s (expected %d)\n", ERR_PREFIX, val, keys[i], values[i]);
				failcnt++;
			}
		}
	}

	if (verbose)
		printf("test_dict_string() %s\n", !failcnt ? "passed" : "FAILED");

	return failcnt;
}

int test_dict_keylist(int verbose) {
	int failcnt = 0;

	Dictionary* dict = dict_init_dictionary(INT);

	int keys[] = { 3, 1, 2, 0, 5, 4, 6 };
	string values[] = { "A", "B", "C", "D", "E", "F", "G" };

	for (int i = 0; i < (sizeof(keys) / sizeof(int)); i++)
		dict_add_item(&dict, &keys[i], values[keys[i]]);

	List* keylist = dict_get_key_list(dict);
	for (int i = 0; i < (sizeof(keys) / sizeof(int)); i++) {
		int val = *(int*)list_get_item(keylist, i);
		if (val != i) {
			if (verbose)
				printf("%s Incorrect list item key %d (expected %d)\n", ERR_PREFIX, val, i);
			failcnt++;
		}
	}

	for (int i = 0; i < keylist->count; i++) {
		string val = dict_get_item(dict, list_get_item(keylist, i));
		if (strcmp(val, values[i]) != 0) {
			if (verbose)
				printf("%s Incorrect dict item value lookup from keylist %s (expected %s)\n", ERR_PREFIX, val, values[i]);
			failcnt++;
		}
	}

	if (verbose)
		printf("test_dict_keylist() %s\n", !failcnt ? "passed" : "FAILED");

	return failcnt;
}

int test_dict_balance(int verbose) {
	int failcnt = 0;

	int intset[10];
	for (int i = 0; i < sizeof(intset) / sizeof(int); i++)
		intset[i] = i;

	Dictionary* dict = dict_init_dictionary(INT);
	for (int i = 0; i < sizeof(intset) / sizeof(int); i++)
		dict_add_item(&dict, &intset[i], &intset[i]);

	if (*(int*)dict->root->key != 3) {
		if (verbose)
			printf("%s Incorrect balanced tree root %d (expected %d)\n", ERR_PREFIX, *(int*)dict->root->key, 3);
		failcnt++;
	}

	List* list = dict_get_key_list(dict);
	for (int i = 0; i < sizeof(intset) / sizeof(int); i++) {
		int val = *(int*)list_get_item(list, i);
		if (val != i) {
			if (verbose)
				printf("%s Incorrect list value %d from balanced dictionary (expected %d)\n", ERR_PREFIX, val, i);
			failcnt++;
		}
	}

	if (verbose)
		printf("test_dict_balance() %s\n", !failcnt ? "passed" : "FAILED");

	return failcnt;
}

int main(int argc, char** argv) {

	int failcnt = 0;
	int verbose = 0;

	for (int a = 1; a < argc; a++) {
		if (strcmp(argv[a], "-verbose") == 0)
			verbose = 1;
		else
			printf("Unknown argument '%s'\n", argv[a]);
	}

	// List test functions
	failcnt += test_list_int(verbose);
	failcnt += test_list_string(verbose);

	// DictNode test functions
	failcnt += test_dict_int(verbose);
	failcnt += test_dict_string(verbose);
	failcnt += test_dict_keylist(verbose);
	failcnt += test_dict_balance(verbose);

	if (!failcnt) {
		printf("All tests passed\n");
		return 0;
	} else {
		printf("%d test%s FAILED\n", failcnt, failcnt == 1 ? "" : "s");
		return failcnt;
	}
}