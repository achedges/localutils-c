//
// Created by Adam Hedges on 11/4/20.
//

#include "tests.h"


string read_test_file(const char* path) {
	FILE* file = fopen(path, "r");
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	string ret = malloc(fileSize + 1);
	fread(ret, 1, fileSize, file);
	fclose(file);

	return ret;
}

int test_json_object_parse(int verbose) {
	string jsonString = read_test_file("../tests/parse-obj.json");

	JsonParser* parser = jsonparser_init();
	JsonElement* result = jsonparser_parse(parser, jsonString);

	if (result == NULL) {
		printf("[result] is NULL\n");
		return 1;
	}

	Dictionary* rootobj = jsonparser_get_dict(result);

	if (rootobj == NULL) {
		printf("[result] is not a Dictionary\n");
		return 1;
	}

	if (rootobj->size != 1) {
		printf("[result] incorrect size\n");
		return 1;
	}

	if (!dict_contains(rootobj, "object")) {
		printf("[result] does not contain 'object' property\n");
		return 1;
	}

	Dictionary* objectDict = jsonparser_get_dict(dict_get_node_value(rootobj, "object"));

	if (objectDict->size != 6) {
		printf("[object] incorrect size\n");
		return 1;
	}

	if (!dict_contains(objectDict, "string")) {
		printf("[object] does not contain 'string' property\n");
		return 1;
	}

	if (strcmp(jsonparser_get_string(dict_get_node_value(objectDict, "string")), "stringvalue") != 0) {
		printf("Incorrect value found at [object][string]\n");
		return 1;
	}

	if (!dict_contains(objectDict, "int")) {
		printf("[object] does not contain 'int' property\n");
		return 1;
	}

	if (*jsonparser_get_long(dict_get_node_value(objectDict, "int")) != 1) {
		printf("Incorrect value found at [object][int]\n");
		return 1;
	}

	if (!dict_contains(objectDict, "float")) {
		printf("[object] does not contain 'float' property\n");
		return 1;
	}

	if (*jsonparser_get_double(dict_get_node_value(objectDict, "float")) != 1.123) {
		printf("Incorrect value found at [object][float]\n");
		return 1;
	}

	if (!dict_contains(objectDict, "bool")) {
		printf("[object] does not contain 'bool' property\n");
		return 1;
	}

	if (!(*jsonparser_get_bool(dict_get_node_value(objectDict, "bool")))) {
		printf("Incorrect value found at [object][bool]\n");
		return 1;
	}

	if (!dict_contains(objectDict, "null")) {
		printf("[object] does not contain 'null' property\n");
		return 1;
	}

	if (dict_get_node_value(objectDict, "null") != NULL) {
		printf("Incorrect value found at [object][null]\n");
		return 1;
	}

	if (!dict_contains(objectDict, "list")) {
		printf("[object] does not contain 'list' property\n");
		return 1;
	}

	List* list = jsonparser_get_list(dict_get_node_value(objectDict, "list"));

	if (list == NULL) {
		printf("[object][list] is NULL\n");
		return 1;
	}

	if (list->count != 3) {
		printf("[object][list] incorrect size\n");
		return 1;
	}

	if (strcmp(jsonparser_get_string(list_get_item(list, 0)), "one") != 0) {
		printf("Incorrect value found at [object][list][0]\n");
		return 1;
	}

	if (strcmp(jsonparser_get_string(list_get_item(list, 1)), "two") != 0) {
		printf("Incorrect value found at [object][list][1]\n");
		return 1;
	}

	if (strcmp(jsonparser_get_string(list_get_item(list, 2)), "\\\"three\\\"") != 0) {
		printf("Inorrect value found at [object][list][2]\n");
		return 1;
	}

	if (verbose) {
		printf("test_json_object_parse() passed\n");
	}

	return 0;
}

int test_json_array_parse(int verbose) {
	string jsonString = read_test_file("../tests/parse-array.json");

	JsonParser* parser = jsonparser_init();
	JsonElement* result = jsonparser_parse(parser, jsonString);

	if (result == NULL) {
		printf("[result] is NULL\n");
		return 1;
	}

	List* list = jsonparser_get_list(result);

	if (list == NULL) {
		printf("[result] is not a List\n");
		return 1;
	}

	if (list->count != 2) {
		printf("[result] incorrect size\n");
		return 1;
	}

	for (int i = 0; i < list->count; i++) {
		Dictionary* obj = jsonparser_get_dict(list_get_item(list, i));

		char key[16];
		char value[16];
		sprintf(key, "some-key-%i", i);
		sprintf(value, "some-value-%i", i);

		if (!dict_contains(obj, "key")) {
			printf("[object] does not contain 'key' property\n");
			return 1;
		}

		if (!dict_contains(obj, "value")) {
			printf("[object] does not contain 'value' property\n");
			return 1;
		}

		if (strcmp(jsonparser_get_string(dict_get_node_value(obj, "key")), key) != 0) {
			printf("Incorrect value found at [list][%i]\n", i);
			return 1;
		}

		if (strcmp(jsonparser_get_string(dict_get_node_value(obj, "value")), value) != 0) {
			printf("Incorrect value found at [list][%i]\n", i);
			return 1;
		}
	}

	if (verbose) {
		printf("test_json_array_parse() passed\n");
	}

	return 0;
}