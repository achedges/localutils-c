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

void set_token(JsonToken** token, JsonTokenType type, string value)
{
	if (*token == NULL)
		*token = malloc(sizeof(JsonToken));

	(*token)->type = type;
	(*token)->value = value;
	(*token)->next = NULL;
}

JsonToken* tokenize(JsonParser* parser)
{
	if (parser->tokenStream == NULL)
		parser->tokenStream = malloc(sizeof(JsonToken));

	JsonToken* token = parser->tokenStream;
	JsonToken** root = &token;

	size_t buffersize;
	size_t valuelen;

	while (parser->i < parser->n)
	{
		if (parser->input[parser->i] == ' ' || parser->input[parser->i] == '\t' || parser->input[parser->i] == '\n')
		{
			parser->i++;
			continue;
		}
		else if (parser->input[parser->i] == '{')
		{
			set_token(&token, OPEN_BRACE, "{");

		}
		else if (parser->input[parser->i] == '}')
		{
			set_token(&token, CLOSE_BRACE, "}");

		}
		else if (parser->input[parser->i] == '[')
		{
			set_token(&token, OPEN_BRACKET, "[");

		}
		else if (parser->input[parser->i] == ']')
		{
			set_token(&token, CLOSE_BRACE, "]");

		}
		else if (parser->input[parser->i] == ',')
		{
			set_token(&token, COMMA, ",");

		}
		else if (parser->input[parser->i] == ':')
		{
			set_token(&token, COLON, ":");

		}
		else if (parser->input[parser->i] == '"')
		{
			parser->i++;

			int escaped = 0;
			buffersize = 8;
			valuelen = 0;

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
			set_token(&token, IDENTIFIER, value);
		}
		else
		{
			buffersize = 8;
			valuelen = 0;

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
			literal[valuelen] = '\0';
			set_token(&token, LITERAL, literal);
		}

		parser->i++;
		token = token->next;
	}

	return *root;
}