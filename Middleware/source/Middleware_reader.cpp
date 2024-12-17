#include "Middleware_reader.hpp"

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
		case KEYWORD: 					return _KEYWORD((KeyWordNum)lexer->tokens[(*pc)++].data.val_num);
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

	if ((lexer->tokens[*pc].type == KEYWORD && lexer->tokens[*pc].data.val_key_word != OPEN_ROUND) || lexer->tokens[*pc].type != KEYWORD) {
		TREE_ERROR_MESSAGE(TREE_ALLOC_ERROR);
		return NULL;
	}

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

	if ((lexer->tokens[*pc].type == KEYWORD && lexer->tokens[*pc].data.val_key_word != CLOSE_ROUND) || lexer->tokens[*pc].type != KEYWORD) {
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

BinaryTreeStatusCode ReadTreeStandard(Tree* tree, IdNameTable* id_name_table, Lexer* lexer) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	FILE* tree_file = fopen(TREE_STANDARD_FILE_, "r");
	if (!tree_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

	struct stat file_info = {};
	stat(TREE_STANDARD_FILE_, &file_info);

	size_t size = (size_t)file_info.st_size;
	char* buffer = (char*)calloc(size, sizeof(char));
	if (!buffer)
		TREE_ERROR_CHECK(TREE_ALLOC_ERROR);

	size_t read_check = fread(buffer, sizeof(char), size, tree_file);
	if (read_check != size)
		TREE_ERROR_CHECK(TREE_READ_ERROR);

	if (fclose(tree_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

#ifdef PRINT_DEBUG
	for (size_t i = 0; i < size; i++) {
		printf("%c", buffer[i]);
	}
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
