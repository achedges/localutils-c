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

	for (int a = 1; a < argc; a++) {
		if (strcmp(argv[a], "-verbose") == 0)
			verbose = 1;
		else
			printf("Unknown argument '%s'\n", argv[a]);
	}

	// List test functions
	failcnt += test_list_int(verbose);
	failcnt += test_list_string(verbose);
	failcnt += test_list_empty_append(verbose);

	// DictNode test functions
	failcnt += test_dict_int(verbose);
	failcnt += test_dict_string(verbose);
	failcnt += test_dict_keylist(verbose);
	failcnt += test_dict_balance(verbose);
	failcnt += test_dict_update(verbose);

	// File test functions
	failcnt += test_file_download(verbose);
	failcnt += test_file_extract(verbose);

	if (!failcnt) {
		printf("\nAll tests passed\n");
		return 0;
	} else {
		printf("\n%d test%s FAILED\n", failcnt, failcnt == 1 ? "" : "s");
		return failcnt;
	}
}