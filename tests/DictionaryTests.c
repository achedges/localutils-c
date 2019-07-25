//
// Created by Adam Hedges on 2019-07-16.
//

#include "tests.h"


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

	List* keylist = dict_get_key_list(dict, IN);
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

	Dictionary *dict = dict_init_dictionary(INT);
	for (int i = 0; i < sizeof(intset) / sizeof(int); i++)
		dict_add_item(&dict, &intset[i], &intset[i]);

	int preorder[10] = {3, 1, 0, 2, 7, 5, 4, 6, 8, 9};
	List* list = dict_get_key_list(dict, PRE);
	for (int i = 0; i < sizeof(intset) / sizeof(int); i++) {
		int val = *(int *) list_get_item(list, i);
		if (val != preorder[i]) {
			if (verbose)
				printf("%s Incorrect pre-order key %d (expected %d)\n", ERR_PREFIX, val, preorder[i]);
			failcnt++;
		}
	}

	dict_del_item(&dict,  &preorder[0]); // delete current root '3'
	int preorder2[9] = { 2, 1, 0, 7, 5, 4, 6, 8, 9 };
	list = dict_get_key_list(dict, PRE);
	for (int i = 0; i < sizeof(preorder2) / sizeof(int); i++) {
		int val = *(int*)list_get_item(list, i);
		if (val != preorder2[i]) {
			if (verbose)
				printf("%s Incorrect pre-order key %d after delete (expected %d)\n", ERR_PREFIX, val, preorder2[i]);
			failcnt++;
		}
	}

	dict_del_item(&dict, &preorder2[7]);
	int preorder3[8] = { 2, 1, 0, 7, 5, 4, 6, 9 };
	list = dict_get_key_list(dict, PRE);
	for (int i = 0; i < sizeof(preorder3) / sizeof(int); i++) {
		int val = *(int*)list_get_item(list, i);
		if (val != preorder3[i]) {
			if (verbose)
				printf("%s Incorrect pre-order key %d after delete (expected %d)\n", ERR_PREFIX, val, preorder3[i]);
			failcnt++;
		}
	}

	if (verbose)
		printf("test_dict_balance() %s\n", !failcnt ? "passed" : "FAILED");

	return failcnt;
}

int test_dict_update(int verbose) {
	int failcnt = 0;
	int keys[] = { 1, 2, 3, 4 };
	int update = 9;

	Dictionary* dict = dict_init_dictionary(INT);
	for (int i = 0; i < sizeof(keys) / sizeof(int); i++)
		dict_add_item(&dict, &keys[i], &keys[i]);

	for (int i = 0; i < sizeof(keys) / sizeof(int); i++)
		dict_update_item(dict, &keys[i], &update);

	for (int i = 0; i < sizeof(keys) / sizeof(int); i++) {
		int _val = *(int*)dict_get_item(dict, &keys[i]);
		if (_val != update) {
			if (verbose)
				printf("%s Incorrect updated value %d (expected %d)\n", ERR_PREFIX, _val, update);
			failcnt++;
		}
	}

	if (verbose)
		printf("test_dict_update() %s\n", !failcnt ? "passed" : "FAILED");

	return failcnt;
}