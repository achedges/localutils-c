//
// Created by Adam Hedges on 2019-07-16.
//

#include "tests.h"


int test_file_download(int verbose) {
	int failcnt = 0;

	const char* url = "https://www.quandl.com/api/v3/databases/ASINDU/download?api_key=fyGPkWbZ4ssvdsmUTH46&download_type=all-trading-days";
	const char* path = "/Users/adam/c/localutils/resources/all-trading-days.zip";

	download_file(url, path);

	struct stat fstat;
	if (stat(path, &fstat) == 0) {
		if (fstat.st_size == 0) {
			if (verbose)
				printf("%s Downloaded file is empty\n", ERR_PREFIX);
			failcnt++;
		}
	} else {
		if (verbose)
			printf("%s Downloaded file could not be found\n", ERR_PREFIX);
		failcnt++;
	}

	remove(path);

	if (verbose)
		printf("test_file_download() %s\n", !failcnt ? "passed" : "FAILED");

	return failcnt;
}

int test_file_extract(int verbose) {
	int failcnt = 0;

	const char* archivepath = "/Users/adam/c/localutils/resources/test-file.zip";
	const char* extractpath = "/Users/adam/c/localutils/resources/test-file.txt";

	extract_file(archivepath, extractpath, 0);

	struct stat fstat;
	if (stat(extractpath, &fstat) == 0) {
		if (fstat.st_size == 0) {
			if (verbose)
				printf("%s Extracted file is empty\n", ERR_PREFIX);
			failcnt++;
		}
	} else {
		if (verbose)
			printf("%s Extracted file could not be found\n", ERR_PREFIX);
		failcnt++;
	}

	remove(extractpath);

	if (verbose)
		printf("test_file_extract() %s\n", !failcnt ? "passed" : "FAILED");

	return failcnt;
}