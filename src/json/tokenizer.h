//
// Created by Adam Hedges on 11/29/20.
//

#ifndef LOCALUTILS_TOKENIZER_H
#define LOCALUTILS_TOKENIZER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../types/types.h"

typedef enum {
	OPEN_BRACE,
	CLOSE_BRACE,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	COLON,
	COMMA,
	IDENTIFIER,
	LITERAL
} JsonTokenType;

typedef struct JsonToken {
	JsonTokenType type;
	string value;
	struct JsonToken* next;
} JsonToken;

JsonToken* init_token(JsonTokenType type, string value);
JsonToken* tokenize(string* input_ptr);
string parse_identifier_value(string* input_ptr, size_t* input_pos, size_t total_len);
string parse_literal_value(string* input_ptr, size_t* input_pos, size_t total_len);

#endif //LOCALUTILS_TOKENIZER_H
