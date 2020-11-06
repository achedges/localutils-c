//
// Created by Adam Hedges on 11/4/20.
//

#include <stdlib.h>
#include "jsonparser.h"


// prototypes

JsonToken* tokenize(JsonParser* parser);
JsonElement* parse_array(JsonToken* stream);
JsonElement* parse_object(JsonToken* stream);

JsonParser* jsonparser_init()
{
	JsonParser* ret = malloc(sizeof(JsonParser));
	ret->n = 0;
	ret->i = 0;
	ret->tokenStream = NULL;
	ret->result = NULL;
	return ret;
}

JsonElement* jsonparser_parse(JsonParser* parser, string input)
{
	parser->input = input;
	parser->n = strlen(parser->input);

	parser->tokenStream = tokenize(parser);

	if (parser->tokenStream == NULL)
		return parser->result;

	if (parser->tokenStream->type == JSON_ARRAY)
		parser->result = parse_array(parser->tokenStream);
	else if (parser->tokenStream->type == JSON_OBJECT)
		parser->result = parse_object(parser->tokenStream);

	return parser->result;
}



// implementations

JsonElement* new_bool(bool value) {
	JsonElement* element = malloc(sizeof(JsonElement));
	element->type = JSON_BOOL;
	element->boolValue = value;
	return element;
}

JsonElement* new_null() {
	JsonElement* element = malloc(sizeof(JsonElement));
	element->type = JSON_NULL;
	return element;
}

JsonElement* new_string(string value) {
	JsonElement* element = malloc(sizeof(JsonElement));
	element->type = JSON_STRING;
	element->stringValue = value;
	return element;
}

JsonElement* new_integer(string text) {
	JsonElement* element = malloc(sizeof(JsonElement));
	element->type = JSON_INT;
	element->intValue = strtol(text, NULL, 10);
	element->doubleValue = strtod(text, NULL);
	return element;
}

JsonElement* new_float(string text) {
	JsonElement* element = malloc(sizeof(JsonElement));
	element->type = JSON_FLOAT;
	element->doubleValue = strtod(text, NULL);
	return element;
}

JsonElement* new_array() {
	JsonElement* element = malloc(sizeof(JsonElement));
	element->type = JSON_ARRAY;
	element->arrayValue = list_init(sizeof(JsonElement), 0);
	return element;
}

JsonElement* new_object() {
	JsonElement* element = malloc(sizeof(JsonElement));
	element->type = JSON_OBJECT;
	element->objectValue = dict_init_dictionary(STRING);
	return element;
}

JsonElement* parse_literal(string text)
{
	if (strcmp(text, "true") == 0)
		return new_bool(1);
	else if (strcmp(text, "false") == 0)
		return new_bool(0);
	else if (strcmp(text, "null") == 0)
		return new_null();
	else if (strchr(text, '.') != NULL)
		return new_float(text);
	else
		return new_integer(text);
}

JsonElement* parse_array(JsonToken* stream) {
	JsonElement* list = new_array();
	JsonToken* token = stream->next;

	while (token->type != CLOSE_BRACKET) {
		switch (token->type) {
			case IDENTIFIER:
				list_append(list->arrayValue, new_string(token->value));
				break;
			case LITERAL:
				list_append(list->arrayValue, parse_literal(token->value));
				break;
			case OPEN_BRACE:
				list_append(list->arrayValue, parse_object(stream));
				break;
			case OPEN_BRACKET:
				list_append(list->arrayValue, parse_array(stream));
				break;
			default:
				break;
		}

		token = token->next;
		if (token == NULL)
			break;
	}

	return list;
}

JsonElement* parse_object(JsonToken* stream) {
	JsonElement* obj = new_object();
	JsonToken* key = stream->next;

	while (key->type != CLOSE_BRACE) {
		if (key->type == COMMA)
			key = key->next;

		JsonToken* separator = key->next;
		JsonToken* value = key->next;

		switch (value->type) {
			case OPEN_BRACE:
				dict_add_item(&obj->objectValue, key->value, parse_object(stream));
				break;
			case OPEN_BRACKET:
				dict_add_item(&obj->objectValue, key->value, parse_array(stream));
				break;
			case IDENTIFIER:
				dict_add_item(&obj->objectValue, key->value, new_string(value->value));
				break;
			case LITERAL:
				dict_add_item(&obj->objectValue, key->value, parse_literal(value->value));
				break;
			default:
				break;
		}

		key = key->next;
		if (key == NULL)
			break;
	}

	return obj;
}

JsonToken* get_token(JsonTokenType type, string value)
{
	JsonToken* token = malloc(sizeof(JsonToken));
	token->type = type;
	token->value = value;
	token->next = NULL;
	return token;
}

string get_identifier_value(JsonParser* parser)
{
	parser->i++;

	int escaped = 0;
	size_t buffersize = 8;
	size_t valuelen = 0;

	string value = malloc(sizeof(char) * buffersize);
	while (parser->input[parser->i] != '"' || escaped)
	{
		if (parser->i == parser->n)
			break;

		if (parser->input[parser->i] == '\\')
			escaped = !escaped;

		if (valuelen == (buffersize - 1)) // save a spot for \0
		{
			buffersize = (int)(buffersize * 1.5);
			value = realloc(value, buffersize);
		}

		value[valuelen++] = parser->input[parser->i];
		parser->i++;
	}

	value[valuelen] = '\0';

	return value;
}

string get_literal_value(JsonParser* parser)
{
	size_t buffersize = 8;
	size_t valuelen = 0;

	string literal = malloc(sizeof(char) * buffersize);
	while (parser->input[parser->i] != ',' && parser->input[parser->i] != ']' && parser->input[parser->i] != '}')
	{
		if (parser->i == parser->n)
			break;

		if (valuelen == (buffersize - 1)) // save a spot for \0
		{
			buffersize = (int)(buffersize * 1.5);
			literal = realloc(literal, buffersize);
		}

		literal[valuelen++] = parser->input[parser->i];
		parser->i++;
	}

	// back up one so we can capture the ending token in the stream
	parser->i -= 1;
	literal[valuelen-1] = '\0';

	return literal;
}

JsonToken* tokenize(JsonParser* parser)
{
	JsonToken* head = NULL;
	JsonToken* prev = NULL;

	while (parser->i < parser->n)
	{
		JsonToken* token = NULL;

		switch (parser->input[parser->i])
		{
			case ' ':
			case '\t':
			case '\n':
				parser->i++;
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
				token = get_token(CLOSE_BRACE, "]");
				break;

			case ',':
				token = get_token(COMMA, ",");
				break;

			case ':':
				token = get_token(COLON, ":");
				break;

			case '"':
				token = get_token(IDENTIFIER, get_identifier_value(parser));
				break;

			default:
				token = get_token(LITERAL, get_literal_value(parser));
				break;
		}

		parser->i++;

		if (head == NULL)
		{
			head = token;
			prev = head;
		}
		else
		{
			prev->next = token;
			prev = prev->next;
		}
	}

	return head;
}