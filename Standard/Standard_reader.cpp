#include "Standard_reader.hpp"

Node_t* GetRoot(Lexer* lexer, size_t* pc, IdNameTable* id_name_table);
Node_t* GetNode(Lexer* lexer, size_t* pc, IdNameTable* id_name_table);
Node_t* GetNodeValue(Lexer* lexer, size_t* pc, IdNameTable* id_name_table);

Node_t* GetNodeValue(Lexer* lexer, size_t* pc, IdNameTable* id_name_table) {

#ifdef PRINT_DEBUG
	printf("GetNodeValue (%zu): ", *pc);
	PrintTokenValue(&lexer->tokens[*pc], id_name_table);
#endif

	if (lexer->tokens[(*pc)].type != NUMBER)
		return NULL;

	switch ((int)lexer->tokens[(*pc)++].data.val_num) {
		case NUMBER: 					return _NUM(lexer->tokens[(*pc)++].data.val_num);
		case IDENTIFIER: 				return _ID((size_t)lexer->tokens[(*pc)++].data.val_num);
		case KEYWORD:					return _KEYWORD((KeyWordNum)lexer->tokens[(*pc)++].data.val_num, NULL, NULL);
		case FUNCTION_DEFINITION: 		return _FUNC_DEF((size_t)lexer->tokens[(*pc)++].data.val_num, NULL, NULL);
		case VAR_DECLARATION: 			return _VAR_DEC((size_t)lexer->tokens[(*pc)++].data.val_num, NULL, NULL);
		case PARAMETERS: 				return _PARAMETERS(NULL, NULL);
		case CALL:		 				return _CALL(NULL, NULL);
		default: {
			TREE_ERROR_MESSAGE(TREE_LANGUAGE_SYNTAX_ERROR);
			return NULL;
		}
	}
}

Node_t* GetNode(Lexer* lexer, size_t* pc, IdNameTable* id_name_table) {

#ifdef PRINT_DEBUG
	printf("GetNode (%zu): ", *pc);
	PrintTokenValue(&lexer->tokens[*pc], id_name_table);
#endif

	if (!lexer || !pc || !id_name_table)
		return NULL;

	Node_t* node = NULL;

	if ((lexer->tokens[*pc].type == KEYWORD && lexer->tokens[*pc].data.val_key_word != STD_OPEN_ROUND) || lexer->tokens[*pc].type != KEYWORD)
		return NULL;

	(*pc)++;
	if (lexer->tokens[(*pc)].type != NUMBER)
		return NULL;

	if ((node = GetNodeValue(lexer, pc, id_name_table)) == NULL) {
		TREE_ERROR_MESSAGE(TREE_LANGUAGE_SYNTAX_ERROR);
		return NULL;
	}

	switch (lexer->tokens[(*pc)].type) {
		case IDENTIFIER: { (*pc)++; break; }
		case KEYWORD: {
			node->left = GetNode(lexer, pc, id_name_table);
			if (node->left) node->left->parent = node;
			break;
		}
		default: { TREE_ERROR_MESSAGE(TREE_LANGUAGE_SYNTAX_ERROR); return NULL; }
	}

	switch (lexer->tokens[(*pc)].type) {
		case IDENTIFIER: { (*pc)++; break; }
		case KEYWORD: {
			node->right = GetNode(lexer, pc, id_name_table);
			if (node->right) node->right->parent = node;
			break;
		}
		default: { TREE_ERROR_MESSAGE(TREE_LANGUAGE_SYNTAX_ERROR); return NULL; }
	}

	if ((lexer->tokens[*pc].type == KEYWORD && lexer->tokens[*pc].data.val_key_word != STD_CLOSE_ROUND) || lexer->tokens[*pc].type != KEYWORD) {
		TREE_ERROR_MESSAGE(TREE_ALLOC_ERROR);
		return NULL;
	}

	(*pc)++;

	return node;
}

Node_t* GetRoot(Lexer* lexer, size_t* pc, IdNameTable* id_name_table) {

#ifdef PRINT_DEBUG
	printf("GetRoot (%zu): ", *pc);
	PrintTokenValue(&lexer->tokens[*pc], id_name_table);
#endif

	Node_t* node = GetNode(lexer, pc, id_name_table);
	if (!node) {
		TREE_ERROR_MESSAGE(TREE_LANGUAGE_SYNTAX_ERROR);
		return NULL;
	}

	if ((*pc)++ != lexer->size) {
		TREE_ERROR_MESSAGE(TREE_LANGUAGE_SYNTAX_ERROR);
		return NULL;
	}

	return node;
}

static BinaryTreeStatusCode NameTableReader(IdNameTable* id_name_table) {

	wchar_t* count_end = NULL;
	id_name_table->size = id_name_table->capacity = (size_t)wcstol(id_name_table->buffer, &count_end, 10);
	if (!id_name_table->capacity)
		TREE_ERROR_CHECK(TREE_READ_ERROR);

	size_t pc = (size_t)(count_end - id_name_table->buffer);
	pc++;

	id_name_table->data = (Identifier*)realloc(id_name_table->data, id_name_table->capacity * sizeof(Identifier));
	if (!id_name_table->data)
		TREE_ERROR_CHECK(TREE_ALLOC_ERROR);

	//General Name Table read
	for (size_t i = 0; i < id_name_table->capacity; i++) {
		id_name_table->data[i].define_status = 0;
		id_name_table->data[i].global = 0;
		id_name_table->data[i].length = 0;
		id_name_table->data[i].num = 0;
		id_name_table->data[i].scope_variables = {};
		id_name_table->data[i].string = NULL;
		id_name_table->data[i].type = ID_VAR;

		id_name_table->data[i].string = id_name_table->buffer + pc;

		size_t length = pc;
		while (id_name_table->buffer[pc] != '\n') pc++;
		length = pc++ - length;

		id_name_table->data[i].length = length;
		id_name_table->data[i].num = (int)i;
	}

	//Scopes count
	pc++;
	int scopes_count = (int)wcstol(id_name_table->buffer + pc, &count_end, 10);
	if (count_end == id_name_table->buffer + pc)
		TREE_ERROR_CHECK(TREE_READ_ERROR);

	if (scopes_count < 1)
		TREE_ERROR_CHECK(TREE_LANGUAGE_SYNTAX_ERROR);

	pc = (size_t)(count_end - id_name_table->buffer);
	pc += 2;

	//Global scope
	int global_scope_size = (int)wcstol(id_name_table->buffer + pc, &count_end, 10);
	if (count_end == id_name_table->buffer + pc)
		TREE_ERROR_CHECK(TREE_READ_ERROR);

	if (global_scope_size < 0)
		TREE_ERROR_CHECK(TREE_LANGUAGE_SYNTAX_ERROR);

	pc = (size_t)(count_end - id_name_table->buffer);
	pc++;

	int global_scope = (int)wcstol(id_name_table->buffer + pc, &count_end, 10);
	if (count_end == id_name_table->buffer + pc)
		TREE_ERROR_CHECK(TREE_READ_ERROR);

	if (global_scope != -1)
		TREE_ERROR_CHECK(TREE_LANGUAGE_SYNTAX_ERROR);

	pc = (size_t)(count_end - id_name_table->buffer);
	pc++;

	//Global scope
	for (size_t i = 0; i < (size_t)global_scope_size; i++) {
		int id_num = (int)wcstol(id_name_table->buffer + pc, &count_end, 10);
		if (count_end == id_name_table->buffer + pc)
			TREE_ERROR_CHECK(TREE_READ_ERROR);

		if (id_num < 0)
			TREE_ERROR_CHECK(TREE_LANGUAGE_SYNTAX_ERROR);

		pc = (size_t)(count_end - id_name_table->buffer);
		pc++;

		int id_type = (int)wcstol(id_name_table->buffer + pc, &count_end, 10);
		if (count_end == id_name_table->buffer + pc)
			TREE_ERROR_CHECK(TREE_READ_ERROR);

		if (id_type < 1)
			TREE_ERROR_CHECK(TREE_LANGUAGE_SYNTAX_ERROR);

		pc = (size_t)(count_end - id_name_table->buffer);
		pc++;

		id_name_table->data[id_num].type = (IdType)id_type;
		id_name_table->data[id_num].global = 1;
		id_name_table->data[id_num].define_status = 1;
	}

	//Other scopes
	pc++;
	for (size_t i = 0; i < (size_t)(scopes_count - 1); i++) {
		int cur_scope_size = (int)wcstol(id_name_table->buffer + pc, &count_end, 10);
		if (count_end == id_name_table->buffer + pc)
			TREE_ERROR_CHECK(TREE_READ_ERROR);

		if (cur_scope_size < 0)
			TREE_ERROR_CHECK(TREE_LANGUAGE_SYNTAX_ERROR);

		pc = (size_t)(count_end - id_name_table->buffer);
		pc++;

		int id_num = (int)wcstol(id_name_table->buffer + pc, &count_end, 10);
		if (count_end == id_name_table->buffer + pc)
			TREE_ERROR_CHECK(TREE_READ_ERROR);

		if (id_num < 0)
			TREE_ERROR_CHECK(TREE_LANGUAGE_SYNTAX_ERROR);

		pc = (size_t)(count_end - id_name_table->buffer);
		pc++;

		id_name_table->data[id_num].scope_variables.capacity = id_name_table->data[id_num].scope_variables.size = (size_t)cur_scope_size;
		id_name_table->data[id_num].scope_variables.data = (int*)calloc((size_t)cur_scope_size, sizeof(int));
		if (!id_name_table->data[id_num].scope_variables.data)
			TREE_ERROR_CHECK(TREE_ALLOC_ERROR);
		for (size_t j = 0; j < (size_t)cur_scope_size; j++) {
			int local_id_num = (int)wcstol(id_name_table->buffer + pc, &count_end, 10);
			if (count_end == id_name_table->buffer + pc)
				TREE_ERROR_CHECK(TREE_READ_ERROR);

			if (local_id_num < 0)
				TREE_ERROR_CHECK(TREE_LANGUAGE_SYNTAX_ERROR);

			pc = (size_t)(count_end - id_name_table->buffer);
			pc++;

			int local_id_type = (int)wcstol(id_name_table->buffer + pc, &count_end, 10);
			if (count_end == id_name_table->buffer + pc)
				TREE_ERROR_CHECK(TREE_READ_ERROR);

			if (local_id_type < 1)
				TREE_ERROR_CHECK(TREE_LANGUAGE_SYNTAX_ERROR);

			pc = (size_t)(count_end - id_name_table->buffer);
			pc++;

			id_name_table->data[id_num].scope_variables.data[j] = local_id_num;
			id_name_table->data[local_id_num].define_status = 1;
			id_name_table->data[local_id_num].type = (IdType)local_id_type;
		}
	}

#ifdef PRINT_DEBUG
	PrintIdNameTable(id_name_table);
#endif

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode ReadNameTableStandard(IdNameTable* id_name_table) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	FILE* name_table_file = fopen(NAME_TABLE_STANDARD_FILE_, "r");
	if (!name_table_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

	struct stat file_info = {};
	stat(NAME_TABLE_STANDARD_FILE_, &file_info);

	size_t size = (size_t)file_info.st_size;
	wchar_t* buffer = (wchar_t*)calloc(size, sizeof(wchar_t));
	if (!buffer)
		TREE_ERROR_CHECK(TREE_ALLOC_ERROR);

	wchar_t c = 0;
	size_t ip = 0;
	while ((c = fgetwc(name_table_file)) != EOF) { buffer[ip] = c; ip++; }

	if (fclose(name_table_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

#ifdef PRINT_DEBUG
	for (size_t i = 0; i < ip; i++) printf("%lc", buffer[i]);
	printf("%zu\n", ip);
#endif

	id_name_table->buffer = buffer;

	tree_status = NameTableReader(id_name_table);
	TREE_ERROR_CHECK(tree_status);

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode ReadTreeStandard(Tree* tree, IdNameTable* id_name_table, Lexer* lexer) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	FILE* tree_file = fopen(TREE_STANDARD_FILE_, "r");
	if (!tree_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

	struct stat file_info = {};
	stat(TREE_STANDARD_FILE_, &file_info);

	size_t size = (size_t)file_info.st_size;
	wchar_t* buffer = (wchar_t*)calloc(size, sizeof(wchar_t));
	if (!buffer)
		TREE_ERROR_CHECK(TREE_ALLOC_ERROR);

	wchar_t c = 0;
	size_t ip = 0;
	while ((c = fgetwc(tree_file)) != EOF) { buffer[ip] = c; ip++; }

	if (fclose(tree_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

#ifdef PRINT_DEBUG
	for (size_t i = 0; i < ip; i++) printf("%lc", buffer[i]);
	printf("%zu\n", ip);
#endif

	lexer->buffer = buffer;
	lexer->buffer_size = size;
	LEXICAL_ANALYSIS(buffer, lexer, id_name_table, size);

	size_t pc = 0;
	tree->root = GetRoot(lexer, &pc, id_name_table);
	if (!tree->root)
		TREE_ERROR_CHECK(TREE_LANGUAGE_SYNTAX_ERROR);

	NodeParentChecker(tree->root);
	NAME_TABLE_PRINT(id_name_table);
	BINARY_TREE_GRAPH_DUMP(tree, "ReadTreeStandard", NULL, id_name_table);

	return TREE_NO_ERROR;
}
