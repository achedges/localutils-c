//
// Created by Adam Hedges on 11/4/20.
//

#include "tests.h"


int test_deserialize(int verbose) {
	int failcnt = 0;

	JsonParser* parser = jsonparser_init();
	JsonElement* result = jsonparser_parse(parser, "{ \"property\": true }");

	if (result == NULL)
		failcnt += 1;

	Dictionary* rootobj = jsonparser_get_dict(result);

	if (dict_contains(rootobj, "property")) {
		JsonElement* value = dict_get_node_value(rootobj, "property");
		if (value != NULL) {
			if (value->type == JSON_BOOL) {
				bool* boolvalue = jsonparser_get_bool(value);
				if (boolvalue != NULL) {
					if (!(*boolvalue)) {
						printf("bool value is incorrect\n");
						failcnt++;
					}
				} else {
					if (verbose) printf("bool value is null\n");
					failcnt++;
				}
			} else {
				if (verbose) printf("value is not a JSON_BOOL\n");
				failcnt++;
			}
		} else {
			if (verbose) printf("value for 'property' element is NULL\n");
			failcnt++;
		}
	} else {
		if (verbose) printf("'property' does not exist in root object\n");
		failcnt++;
	}

	if (verbose)
		printf("test_deserialize() %s\n", !failcnt ? "passed" : "FAILED");

	return failcnt;
}