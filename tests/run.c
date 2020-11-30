//
// Created by Adam Hedges on 2019-07-11.
//

#include <stdio.h>
#include <string.h>
#include "../tests/tests.h"


const char* ERR_PREFIX = "   *** ERROR ***";

int main(int argc, char** argv) {

	int failcnt = 0;
	int verbose = 0;

	bool run_list_tests = false;
	bool run_dict_tests = false;
	bool run_file_tests = false;
	bool run_json_tests = false;

	for (int a = 1; a < argc; a++) {
		if (strcmp(argv[a], "-verbose") == 0) {
			verbose = 1;
		} else if (strcmp(argv[a], "-list") == 0) {
			run_list_tests = true;
		} else if (strcmp(argv[a], "-dict") == 0) {
			run_dict_tests = true;
		} else if (strcmp(argv[a], "-file") == 0) {
			run_file_tests = true;
		} else if (strcmp(argv[a], "-json") == 0) {
			run_json_tests = true;
		} else {
			printf("Unknown argument '%s'\n", argv[a]);
		}
	}

	// List test functions
	if (run_list_tests)	{
		failcnt += test_list_int(verbose);
		failcnt += test_list_string(verbose);
		failcnt += test_list_empty_append(verbose);
	}

	// DictNode test functions
	if (run_dict_tests) {
		failcnt += test_dict_int(verbose);
		failcnt += test_dict_string(verbose);
		failcnt += test_dict_keylist(verbose);
		failcnt += test_dict_balance(verbose);
		failcnt += test_dict_update(verbose);
		failcnt += test_dict_next_prev(verbose);
		failcnt += test_dict_parent(verbose);
	}

	// File test functions
	if (run_file_tests) {
		failcnt += test_file_download(verbose);
		failcnt += test_file_extract(verbose);
	}

	// JsonParser test functions
	if (run_json_tests) {
		failcnt += test_json_object_parse(verbose);
		failcnt += test_json_array_parse(verbose);
	}

	if (!failcnt) {
		printf("\nAll tests passed\n");
		return 0;
	} else {
		printf("\n%d test%s FAILED\n", failcnt, failcnt == 1 ? "" : "s");
		return failcnt;
	}
}