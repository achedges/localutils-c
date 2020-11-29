//
// Created by Adam Hedges on 11/27/20.
//

#include "jsonparser.h"

JsonToken* get_token(JsonTokenType type, string value) {
	JsonToken* token = malloc(sizeof(JsonToken));
	token->type = type;
	token->value = value;
	token->next = NULL;
	return token;
}

string get_identifier_value(string* input_ptr, size_t* input_pos, size_t total_len) {
	size_t i = *input_pos + 1;

	size_t buffersize = 8;
	size_t valuelen = 0;

	string value = malloc(sizeof(char) * buffersize);
	while ((*input_ptr)[i] != '"' || (*input_ptr)[i-1] == '\\') {
		if (i == total_len) {
			break;
		}

		if (valuelen == (buffersize - 1)) { // save a spot for \0
			buffersize = (int) (buffersize * 1.5);
			value = realloc(value, buffersize);
		}

		value[valuelen++] = (*input_ptr)[i];
		i++;
	}

	value[valuelen] = '\0';
	(*input_pos) = i; // set outer index

	return value;
}

string get_literal_value(string* input_ptr, size_t* input_pos, size_t total_len) {
	size_t i = *input_pos;
	size_t buffersize = 8;
	size_t valuelen = 0;

	string literal = malloc(sizeof(char) * buffersize);
	while ((*input_ptr)[i] != ',' && (*input_ptr)[i] != ']' && (*input_ptr)[i] != '}') {
		if (i == total_len) {
			break;
		}

		if (valuelen == (buffersize - 1)) { // save a spot for \0
			buffersize = (int) (buffersize * 1.5);
			literal = realloc(literal, buffersize);
		}

		literal[valuelen++] = (*input_ptr)[i];
		i++;
	}

	// back up one so we can capture the ending token in the stream
	i -= 1;
	literal[valuelen] = '\0';
	(*input_pos) = i;

	return literal;
}

JsonToken* tokenize(string* input_ptr) {
	JsonToken* head = NULL;
	JsonToken* prev = NULL;

	size_t i = 0;
	size_t n = strlen(*input_ptr);

	while (i < n) {
		JsonToken* token = NULL;

		switch ((*input_ptr)[i]) {
			case ' ':
			case '\t':
			case '\n':
				i++;
				continue;

			case '{':
				token = get_token(OPEN_BRACE, "{");
				break;

			case '}':
				token = get_token(CLOSE_BRACE, "}");
				break;

			case '[':
				token = get_token(OPEN_BRACKET, "[");
				break;

			case ']':
				token = get_token(CLOSE_BRACKET, "]");
				break;

			case ',':
				token = get_token(COMMA, ",");
				break;

			case ':':
				token = get_token(COLON, ":");
				break;

			case '"':
				token = get_token(IDENTIFIER, get_identifier_value(input_ptr, &i, n));
				break;

			default:
				token = get_token(LITERAL, get_literal_value(input_ptr, &i, n));
				break;
		}

		i++;

		if (head == NULL) {
			head = token;
			prev = head;
		} else {
			prev->next = token;
			prev = prev->next;
		}
	}

	return head;
}