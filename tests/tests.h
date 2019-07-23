//
// Created by Adam Hedges on 2019-07-16.
//


#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "../src/types/types.h"
#include "../src/types/list.h"
#include "../src/types/dict.h"
#include "../src/files/files.h"

#ifndef LOCALUTILS_TESTS_H
#define LOCALUTILS_TESTS_H


const extern char* ERR_PREFIX;

// List tests
int test_list_int(int verbose);
int test_list_string(int verbose);
int test_list_empty_append(int verbose);

// Dictionary tests
int test_dict_int(int verbose);
int test_dict_string(int verbose);
int test_dict_keylist(int verbose);
int test_dict_balance(int verbose);

// File tests
int test_file_download(int verbose);
int test_file_extract(int verbose);


#endif //LOCALUTILS_TESTS_H
