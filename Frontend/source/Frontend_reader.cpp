#include "Frontend_reader.hpp"
#include "Tree_dump.hpp"

Node_t* GetGrammar(Lexer* lexer, size_t* pc);
Node_t* GetOperator(Lexer* lexer, size_t* pc);
Node_t* GetAssignment(Lexer* lexer, size_t* pc);
Node_t* GetIf(Lexer* lexer, size_t* pc);
Node_t* GetExpression(Lexer* lexer, size_t* pc);
Node_t* GetTerminator(Lexer* lexer, size_t* pc);
Node_t* GetPriority(Lexer* lexer, size_t* pc);
Node_t* GetPower(Lexer* lexer, size_t* pc);
Node_t* GetFunction(Lexer* lexer, size_t* pc);
Node_t* GetIdentifier(Lexer* lexer, size_t* pc);
Node_t* GetNumber(Lexer* lexer, size_t* pc);

Node_t* GetNumber(Lexer* lexer, size_t* pc) {

#ifdef PRINT_DEBUG
	printf("Number (%zu): ", *pc);
	PrintTokenValueGrammar(&lexer->tokens[*pc]);
#endif

	if (lexer->tokens[*pc].type == NUMBER)
		return _NUM(lexer->tokens[(*pc)++].data.val_num);
	else
		return NULL;

}

Node_t* GetIdentifier(Lexer* lexer, size_t* pc) {

#ifdef PRINT_DEBUG
	printf("Identifier (%zu): ", *pc);
	PrintTokenValueGrammar(&lexer->tokens[*pc]);
#endif

	if (lexer->tokens[*pc].type == IDENTIFIER)
		return _ID(lexer->tokens[(*pc)++].data.val_id);
	else
		return NULL;
}

Node_t* GetFunction(Lexer* lexer, size_t* pc) {

#ifdef PRINT_DEBUG
	printf("Function (%zu): ", *pc);
	PrintTokenValueGrammar(&lexer->tokens[*pc]);
#endif

	Node_t* node = NULL;

	if (lexer->tokens[*pc].type == KEYWORD) {
		KeyWordNum key_word = lexer->tokens[(*pc)++].data.val_key_word;

		if (lexer->tokens[*pc].data.val_key_word == OPEN_ROUND) {
			(*pc)++;
			node = GetExpression(lexer, pc);
			if (!node) {
				TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
				return NULL;
			}

			if (lexer->tokens[*pc].data.val_key_word != CLOSE_ROUND) {
				TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
				return NULL;
			}

			(*pc)++;
			switch (key_word) {
				case SIN:  return node = _SIN(node);
				case COS:  return node = _COS(node);
				case SQRT: return node = _SQRT(node);
				default:   return NULL;
			}
		}
	}

	return node;
}

Node_t* GetPower(Lexer* lexer, size_t* pc) {

#ifdef PRINT_DEBUG
	printf("Power (%zu): ", *pc);
	PrintTokenValueGrammar(&lexer->tokens[*pc]);
#endif

	Node_t* node1 = GetPriority(lexer, pc);
	if (!node1) {
		TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
		return NULL;
	}

	while (lexer->tokens[*pc].data.val_key_word == POW) {
		(*pc)++;
		Node_t* node2 = GetPriority(lexer, pc);
		if (!node2) {
			TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
			return NULL;
		}

		node1 = _POW(node1, node2);
	}

	return node1;
}

Node_t* GetPriority(Lexer* lexer, size_t* pc) {

#ifdef PRINT_DEBUG
	printf("Priority (%zu): ", *pc);
	PrintTokenValueGrammar(&lexer->tokens[*pc]);
#endif

	Node_t* node = NULL;

	if (lexer->tokens[*pc].data.val_key_word == OPEN_ROUND) {
		(*pc)++;
		node = GetExpression(lexer, pc);
		if (!node) {
			TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
			return NULL;
		}

		if (lexer->tokens[*pc].data.val_key_word != CLOSE_ROUND) {
			TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
			return NULL;
		}

		(*pc)++;
		return node;
	}
	if ((node = GetNumber(lexer, pc)) != NULL)
		return node;

	if ((node = GetIdentifier(lexer, pc)) != NULL)
		return node;

	if ((node = GetFunction(lexer, pc)) != NULL)
		return node;

	return NULL;
}

Node_t* GetTerminator(Lexer* lexer, size_t* pc) {

#ifdef PRINT_DEBUG
	printf("Terminator (%zu): ", *pc);
	PrintTokenValueGrammar(&lexer->tokens[*pc]);
#endif

	Node_t* node1 = GetPower(lexer, pc);
	if (!node1) {
		TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
		return NULL;
	}

	while (lexer->tokens[*pc].data.val_key_word == MUL || lexer->tokens[*pc].data.val_key_word == DIV) {
		KeyWordNum key_word = lexer->tokens[(*pc)++].data.val_key_word;
		Node_t* node2 = GetPower(lexer, pc);
		if (!node2) {
			TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
			return NULL;
		}

		if (key_word == MUL)
			node1 = _MUL(node1, node2);
		else
			node1 = _DIV(node1, node2);
	}

	return node1;
}

Node_t* GetExpression(Lexer* lexer, size_t* pc) {

#ifdef PRINT_DEBUG
	printf("Expression (%zu): ", *pc);
	PrintTokenValueGrammar(&lexer->tokens[*pc]);
#endif

	Node_t* node1 = GetTerminator(lexer, pc);
	if (!node1) {
		TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
		return NULL;
	}

	while (lexer->tokens[*pc].data.val_key_word == ADD || lexer->tokens[*pc].data.val_key_word == SUB) {
		KeyWordNum key_word = lexer->tokens[(*pc)++].data.val_key_word;
		Node_t* node2 = GetTerminator(lexer, pc);
		if (!node2) {
			TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
			return NULL;
		}

		if (key_word == ADD)
			node1 = _ADD(node1, node2);
		else
			node1 = _SUB(node1, node2);
	}

	return node1;
}

Node_t* GetAssignment(Lexer* lexer, size_t* pc) {

#ifdef PRINT_DEBUG
	printf("Assignment (%zu): ", *pc);
	PrintTokenValueGrammar(&lexer->tokens[*pc]);
#endif

	Node_t* node = GetIdentifier(lexer, pc);
	if (!node)
		return NULL;

	if (lexer->tokens[(*pc)++].data.val_key_word != ASSIGNMENT) {
		TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
		return NULL;
	}

	Node_t* node2 = GetExpression(lexer, pc);
	if (!node2) {
		TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
		return NULL;
	}

	return node = _ASSIGNMENT(node, node2);
}

Node_t* GetIf(Lexer* lexer, size_t* pc) {

#ifdef PRINT_DEBUG
	printf("If (%zu): ", *pc);
	PrintTokenValueGrammar(&lexer->tokens[*pc]);
#endif

#define TERMINALS_CHECKER(terminal) {									 \
	if (lexer->tokens[(*pc)++].data.val_key_word != terminal) {			\
		TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);				\
		return NULL;													\
	}																	\
}

	TERMINALS_CHECKER(IF);

	TERMINALS_CHECKER(OPEN_ROUND);
	Node_t* node = GetExpression(lexer, pc);
	if (!node) {
		TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
		return NULL;
	}
	TERMINALS_CHECKER(CLOSE_ROUND);

	TERMINALS_CHECKER(OPEN_FIGURE);
	Node_t* node2 = GetOperator(lexer, pc);
	if (!node2) {
		TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
		return NULL;
	}
	TERMINALS_CHECKER(SEQUENTIAL_OP)

	node2 = _SEQUENTIAL_OP(node2, NULL);

	while (lexer->tokens[(*pc)].data.val_key_word != CLOSE_FIGURE) {
		(*pc)++;
		Node_t* node3 = GetOperator(lexer, pc);
		if (!node3) {
			TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
			return NULL;
		}
		TERMINALS_CHECKER(SEQUENTIAL_OP);

		node2->right = _SEQUENTIAL_OP(node3, NULL);
	}

	TERMINALS_CHECKER(CLOSE_FIGURE);

#undef TERMINAL_CHECKER

	return node = _IF(node, node2);
}

Node_t* GetOperator(Lexer* lexer, size_t* pc) {

#ifdef PRINT_DEBUG
	printf("Operator (%zu): ", *pc);
	PrintTokenValueGrammar(&lexer->tokens[*pc]);
#endif

	Node_t* node = NULL;

	if ((node = GetAssignment(lexer, pc)) != NULL)
		return node;

	if ((node = GetIf(lexer, pc)) != NULL)
		return node;

	return NULL;
}

Node_t* GetGrammar(Lexer* lexer, size_t* pc) {

#ifdef PRINT_DEBUG
	printf("Grammar (%zu): ", *pc);
	PrintTokenValueGrammar(&lexer->tokens[*pc]);
#endif

	Node_t* node = GetOperator(lexer, pc);
	if (!node) {
		TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
		return NULL;
	}

	if (lexer->tokens[(*pc)++].data.val_key_word != SEQUENTIAL_OP) {
		TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
		return NULL;
	}

	node = _SEQUENTIAL_OP(node, NULL);

	Node_t* prev_node = node;
	while ((*pc) != lexer->size) {
		Node_t* node2 = GetOperator(lexer, pc);
		if (!node2) {
			TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
			return NULL;
		}

		if (lexer->tokens[(*pc)++].data.val_key_word != SEQUENTIAL_OP) {
			TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
			return NULL;
		}

		node2 = _SEQUENTIAL_OP(node2, NULL);
		prev_node->right = node2;
		node2->parent = prev_node;
		prev_node = node2;
	}

	if ((*pc)++ != lexer->size) {
		TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
		return NULL;
	}

	return node;
}

BinaryTreeStatusCode ReadProgram(Tree* tree, IdNameTable* id_name_table, Lexer* lexer) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	FILE* prog_file = fopen(FRONT_PROG_FILE_, "r");
	if (!prog_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

	struct stat file_info = {};
	stat(FRONT_PROG_FILE_, &file_info);

	size_t size = (size_t)file_info.st_size;
	char* buffer = (char*)calloc(size, sizeof(char));
	if (!buffer)
		TREE_ERROR_CHECK(TREE_ALLOC_ERROR);

	size_t read_check = fread(buffer, sizeof(char), size, prog_file);
	if (read_check != size)
		TREE_ERROR_CHECK(TREE_READ_ERROR);

	if (fclose(prog_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

#ifdef PRINT_DEBUG
	for (size_t i = 0; i < size; i++) {
		printf("%c", buffer[i]);
	}
#endif

	lexer->buffer = buffer;
	LEXICAL_ANALYSIS(buffer, lexer, id_name_table, size);

	size_t pc = 0;
	tree->root = GetGrammar(lexer, &pc);
	if (!tree->root)
		TREE_ERROR_CHECK(TREE_EXPRESSION_SYNTAX_ERROR);

	BINARY_TREE_GRAPH_DUMP(tree, "ReadProgram", NULL, id_name_table);

	return TREE_NO_ERROR;
}
