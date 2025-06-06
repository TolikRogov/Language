#include "Lexer.hpp"

BinaryTreeStatusCode SkipExtra(const wchar_t* buffer, size_t* token_start, size_t size) {

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

static const wchar_t* GetTokenStringType(NodeType type) {
	switch (type) {
		case NUMBER: 				return L"NUMBER";
		case IDENTIFIER: 			return L"IDENTIFIER";
		case KEYWORD:  				return L"KEYWORD";
		case FUNCTION_DEFINITION: 	return L"FUNCTION_DEFINITION";
		case PARAMETERS:			return L"PARAMETERS";
		case VAR_DECLARATION:		return L"VAR_DECLARATION";
		case UNW:
		default:  return L"UNW";
	}
}

BinaryTreeStatusCode PrintTokenValue(Token* token, IdNameTable* id_name_table) {

	switch (token->type) {
		case NUMBER: 				{ printf(GREEN("%lg"), token->data.val_num); break; }
		case IDENTIFIER: {
			for (size_t i = 0; i < id_name_table->data[token->data.val_id].length; i++)
				printf(GREEN("%lc"), id_name_table->data[token->data.val_id].string[i]);
			printf(" (%zu)", token->data.val_id);
			break;
		}
		case KEYWORD:  				{ printf(GREEN("%ls (%d)"), KeyWordsGetString(token->data.val_key_word), token->data.val_key_word); break; }
		case FUNCTION_DEFINITION: {
			for (size_t j = 0; j < id_name_table->data[token->data.val_func_def].length; j++)
				printf(GREEN("%lc"), id_name_table->data[token->data.val_func_def].string[j]);
			printf(" (%zu)", token->data.val_func_def);
			break;
		}
		case VAR_DECLARATION: {
			for (size_t j = 0; j < id_name_table->data[token->data.val_decl_var].length; j++)
				printf(GREEN("%lc"), id_name_table->data[token->data.val_decl_var].string[j]);
			printf(" (%zu)", token->data.val_decl_var);
			break;
		}
		case PARAMETERS:		 	{ printf(GREEN("%ls"), L"PARAMETERS"); break; }
		case UNW:
		default:  					{ printf(GREEN("%ls"), L"UNW"); break; }
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
		printf(BLUE("Lexer token[%.2zu]:")
					" type - " GREEN("%10ls")
					" index - " GREEN("%5zu"),
					i,
					GetTokenStringType(lexer->tokens[i].type), lexer->tokens[i].index);
		printf(" value - ");
		PrintTokenValue(&lexer->tokens[i], id_name_table);
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode FindOperation(wchar_t* buffer, Lexer* lexer, LexicalAnalysis_t* lxa) {

	for (size_t i = 0; keywords[i].num != AMOUNT_OF_KEYWORDS; i++) {
		*lxa->op_find = 1;
		size_t j = 0;
		for (; *lxa->token_start + j != *lxa->token_end && *(keywords[i].string + j) != '\0'; j++) {
			if (buffer[*lxa->token_start + j] != *(keywords[i].string + j)) {
				*lxa->op_find = 0;
				break;
			}
		}
		if (*lxa->op_find && *lxa->token_start + j == *lxa->token_end && *(keywords[i].string + j) == '\0') {
			lexer->tokens[lexer->size].type = KEYWORD;
			lexer->tokens[lexer->size].data.val_key_word = keywords[i].num;
			lexer->tokens[lexer->size].index = *lxa->token_start;
			lexer->size++;
			*lxa->token_start = *lxa->token_end;
			*lxa->op_find = 2;
			break;
		}
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode UnaryMinus(Lexer* lexer) {

	for (size_t i = 0; i < lexer->size; i++) {
		if ((lexer->tokens[i].type == KEYWORD && lexer->tokens[i].data.val_key_word != SUB) || lexer->tokens[i].type != KEYWORD)
			continue;

		if (i > 0 && lexer->tokens[i - 1].type == KEYWORD && lexer->tokens[i + 1].type == NUMBER) {
			lexer->tokens[i].type = NUMBER;
			lexer->tokens[i].data.val_num = lexer->tokens[i + 1].data.val_num * (-1);
			for (size_t j = i + 1; j < lexer->size - 1; j++) {
				lexer->tokens[j].type = lexer->tokens[j + 1].type;
				switch (lexer->tokens[j + 1].type) {
					case NUMBER: 		{ lexer->tokens[j].data.val_num 		= lexer->tokens[j + 1].data.val_num; 		break; }
					case IDENTIFIER: 	{ lexer->tokens[j].data.val_id 			= lexer->tokens[j + 1].data.val_id; 		break; }
					case KEYWORD: 		{ lexer->tokens[j].data.val_key_word	= lexer->tokens[j + 1].data.val_key_word; 	break; }
					default: 			break;
				}
				lexer->tokens[j].index = lexer->tokens[j + 1].index;
			}
		}
	}

	return TREE_NO_ERROR;
}

int GetNumber(wchar_t* buffer, Lexer* lexer, LexicalAnalysis_t* lxa) {

	Number_t number = wcstod(buffer + *lxa->token_start, lxa->token_end_pointer);
	if (buffer + *lxa->token_start != *lxa->token_end_pointer) {
		lexer->tokens[lexer->size].type = NUMBER;
		lexer->tokens[lexer->size].data.val_num = number;
		lexer->tokens[lexer->size].index = *lxa->token_start;
		lexer->size++;
		*lxa->token_start = (size_t)(*lxa->token_end_pointer - buffer);

		return 1;
	}

	return 0;
}

BinaryTreeStatusCode FindVariable(wchar_t* buffer, Lexer* lexer, LexicalAnalysis_t* lxa) {

	for (size_t i = 0; i < lxa->id_name_table->size; i++) {
		*lxa->var_find = 1;
		size_t j = 0;
		for (; *lxa->token_start + j != *lxa->token_end && j != lxa->id_name_table->data[i].length; j++) {
			if (buffer[*lxa->token_start + j] != *(lxa->id_name_table->data[i].string + j)) {
				*lxa->var_find = 0;
				break;
			}
		}
		if (*lxa->var_find && *lxa->token_start + j == *lxa->token_end && j == lxa->id_name_table->data[i].length) {
			lexer->tokens[lexer->size].type = IDENTIFIER;
			lexer->tokens[lexer->size].data.val_id = (size_t)lxa->id_name_table->data[i].num;
			lexer->tokens[lexer->size].index = *lxa->token_start;
			lexer->size++;
			*lxa->token_start = *lxa->token_end;
			*lxa->var_find = 2;
			break;
		}
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode AddNewVariable(wchar_t* buffer, Lexer* lexer, LexicalAnalysis_t* lxa) {

	Identifier* cur_id = &lxa->id_name_table->data[lxa->id_name_table->size];

	cur_id->num = (int)lxa->id_name_table->size;
	cur_id->define_status = 0;
	cur_id->length = (size_t)(*lxa->token_end - *lxa->token_start);
	cur_id->string = buffer + *lxa->token_start;
	cur_id->type = ID_VAR;

	lexer->tokens[lexer->size].type = IDENTIFIER;
	lexer->tokens[lexer->size].data.val_id = (size_t)lxa->id_name_table->data[lxa->id_name_table->size++].num;
	lexer->tokens[lexer->size].index = *lxa->token_start;
	lexer->size++;

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode ReplacingShift(Lexer* lexer, size_t i, size_t cnt) {

	for (size_t j = i + 1; j < lexer->capacity - cnt + 1; j++) {
		lexer->tokens[j].type = lexer->tokens[j + cnt - 1].type;
		switch (lexer->tokens[j + cnt - 1].type) {
			case NUMBER: 		{ lexer->tokens[j].data.val_num 		= lexer->tokens[j + cnt - 1].data.val_num; 		break; }
			case IDENTIFIER: 	{ lexer->tokens[j].data.val_id 			= lexer->tokens[j + cnt - 1].data.val_id; 		break; }
			case KEYWORD: 		{ lexer->tokens[j].data.val_key_word	= lexer->tokens[j + cnt - 1].data.val_key_word; 	break; }
			default: 			break;
		}
		lexer->tokens[j].index = lexer->tokens[j + cnt - 1].index;
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode ReplaceToken(Token* token, Data_t data) {

	token->data.val_key_word = data.val_key_word;

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode SecondRun(Lexer* lexer) {

#define TOKEN_VAL(num) lexer->tokens[(num)].data.val_key_word
#define TOKEN(num) &lexer->tokens[(num)]

	#include "ReplaceTable.hpp"

	size_t replace_table_size = sizeof(ReplaceTable) / sizeof(ReplaceTable[0]);

	for (int i = 0; i < (int)lexer->capacity; i++) {

		for (int rule = 0; rule < (int)replace_table_size; rule++) {

			int replace_status = 1;
			int tkn = 0;

			for (; ReplaceTable[rule].array[tkn] != LEX_END; tkn++) {
				if (ReplaceTable[rule].array[tkn] != TOKEN_VAL(i + tkn)) {
					replace_status = 0;
					break;
				}
			}

			if (replace_status) {
				ReplaceToken(TOKEN(i), {.val_key_word = ReplaceTable[rule].changed});
				ReplacingShift(lexer, (size_t)i, (size_t)(tkn));
				break;
			}
		}
	}

#undef TOKEN
#undef TOKEN_VAL

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode LexerFillByEmpty(Lexer* lexer, size_t cnt) {

	for (size_t i = cnt; i < lexer->capacity; i++) {
		lexer->tokens[i].data.val_num = 0;
		lexer->tokens[i].index = 0;
		lexer->tokens[i].type = UNW;
	}

	return TREE_NO_ERROR;
}

size_t LexerFindSize(Lexer* lexer) {

	for (size_t i = 0; i < lexer->capacity; i++) {
		if (lexer->tokens[i].type == UNW)
			return i;
	}

	return 0;
}

BinaryTreeStatusCode LexicalAnalysis(wchar_t* buffer, Lexer* lexer, IdNameTable* id_name_table, size_t size) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	size_t token_start = 0;
	wchar_t* token_end_pointer = NULL;
	size_t token_end = 1;
	int op_find = 0;
	int var_find = 0;

	LexicalAnalysis_t lxa = { 	.id_name_table = id_name_table,
								.size = lexer->buffer_size,
								.token_start = &token_start,
								.token_end = &token_end,
								.op_find = &op_find,
								.var_find = &var_find,
								.token_end_pointer = &token_end_pointer};

	while (true) {
		LEXER_REALLOC(lexer);

		SKIP_EXTRA(buffer, &token_start, size);

		if (token_start == size)
			break;

		token_end = token_start + 1;

		while ((iswalpha(buffer[token_end]) || buffer[token_end] == '_') &&
			  ((iswalpha(buffer[token_end - 1])) || buffer[token_end - 1] == '_')) {
			  token_end++;
		}

		op_find = 0;
		FindOperation(buffer, lexer, &lxa);
		if (op_find == 2)
			continue;

		if (GetNumber(buffer, lexer, &lxa))
			continue;

		while ((iswalpha(buffer[token_end]) || isdigit(buffer[token_end]) || buffer[token_end] == '_') &&
			   (iswalpha(buffer[token_end - 1]) || isdigit(buffer[token_end - 1]) || buffer[token_end - 1] == '_'))
			{ token_end++; }

		var_find = 0;
		FindVariable(buffer, lexer, &lxa);
		if (var_find == 2)
			continue;

		ID_NAME_TABLE_REALLOC(id_name_table);

		AddNewVariable(buffer, lexer, &lxa);
		token_start = token_end;
	}

	UnaryMinus(lexer);
	SecondRun(lexer);
	LexerFillByEmpty(lexer, (lexer->size = LexerFindSize(lexer)));

#ifdef PRINT_DEBUG
	PrintLexer(lexer, id_name_table);
	PrintIdNameTable(id_name_table);
#endif

	return TREE_NO_ERROR;
}
