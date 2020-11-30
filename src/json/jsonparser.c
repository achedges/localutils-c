//
// Created by Adam Hedges on 11/4/20.
//

#include <stdlib.h>
#include "jsonparser.h"


// prototypes

JsonToken* next_token(JsonParser* parser);
JsonElement* parse_array(JsonParser* parser);
JsonElement* parse_object(JsonParser* parser);

JsonParser* jsonparser_init() {
	JsonParser* ret = malloc(sizeof(JsonParser));
	ret->n = 0;
	ret->i = 0;
	ret->currentToken = NULL;
	ret->result = NULL;
	return ret;
}

JsonElement* jsonparser_parse(JsonParser* parser, string input) {
	parser->input = input;
	parser->n = strlen(parser->input);

	parser->currentToken = tokenize(&parser->input);

	if (parser->currentToken == NULL) {
		return parser->result;
	}

	if (parser->currentToken->type == OPEN_BRACKET) {
		parser->result = parse_array(parser);
	} else if (parser->currentToken->type == OPEN_BRACE) {
		parser->result = parse_object(parser);
	}

	return parser->result;
}

Dictionary* jsonparser_get_dict(JsonElement* element) {
	return element->type == JSON_OBJECT ? element->objectValue : NULL;
}

List* jsonparser_get_list(JsonElement* element) {
	return element->type == JSON_ARRAY ? element->arrayValue : NULL;
}

string jsonparser_get_string(JsonElement* element) {
	return element->type == JSON_STRING ? element->stringValue : NULL;
}

long* jsonparser_get_long(JsonElement* element) {
	return element->type == JSON_INT ? element->intValue : NULL;
}

double* jsonparser_get_double(JsonElement* element) {
	return element->type == JSON_FLOAT ? element->doubleValue : NULL;
}

bool* jsonparser_get_bool(JsonElement* element) {
	return element->type == JSON_BOOL ? element->boolValue : NULL;
}



// implementations

JsonElement* new_json_element(JsonType type) {
	JsonElement* element = malloc(sizeof(JsonElement));
	element->type = type;
	element->objectValue = NULL;
	element->arrayValue = NULL;
	element->boolValue = NULL;
	element->doubleValue = NULL;
	element->intValue = NULL;
	element->stringValue = NULL;
	return element;
}

JsonElement* new_bool(bool value) {
	JsonElement* element = new_json_element(JSON_BOOL);
	element->boolValue = malloc(sizeof(bool));
	*(element->boolValue) = value;
	return element;
}

JsonElement* new_string(string value) {
	JsonElement* element = new_json_element(JSON_STRING);
	element->stringValue = value;
	return element;
}

JsonElement* new_integer(string text) {
	JsonElement* element = new_json_element(JSON_INT);
	long* longval = malloc(sizeof(long));
	(*longval) = strtol(text, NULL, 10);
	double* doubleval = malloc(sizeof(double));
	(*doubleval) = (double)(*longval);
	element->intValue = longval;
	element->doubleValue = doubleval;
	return element;
}

JsonElement* new_float(string text) {
	JsonElement* element = new_json_element(JSON_FLOAT);
	element->doubleValue = malloc(sizeof(double));
	(*(element->doubleValue)) = strtod(text, NULL);
	return element;
}

JsonElement* new_array() {
	JsonElement* element = new_json_element(JSON_ARRAY);
	element->arrayValue = list_init(sizeof(JsonElement), 0);
	return element;
}

JsonElement* new_object() {
	JsonElement* element = new_json_element(JSON_OBJECT);
	element->objectValue = dict_init_dictionary(STRING);
	return element;
}

JsonElement* parse_literal(string text) {
	if (strcmp(text, "true") == 0) {
		return new_bool(1);
	} else if (strcmp(text, "false") == 0) {
		return new_bool(0);
	} else if (strcmp(text, "null") == 0) {
		return NULL;
	} else if (strchr(text, '.') != NULL) {
		return new_float(text);
	} else {
		return new_integer(text);
	}
}

JsonElement* parse_array(JsonParser* parser) {
	JsonElement* list = new_array();
	JsonToken* token = next_token(parser);

	while (token->type != CLOSE_BRACKET) {
		switch (token->type) {
			case IDENTIFIER:
				list_append(list->arrayValue, new_string(token->value));
				break;
			case LITERAL:
				list_append(list->arrayValue, parse_literal(token->value));
				break;
			case OPEN_BRACE:
				list_append(list->arrayValue, parse_object(parser));
				break;
			case OPEN_BRACKET:
				list_append(list->arrayValue, parse_array(parser));
				break;
			default:
				break;
		}

		token = next_token(parser);
		if (token == NULL) {
			break;
		}
	}

	return list;
}

JsonElement* parse_object(JsonParser* parser) {

	JsonElement* obj = new_object();
	JsonToken* key = next_token(parser);

	while (key->type != CLOSE_BRACE) {
		if (key->type == COMMA) {
			key = next_token(parser);
		}

		JsonToken* separator = next_token(parser);
		JsonToken* value = next_token(parser);

		if (separator->type != COLON) {
			printf("Invalid key/value separator detected: '%s'", separator->value);
			return obj;
		}

		switch (value->type) {
			case OPEN_BRACE:
				dict_add_item(&obj->objectValue, key->value, parse_object(parser));
				break;
			case OPEN_BRACKET:
				dict_add_item(&obj->objectValue, key->value, parse_array(parser));
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

		key = next_token(parser);
		if (key == NULL) {
			break;
		}
	}

	return obj;
}

JsonToken* next_token(JsonParser* parser) {
	parser->currentToken = parser->currentToken->next;
	return parser->currentToken;
}
