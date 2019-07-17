//
// Created by Adam Hedges on 2019-07-16.
//

#include "tests.h"
#include "../src/files/files.h"


int test_file_download(int verbose) {
	int failcnt = 0;

	const char* url = "https://www.quandl.com/api/v3/databases/ASINDU/download?api_key=fyGPkWbZ4ssvdsmUTH46&download_type=all-trading-days";
	const char* path = "/Users/adam/c/localutils/resources/test-file.zip";

	download_file(url, path);
	
	// check the file size?

	if (verbose)
		printf("%s Need to finish implementing the file download test\n", ERR_PREFIX);

	return failcnt;
}