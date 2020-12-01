//
// Created by Adam Hedges on 11/4/20.
//

#ifndef LOCALUTILS_JSONPARSER_H
#define LOCALUTILS_JSONPARSER_H

#include <stdbool.h>
#include "tokenizer.h"
#include "../types/dict.h"
#include "../types/list.h"
#include "../types/types.h"

typedef enum {
	JSON_OBJECT,
	JSON_ARRAY,
	JSON_BOOL,
	JSON_STRING,
	JSON_INT,
	JSON_FLOAT
} JsonType;

typedef struct {
	JsonType type;
	bool* boolValue;
	string stringValue;
	long* intValue;
	double* doubleValue;
	List* arrayValue;
	Dictionary* objectValue;
} JsonElement;

typedef struct {
	string input;
	size_t n;
	int i;
	JsonToken* currentToken;
	JsonElement* result;
} JsonParser;

JsonParser* jsonparser_init();
JsonElement* jsonparser_parse(JsonParser* parser, string input);
Dictionary* jsonparser_get_dict(JsonElement* element);
List* jsonparser_get_list(JsonElement* element);
string jsonparser_get_string(JsonElement* element);
long* jsonparser_get_long(JsonElement* element);
double* jsonparser_get_double(JsonElement* element);
bool* jsonparser_get_bool(JsonElement* element);

#endif //LOCALUTILS_JSONPARSER_H
