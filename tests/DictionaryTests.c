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
			string val = (string) dict_get_node_value(dict, &keys[i]);
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
			int val = *(int*) dict_get_node_value(dict, keys[i]);
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
		string val = dict_get_node_value(dict, list_get_item(keylist, i));
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
		int _val = *(int*) dict_get_node_value(dict, &keys[i]);
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

int test_dict_next_prev(int verbose) {
	int failcnt = 0;

	int keys[] = { 4, 1, 7, 2, 9 };
	int next[] = { 7, 2, 9, 4, -1 };
	int prev[] = { 2, -1, 4, 1, 7 };

	Dictionary* dict = dict_init_dictionary(INT);
	for (int i = 0; i < sizeof(keys) / sizeof(int); i++)
		dict_add_item(&dict, &keys[i], &keys[i]);

	for (int i = 0; i < sizeof(keys) / sizeof(int); i++) {
		DictNode* nextNode = dict_get_next(dict, &keys[i]);
		DictNode* prevNode = dict_get_prev(dict, &keys[i]);

		int value = *(int*) dict_get_node_value(dict, &keys[i]);
		int nextValue = (nextNode != NULL) ? *(int*)(nextNode->value) : -1;
		int prevValue = (prevNode != NULL) ? *(int*)(prevNode->value) : -1;

		if (value != keys[i]) {
			if (verbose)
				printf("%s Incorrect node value %d (expected %d)\n", ERR_PREFIX, value, keys[i]);
			failcnt++;
		}

		if (nextValue != next[i]) {
			if (verbose)
				printf("%s Incorrect next node value %d for node %d (expected %d)\n", ERR_PREFIX, nextValue, value, next[i]);
			failcnt++;
		}

		if (prevValue != prev[i]) {
			if (verbose)
				printf("%s Incorrect previous node value %d for node %d (expected %d)\n", ERR_PREFIX, prevValue, value, prev[i]);
			failcnt++;
		}
	}

	if (verbose)
		printf("test_dict_next_prev() %s\n", !failcnt ? "passed" : "FAILED");

	return failcnt;
}

int _test_dict_parent_dfs(DictNode* node, int verbose) {
	if (node == NULL)
		return 0;

	int failcnt = 0;
	failcnt += _test_dict_parent_dfs(node->left, verbose);
	failcnt += _test_dict_parent_dfs(node->right, verbose);

	if (node->left != NULL && node->left->parent != node)
	{
		if (verbose)
			printf("%s Incorrect parent node %d for left child %d (expected %d)\n", ERR_PREFIX, *(int*)(node->left->parent->key), *(int*)(node->left->key), *(int*)node->key);
		failcnt++;
	}

	if (node->right != NULL && node->right->parent != node) {
		if (verbose)
			printf("%s Incorrect parent node %d for right child %d (expected %d)\n", ERR_PREFIX, *(int*)(node->right->parent->key), *(int*)(node->right->key), *(int*)node->key);
		failcnt++;
	}

	return failcnt;
}

int test_dict_parent(int verbose) {
	int failcnt = 0;

	int keys[] = { 1, 2, 3, 4, 5 };
	Dictionary* dict = dict_init_dictionary(INT);
	for (int i = 0; i < sizeof(keys) / sizeof(int); i++)
		dict_add_item(&dict, &keys[i], &keys[i]);

	if (dict->root->parent != NULL) {
		if (verbose)
			printf("%s Root parent is not NULL\n", ERR_PREFIX);
		failcnt++;
	}

	failcnt += _test_dict_parent_dfs(dict->root, verbose);

	if (verbose)
		printf("test_dict_parent() %s\n", !failcnt ? "passed" : "FAILED");

	return failcnt;
}