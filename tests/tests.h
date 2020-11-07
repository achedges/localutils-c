//
// Created by Adam Hedges on 2019-07-16.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "../src/types/types.h"
#include "../src/types/list.h"
#include "../src/types/dict.h"
#include "../src/files/files.h"
#include "../src/json/jsonparser.h"

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
int test_dict_update(int verbose);
int test_dict_next_prev(int verbose);
int test_dict_parent(int verbose);

// File tests
int test_file_download(int verbose);
int test_file_extract(int verbose);

// JsonParser tests
int test_json_object_parse(int verbose);
int test_json_array_parse(int verbose);


#endif //LOCALUTILS_TESTS_H
