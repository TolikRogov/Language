#include "Lexer.hpp"

BinaryTreeStatusCode SkipExtra(const char* buffer, size_t* token_start, size_t size) {

	while (isspace(buffer[*token_start])) {
		(*token_start)++;
		if (*token_start == size)
			return TREE_NO_ERROR;
	}

	if (buffer[*token_start] == TOC) {
		while (buffer[++(*token_start)] != TOC) {
			if (*token_start == size)
				return TREE_EXPRESSION_COMMENTS_ERROR;
		}
		(*token_start)++;
	}

	while (isspace(buffer[*token_start])) {
		(*token_start)++;
		if (*token_start == size)
			return TREE_NO_ERROR;
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode LexerCtor(Lexer* lexer) {

	if (!lexer)
		return TREE_NULL_POINTER;

	lexer->capacity = LEXER_DEFAULT_CAPACITY;

	lexer->tokens = (Token*)calloc(lexer->capacity, sizeof(Token));
	if (!lexer->tokens)
		return TREE_ALLOC_ERROR;

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode LexerDtor(Lexer* lexer) {

	if (!lexer)
		return TREE_NO_ERROR;

	if (lexer->buffer) {
		free(lexer->buffer);
		lexer->buffer = NULL;
	}

	lexer->capacity = 0;
	lexer->size = 0;

	if (lexer->tokens) {
		free(lexer->tokens);
		lexer->tokens = NULL;
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode LexerRealloc(Lexer* lexer) {

	if (lexer->capacity == lexer->size) {
		size_t old_capacity = lexer->capacity;
		lexer->tokens = (Token*)realloc(lexer->tokens, (lexer->capacity *= 2) * sizeof(Token));
		if (!lexer->tokens)
			return TREE_ALLOC_ERROR;

		for (size_t i = old_capacity; i < lexer->capacity; i++) {
			lexer->tokens[i].data.val_num = 0;
			lexer->tokens[i].index = 0;
			lexer->tokens[i].type = UNW;
		}
	}

	return TREE_NO_ERROR;
}

static const char* GetTokenStringType(NodeType type) {
	switch (type) {
		case NUMBER: 				return RET_STRING(NUMBER);
		case IDENTIFIER: 			return RET_STRING(IDENTIFIER);
		case KEYWORD:  				return RET_STRING(KEYWORD);
		case FUNCTION_DEFINITION: 	return RET_STRING(FUNCTION_DEFINITION);
		case PARAMETERS:			return RET_STRING(PARAMETERS);
		case VAR_DECLARATION:		return RET_STRING(VAR_DECLARATION);
		case UNW:
		default:  return RET_STRING(UNW);
	}
}

BinaryTreeStatusCode PrintTokenValue(Token* token, IdNameTable* id_name_table) {

	switch (token->type) {
		case NUMBER: 				{ printf(GREEN("%lg"), token->data.val_num); break; }
		case IDENTIFIER: {
			for (size_t i = 0; i < id_name_table->data[token->data.val_id].length; i++)
				printf(GREEN("%c"), id_name_table->data[token->data.val_id].string[i]);
			break;
		}
		case KEYWORD:  				{ printf(GREEN("%s"), KeyWordsGetString(token->data.val_key_word)); break; }
		case FUNCTION_DEFINITION: {
			for (size_t j = 0; j < id_name_table->data[token->data.val_func_def].length; j++)
				printf(GREEN("%c"), id_name_table->data[token->data.val_func_def].string[j]);
			break;
		}
		case VAR_DECLARATION: {
			for (size_t j = 0; j < id_name_table->data[token->data.val_decl_var].length; j++)
				printf(GREEN("%c"), id_name_table->data[token->data.val_decl_var].string[j]);
			break;
		}
		case PARAMETERS:		 	{ printf(GREEN("%s"), RET_STRING(PARAMETERS)); break; }
		case UNW:
		default:  					{ printf(GREEN("%s"), RET_STRING(UNW)); break; }
	}
	printf("\n");
	return TREE_NO_ERROR;
}

//FIXME
BinaryTreeStatusCode PrintTokenValueGrammar(Token* token) {
	switch (token->type) {
		case NUMBER: 				{ printf(GREEN("%lg"), token->data.val_num); break; }
		case IDENTIFIER: 			{ printf(GREEN("%zu"), token->data.val_id); break; }
		case KEYWORD:  				{ printf(GREEN("%s"), KeyWordsGetString(token->data.val_key_word)); break; }
		case FUNCTION_DEFINITION: 	{ printf(GREEN("%zu"), token->data.val_func_def); break; }
		case VAR_DECLARATION: 		{ printf(GREEN("%zu"), token->data.val_decl_var); break; }
		case PARAMETERS: 			{ printf(GREEN("%s"),  RET_STRING(PARAMETERS)); break; }
		case UNW:
		default:  { printf(GREEN("%s"), RET_STRING(UNW)); break; }
	}
	printf("\n");
	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PrintLexer(Lexer* lexer, IdNameTable* id_name_table) {

	if (!lexer)
		return TREE_NULL_POINTER;

	printf("\n");
	printf(BLUE("Lexer address:") " " GREEN("%p") "\n", lexer);
	printf(BLUE("Lexer capacity:") " " GREEN("%zu") "\n", lexer->capacity);
	printf(BLUE("Lexer size:") " " GREEN("%zu") "\n", lexer->size);

	for (size_t i = 0; i < lexer->capacity; i++) {
		printf(BLUE("Lexer token[%.2zu]:") " type - " GREEN("%10s") " index - " GREEN("%5zu"), i, GetTokenStringType(lexer->tokens[i].type), lexer->tokens[i].index);
		printf(" value - ");
		PrintTokenValue(&lexer->tokens[i], id_name_table);
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode LexicalAnalysis(char* buffer, Lexer* lexer, IdNameTable* id_name_table, size_t size) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	size_t token_start = 0;
	char* token_end_pointer = NULL;
	size_t token_end = 1;

	while (true) {
		LEXER_REALLOC(lexer);

		SKIP_EXTRA(buffer, &token_start, size);

		if (token_start == size)
			break;

		token_end = token_start + 1;

		while ((isalpha(buffer[token_end])) && (isalpha(buffer[token_end - 1]))) { token_end++; }

		int op_find = 0;
		for (size_t i = 0; keywords[i].num != AMOUNT_OF_KEYWORDS; i++) {
			op_find = 1;
			size_t j = 0;
			for (; token_start + j != token_end && *(keywords[i].string + j) != '\0'; j++) {
				if (buffer[token_start + j] != *(keywords[i].string + j)) {
					op_find = 0;
					break;
				}
			}
			if (op_find && token_start + j == token_end && *(keywords[i].string + j) == '\0') {
				lexer->tokens[lexer->size].type = KEYWORD;
				lexer->tokens[lexer->size].data.val_key_word = keywords[i].num;
				lexer->tokens[lexer->size].index = token_start;
				lexer->size++;
				token_start = token_end;
				op_find = 2;
				break;
			}
		}

		if (op_find == 2)
			continue;

		Number_t number = strtod(buffer + token_start, &token_end_pointer);
		if (buffer + token_start != token_end_pointer) {
			lexer->tokens[lexer->size].type = NUMBER;
			lexer->tokens[lexer->size].data.val_num = number;
			lexer->tokens[lexer->size].index = token_start;
			lexer->size++;
			token_start = (size_t)(token_end_pointer - buffer);
			continue;
		}

		while ((isalpha(buffer[token_end]) || isdigit(buffer[token_end]) || buffer[token_end] == '_') &&
			   (isalpha(buffer[token_end - 1]) || isdigit(buffer[token_end - 1]) || buffer[token_end - 1] == '_')) { token_end++; }

		int var_find = 0;
		for (size_t i = 0; i < id_name_table->size; i++) {
			var_find = 1;
			size_t j = 0;
			for (; token_start + j != token_end && j != id_name_table->data[i].length; j++) {
				if (buffer[token_start + j] != *(id_name_table->data[i].string + j)) {
					var_find = 0;
					break;
				}
			}
			if (var_find && token_start + j == token_end && j == id_name_table->data[i].length) {
				lexer->tokens[lexer->size].type = IDENTIFIER;
				lexer->tokens[lexer->size].data.val_id = (size_t)id_name_table->data[i].num;
				lexer->tokens[lexer->size].index = token_start;
				lexer->size++;
				token_start = token_end;
				var_find = 2;
				break;
			}
		}

		if (var_find == 2)
			continue;

		ID_NAME_TABLE_REALLOC(id_name_table);

		id_name_table->data[id_name_table->size].num = (int)id_name_table->size;
		id_name_table->data[id_name_table->size].define_status = 0;
		id_name_table->data[id_name_table->size].length = (size_t)(token_end - token_start);
		id_name_table->data[id_name_table->size].string = buffer + token_start;
		id_name_table->data[id_name_table->size].type = ID_VAR;

		lexer->tokens[lexer->size].type = IDENTIFIER;
		lexer->tokens[lexer->size].data.val_id = (size_t)id_name_table->data[id_name_table->size++].num;
		lexer->tokens[lexer->size].index = token_start;
		lexer->size++;
		token_start = token_end;
	}

#ifdef PRINT_DEBUG
		PrintLexer(lexer, id_name_table);
		PrintIdNameTable(id_name_table);
#endif

	return TREE_NO_ERROR;
}
