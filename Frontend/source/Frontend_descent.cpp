#include "Frontend_descent.hpp"
#include "Tree_dump.hpp"

Node_t* GetGrammar(Lexer* lexer, size_t* pc);
Node_t* GetFunction(Lexer* lexer, size_t* pc);
Node_t* GetStatement(Lexer* lexer, size_t* pc);
Node_t* GetAssignment(Lexer* lexer, size_t* pc);
Node_t* GetIf(Lexer* lexer, size_t* pc);
Node_t* GetExpression(Lexer* lexer, size_t* pc);
Node_t* GetTerminator(Lexer* lexer, size_t* pc);
Node_t* GetPriority(Lexer* lexer, size_t* pc);
Node_t* GetPower(Lexer* lexer, size_t* pc);
Node_t* GetOperator(Lexer* lexer, size_t* pc);
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

Node_t* GetOperator(Lexer* lexer, size_t* pc) {

#ifdef PRINT_DEBUG
	printf("Operator (%zu): ", *pc);
	PrintTokenValueGrammar(&lexer->tokens[*pc]);
#endif

	Node_t* node = NULL;

	if (lexer->tokens[*pc].type == KEYWORD) {
		KeyWordNum key_word = lexer->tokens[(*pc)++].data.val_key_word;

		if (lexer->tokens[*pc].data.val_key_word == OPEN_ROUND) {
			(*pc)++;
			node = GetExpression(lexer, pc);
			if (!node)
				LANGUAGE_SYNTAX_ERROR(lexer, pc);

			if (lexer->tokens[*pc].data.val_key_word != CLOSE_ROUND)
				LANGUAGE_SYNTAX_ERROR(lexer, pc);

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
	if (!node1)
		LANGUAGE_SYNTAX_ERROR(lexer, pc);

	while (lexer->tokens[*pc].data.val_key_word == POW) {
		(*pc)++;
		Node_t* node2 = GetPriority(lexer, pc);
		if (!node2)
			LANGUAGE_SYNTAX_ERROR(lexer, pc);

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
		if (!node)
			LANGUAGE_SYNTAX_ERROR(lexer, pc);

		if (lexer->tokens[*pc].data.val_key_word != CLOSE_ROUND)
			LANGUAGE_SYNTAX_ERROR(lexer, pc);

		(*pc)++;
		return node;
	}
	if ((node = GetNumber(lexer, pc)) != NULL)
		return node;

	if ((node = GetIdentifier(lexer, pc)) != NULL)
		return node;

	if ((node = GetOperator(lexer, pc)) != NULL)
		return node;

	return NULL;
}

Node_t* GetTerminator(Lexer* lexer, size_t* pc) {

#ifdef PRINT_DEBUG
	printf("Terminator (%zu): ", *pc);
	PrintTokenValueGrammar(&lexer->tokens[*pc]);
#endif

	Node_t* node1 = GetPower(lexer, pc);
	if (!node1)
		LANGUAGE_SYNTAX_ERROR(lexer, pc);

	while (lexer->tokens[*pc].data.val_key_word == MUL || lexer->tokens[*pc].data.val_key_word == DIV) {
		KeyWordNum key_word = lexer->tokens[(*pc)++].data.val_key_word;
		Node_t* node2 = GetPower(lexer, pc);
		if (!node2)
			LANGUAGE_SYNTAX_ERROR(lexer, pc);

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
	if (!node1)
		LANGUAGE_SYNTAX_ERROR(lexer, pc);

	while (lexer->tokens[*pc].data.val_key_word == ADD || lexer->tokens[*pc].data.val_key_word == SUB) {
		KeyWordNum key_word = lexer->tokens[(*pc)++].data.val_key_word;
		Node_t* node2 = GetTerminator(lexer, pc);
		if (!node2)
			LANGUAGE_SYNTAX_ERROR(lexer, pc);

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

	if (lexer->tokens[(*pc)++].data.val_key_word != ASSIGNMENT)
		LANGUAGE_SYNTAX_ERROR(lexer, pc);

	Node_t* node2 = GetExpression(lexer, pc);
	if (!node2)
		LANGUAGE_SYNTAX_ERROR(lexer, pc);

	return node = _ASSIGNMENT(node, node2);
}

Node_t* GetIf(Lexer* lexer, size_t* pc) {

#ifdef PRINT_DEBUG
	printf("If (%zu): ", *pc);
	PrintTokenValueGrammar(&lexer->tokens[*pc]);
#endif

#define TERMINALS_CHECKER(terminal) {									 \
	if (lexer->tokens[(*pc)++].data.val_key_word != terminal)			\
		LANGUAGE_SYNTAX_ERROR(lexer, pc);								\
}

	if (lexer->tokens[(*pc)++].data.val_key_word != IF)
		return NULL;

	TERMINALS_CHECKER(OPEN_ROUND);
	Node_t* node = GetExpression(lexer, pc);
	if (!node)
		LANGUAGE_SYNTAX_ERROR(lexer, pc);

	TERMINALS_CHECKER(CLOSE_ROUND);

	TERMINALS_CHECKER(OPEN_FIGURE);
	Node_t* node2 = GetStatement(lexer, pc);
	if (!node2)
		LANGUAGE_SYNTAX_ERROR(lexer, pc);

	TERMINALS_CHECKER(SEQUENTIAL_OP)

	node2 = _SEQUENTIAL_OP(node2, NULL);

	Node_t* prev_node = node2;
	while (lexer->tokens[(*pc)].data.val_key_word != CLOSE_FIGURE) {
		Node_t* node3 = GetStatement(lexer, pc);
		if (!node3)
			LANGUAGE_SYNTAX_ERROR(lexer, pc);

		TERMINALS_CHECKER(SEQUENTIAL_OP);

		node3 = _SEQUENTIAL_OP(node3, NULL);
		prev_node->right = node3;
		node3->parent = prev_node;
		prev_node = node3;
	}

	TERMINALS_CHECKER(CLOSE_FIGURE);

#undef TERMINAL_CHECKER

	return node = _IF(node, node2);
}

Node_t* GetStatement(Lexer* lexer, size_t* pc) {

#ifdef PRINT_DEBUG
	printf("Statement (%zu): ", *pc);
	PrintTokenValueGrammar(&lexer->tokens[*pc]);
#endif

	Node_t* node = NULL;

	if ((node = GetAssignment(lexer, pc)) != NULL)
		return node;

	if ((node = GetIf(lexer, pc)) != NULL)
		return node;

	return NULL;
}

Node_t* GetFunction(Lexer* lexer, size_t* pc) {

#ifdef PRINT_DEBUG
	printf("Function (%zu): ", *pc);
	PrintTokenValueGrammar(&lexer->tokens[*pc]);
#endif

#define TERMINALS_CHECKER(terminal) {									 \
	if (lexer->tokens[(*pc)++].data.val_key_word != terminal)			\
		LANGUAGE_SYNTAX_ERROR(lexer, pc);								\
}

	if (lexer->tokens[(*pc)++].data.val_key_word != INIT_TYPE)
		return NULL;

	Node_t* node = GetIdentifier(lexer, pc);
	if (!node)
		LANGUAGE_SYNTAX_ERROR(lexer, pc);
	node = _FUNC_DEF(node->data.val_func_def, node, NULL);

	TERMINALS_CHECKER(OPEN_ROUND);

	Node_t* node = GetStatement(lexer, pc);
	if (!node)
		LANGUAGE_SYNTAX_ERROR(lexer, pc);

	if (lexer->tokens[(*pc)++].data.val_key_word != SEQUENTIAL_OP)
		LANGUAGE_SYNTAX_ERROR(lexer, pc);

	node = _SEQUENTIAL_OP(node, NULL);

	Node_t* prev_node = node;
	while ((*pc) != lexer->size) {
		Node_t* node2 = GetStatement(lexer, pc);
		if (!node2)
			LANGUAGE_SYNTAX_ERROR(lexer, pc);

		if (lexer->tokens[(*pc)++].data.val_key_word != SEQUENTIAL_OP)
			LANGUAGE_SYNTAX_ERROR(lexer, pc);

		node2 = _SEQUENTIAL_OP(node2, NULL);
		prev_node->right = node2;
		node2->parent = prev_node;
		prev_node = node2;
	}

#undef TERMINALS_CHECKER

	return NULL;
}

Node_t* GetGrammar(Lexer* lexer, size_t* pc) {

#ifdef PRINT_DEBUG
	printf("Grammar (%zu): ", *pc);
	PrintTokenValueGrammar(&lexer->tokens[*pc]);
#endif

	Node_t* node = GetFunction(lexer, pc);
	if (!node)
		LANGUAGE_SYNTAX_ERROR(lexer, pc);

	if (lexer->tokens[(*pc)++].data.val_key_word != SEQUENTIAL_OP)
		LANGUAGE_SYNTAX_ERROR(lexer, pc);

	node = _SEQUENTIAL_OP(node, NULL);

	Node_t* prev_node = node;
	while ((*pc) != lexer->size) {
		Node_t* node2 = GetStatement(lexer, pc);
		if (!node2)
			LANGUAGE_SYNTAX_ERROR(lexer, pc);

		if (lexer->tokens[(*pc)++].data.val_key_word != SEQUENTIAL_OP)
			LANGUAGE_SYNTAX_ERROR(lexer, pc);

		node2 = _SEQUENTIAL_OP(node2, NULL);
		prev_node->right = node2;
		node2->parent = prev_node;
		prev_node = node2;
	}

	if ((*pc)++ != lexer->size)
		LANGUAGE_SYNTAX_ERROR(lexer, pc);

	return node;
}

BinaryTreeStatusCode CreateTreeFromFile(Tree* tree, IdNameTable* id_name_table, Lexer* lexer) {

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
	lexer->buffer_size = size;
	LEXICAL_ANALYSIS(buffer, lexer, id_name_table, size);

	size_t pc = 0;
	tree->root = GetGrammar(lexer, &pc);
	if (!tree->root)
		TREE_ERROR_CHECK(TREE_LANGUAGE_SYNTAX_ERROR);

	BINARY_TREE_GRAPH_DUMP(tree, "ReadProgram", NULL, id_name_table);

	return TREE_NO_ERROR;
}

Node_t* LanguageSyntaxError(Lexer* lexer, size_t* pc) {

	if (*pc > lexer->size) {
		for (size_t i = 0; i < lexer->buffer_size - 1; i++)
			printf("%c", lexer->buffer[i]);
		printf(" " RED("<---") "\n");
		if (fclose(stdout)) {}
		return NULL;
	}

	int new_row_flag = 0;
	int seq_op_flag = 0;
	size_t j = 0;
	int k = 2;
	if ((*pc) > 2) {
		while (lexer->tokens[(*pc) - 2].index + j != lexer->tokens[(*pc)].index) {
			if (lexer->buffer[lexer->tokens[(*pc) - 2].index + j] == '\n') {
				new_row_flag = 1;
				break;
			}
			j++;
		}
		while (k > 0) {
			if (lexer->tokens[((int)(*pc) - k)].data.val_key_word == SEQUENTIAL_OP) {
				seq_op_flag = 1;
				break;
			}
			k--;
		}
	}

	if (new_row_flag && !seq_op_flag) {
		for (size_t i = 0; i < lexer->buffer_size; i++) {

			if (lexer->tokens[*pc - 2].index == i) {
				while (lexer->buffer[i] != '\n')
					printf("%c", lexer->buffer[i++]);
				printf(" " RED("<---") "\n");
				continue;
			}

			printf("%c", lexer->buffer[i]);
		}

		if (fclose(stdout)) {}

		return NULL;
	}

	for (size_t i = 0; i < lexer->buffer_size; i++) {

		if (lexer->tokens[*pc].index == i) {
			while (lexer->buffer[i] != '\n')
				printf("%c", lexer->buffer[i++]);
			printf(" " RED("<---") "\n");
			continue;
		}

		printf("%c", lexer->buffer[i]);
	}

	if (fclose(stdout)) {}

	return NULL;
}
