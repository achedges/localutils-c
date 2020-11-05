//
// Created by Adam Hedges on 11/4/20.
//

#include <stdlib.h>
#include "jsonparser.h"


// prototypes

JsonToken* tokenize(JsonParser* parser);

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

	return NULL;
}



// implementations

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