//
// Created by Adam Hedges on 11/4/20.
//

#ifndef LOCALUTILS_JSONPARSER_H
#define LOCALUTILS_JSONPARSER_H

#include <stdlib.h>
#include <string.h>
#include "../types/types.h"

typedef enum
{
	OPEN_BRACE,
	CLOSE_BRACE,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	COLON,
	COMMA,
	IDENTIFIER,
	LITERAL
} JsonTokenType;

typedef enum
{
	JSON_OBJECT,
	JSON_ARRAY,
	JSON_BOOL,
	JSON_NULL,
	JSON_STRING,
	JSON_INT,
	JSON_FLOAT
} JsonType;

typedef struct JsonToken
{
	JsonTokenType type;
	string value;
	struct JsonToken* next;
} JsonToken;

typedef struct
{
	JsonType type;
	void* value;
} JsonElement;

typedef struct
{
	string input;
	size_t n;
	int i;
	JsonToken* tokenStream;
	JsonElement* result;
} JsonParser;

JsonParser* jsonparser_init();
JsonElement* jsonparser_parse(JsonParser* parser, const string input);

#endif //LOCALUTILS_JSONPARSER_H
