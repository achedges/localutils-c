//
// Created by Adam Hedges on 11/4/20.
//

#include "tests.h"


int test_deserialize(int verbose) {
	int failcnt = 0;

	JsonParser* parser = jsonparser_init();
	JsonElement* result = jsonparser_parse(parser, "{ \"property\": \"value\" }");

	if (result == NULL)
		failcnt += 1;

	return failcnt;
}