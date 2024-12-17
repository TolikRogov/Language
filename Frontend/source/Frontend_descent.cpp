#include "Frontend_descent.hpp"
#include "Frontend_standard.hpp"
#include "Tree_dump.hpp"
#include "Tree_dump.hpp"

Node_t* GetGrammar(FrontedDescent* descent);
Node_t* GetFunctionDefinition(FrontedDescent* descent);
Node_t* GetDefinitionParameters(FrontedDescent* descent);
Node_t* GetOperator(FrontedDescent* descent);
Node_t* GetStatement(FrontedDescent* descent);
Node_t* GetReturn(FrontedDescent* descent);
Node_t* GetVariableDeclaration(FrontedDescent* descent);
Node_t* GetAssignment(FrontedDescent* descent);
Node_t* GetExpression(FrontedDescent* descent);
Node_t* GetTerminator(FrontedDescent* descent);
Node_t* GetPriority(FrontedDescent* descent);
Node_t* GetPower(FrontedDescent* descent);
Node_t* GetCallParameters(FrontedDescent* descent);
Node_t* GetCall(FrontedDescent* descent);
Node_t* GetOperation(FrontedDescent* descent);
Node_t* GetIdVariable(FrontedDescent* descent);
Node_t* GetIdFunction(FrontedDescent* descent);
Node_t* GetNumber(FrontedDescent* descent);

Node_t* GetNumber(FrontedDescent* descent) {

#ifdef PRINT_DEBUG
	printf("Number (%zu): ", *(descent->pc));
	PrintTokenValue(&descent->lexer->tokens[*(descent->pc)], descent->id_name_table);
#endif

	if (descent->lexer->tokens[*(descent->pc)].type == NUMBER)
		return _NUM(descent->lexer->tokens[(*(descent->pc))++].data.val_num);
	else
		return NULL;

}

Node_t* GetIdVariable(FrontedDescent* descent) {

#ifdef PRINT_DEBUG
	printf("IdVariable (%zu): ", *(descent->pc));
	PrintTokenValue(&descent->lexer->tokens[*(descent->pc)], descent->id_name_table);
#endif

	if (descent->lexer->tokens[*(descent->pc)].type != IDENTIFIER)
		return NULL;

	Identifier* cur_id = &descent->id_name_table->data[descent->lexer->tokens[(*(descent->pc))].data.val_id];
	Identifier* cur_scope = &descent->id_name_table->data[descent->cur_scope];

	if (descent->lexer->tokens[*(descent->pc) + 1].type == KEYWORD && descent->lexer->tokens[(*(descent->pc)) + 1].data.val_key_word == OPEN_ROUND)
		return NULL;

#ifdef PRINT_DEBUG
	printf(YELLOW("current scope: "));
	if (descent->cur_scope < 0)
		printf("global");
	else
		PrintNString(stdout, cur_scope->string, cur_scope->length);
	printf("\n");
#endif

	if ((descent->cur_scope) < 0) {
		cur_id->global = 1;
		return _ID(descent->lexer->tokens[(*(descent->pc))++].data.val_id);
	}

	if (descent->lexer->tokens[*(descent->pc) - 1].type == KEYWORD && descent->lexer->tokens[(*(descent->pc)) - 1].data.val_key_word == INIT_TYPE) {
		if (!FindLocalVariableInScope(cur_scope, cur_id) && cur_id->global == 0) {
			cur_id->define_status = 1;

			ScopeLocalVariablesRealloc(cur_scope);
			cur_scope->scope_variables.data[cur_scope->scope_variables.size] = cur_id->num;
			cur_scope->scope_variables.size++;
		}
		else {
			printf(RED("ERROR: using of redefined variable!")"\n");
			LANGUAGE_SYNTAX_ERROR(descent);
		}
	}

	if (!FindLocalVariableInScope(cur_scope, cur_id) && cur_id->global == 0) {
		printf(RED("ERROR: using of undefined variable!")"\n");
		LANGUAGE_SYNTAX_ERROR(descent);
	}

	return _ID(descent->lexer->tokens[(*(descent->pc))++].data.val_id);
}

Node_t* GetIdFunction(FrontedDescent* descent) {

#ifdef PRINT_DEBUG
	printf("IdFunction (%zu): ", *(descent->pc));
	PrintTokenValue(&descent->lexer->tokens[*(descent->pc)], descent->id_name_table);
#endif

	if (descent->lexer->tokens[*(descent->pc)].type != IDENTIFIER)
		return NULL;

	Identifier* cur_id = &descent->id_name_table->data[descent->lexer->tokens[(*(descent->pc))].data.val_id];

	if (descent->lexer->tokens[*(descent->pc) + 1].type == KEYWORD && descent->lexer->tokens[(*(descent->pc)) + 1].data.val_key_word != OPEN_ROUND)
		return NULL;
	cur_id->type = ID_FUNCTION;

	if (descent->lexer->tokens[*(descent->pc) - 1].type == KEYWORD && descent->lexer->tokens[(*(descent->pc)) - 1].data.val_key_word != INIT_TYPE)
		return _ID(descent->lexer->tokens[(*(descent->pc))++].data.val_id);

	switch (cur_id->define_status) {
		case 0: {
			cur_id->define_status = 1;
			descent->cur_scope = cur_id->num;

			cur_id->scope_variables.capacity = DEFAULT_COUNT_LOCAL_VARIABLES;
			cur_id->scope_variables.data = (int*)calloc(cur_id->scope_variables.capacity, sizeof(int));
			if (!cur_id->scope_variables.data) {
				TREE_ERROR_MESSAGE(TREE_ALLOC_ERROR);
				return NULL;
			}
			break;
		}
		default: {
			printf(RED("ERROR: using of redefined function!")"\n");
		 	LANGUAGE_SYNTAX_ERROR(descent);
		}
	}

	return _ID(descent->lexer->tokens[(*(descent->pc))++].data.val_id);
}

Node_t* GetOperation(FrontedDescent* descent) {

#ifdef PRINT_DEBUG
	printf("Operation (%zu): ", *(descent->pc));
	PrintTokenValue(&descent->lexer->tokens[*(descent->pc)], descent->id_name_table);
#endif

	Node_t* node = NULL;

	if (descent->lexer->tokens[*(descent->pc)].type != KEYWORD)
		return NULL;

	KeyWordNum key_word = descent->lexer->tokens[(*(descent->pc))++].data.val_key_word;

	if (descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[*(descent->pc)].data.val_key_word == OPEN_ROUND) {
		(*(descent->pc))++;
		node = GetExpression(descent);
		if (!node)
			LANGUAGE_SYNTAX_ERROR(descent);

		if ((descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[*(descent->pc)].data.val_key_word != CLOSE_ROUND) ||
			 descent->lexer->tokens[*(descent->pc)].type != KEYWORD)
			LANGUAGE_SYNTAX_ERROR(descent);

		(*(descent->pc))++;
		switch (key_word) {
			case SIN:  	return node = _SIN(NULL, node);
			case COS: 	return node = _COS(NULL, node);
			case SQRT: 	return node = _SQRT(NULL, node);
			case FLOOR: return node = _FLOOR(NULL, node);
			default:   return NULL;
		}
	}

	return node;
}

Node_t* GetCallParameters(FrontedDescent* descent) {

#ifdef PRINT_DEBUG
	printf("CallParameters (%zu): ", *(descent->pc));
	PrintTokenValue(&descent->lexer->tokens[*(descent->pc)], descent->id_name_table);
#endif

#define TERMINALS_CHECKER(terminal) {											 	 														 \
	if (descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[(*(descent->pc))++].data.val_key_word != terminal)	\
		LANGUAGE_SYNTAX_ERROR(descent);																										\
}
	Node_t* node = GetExpression(descent);
	if (!node)
		return _COMMA_OP(NULL, NULL);
	node = _COMMA_OP(node, NULL);

	Node_t* prev_node = node;
	while (descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[(*(descent->pc))].data.val_key_word != CLOSE_ROUND) {
		TERMINALS_CHECKER(COMMA_OP);
		Node_t* node2 = GetExpression(descent);
		if (!node2)
			LANGUAGE_SYNTAX_ERROR(descent);

		node2 = _COMMA_OP(node2, NULL);
		prev_node->right = node2;
		node2->parent =  prev_node;
		prev_node = node2;
	}

#undef TERMINALS_CHECKER

	return node;
}

Node_t* GetCall(FrontedDescent* descent) {

#ifdef PRINT_DEBUG
	printf("Call (%zu): ", *(descent->pc));
	PrintTokenValue(&descent->lexer->tokens[*(descent->pc)], descent->id_name_table);
#endif

#define TERMINALS_CHECKER(terminal) {									 			 														 \
	if (descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[(*(descent->pc))++].data.val_key_word != terminal)	\
		LANGUAGE_SYNTAX_ERROR(descent);																										\
}

	Node_t* node = GetIdFunction(descent);
	if (!node)
		return NULL;

	TERMINALS_CHECKER(OPEN_ROUND);

	Node_t* node2 = GetCallParameters(descent);
	if (!node2)
		LANGUAGE_SYNTAX_ERROR(descent);
	node2 = _PARAMETERS(node2, NULL);

	TERMINALS_CHECKER(CLOSE_ROUND);

#undef TERMINALS_CHECKER

	return node = _CALL(node2, node);
}

Node_t* GetPower(FrontedDescent* descent) {

#ifdef PRINT_DEBUG
	printf("Power (%zu): ", *(descent->pc));
	PrintTokenValue(&descent->lexer->tokens[*(descent->pc)], descent->id_name_table);
#endif

	Node_t* node1 = GetPriority(descent);
	if (!node1)
		LANGUAGE_SYNTAX_ERROR(descent);

	while (descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[*(descent->pc)].data.val_key_word == POW) {
		(*(descent->pc))++;
		Node_t* node2 = GetPriority(descent);
		if (!node2)
			LANGUAGE_SYNTAX_ERROR(descent);

		node1 = _POW(node1, node2);
	}

	return node1;
}

Node_t* GetPriority(FrontedDescent* descent) {

#ifdef PRINT_DEBUG
	printf("Priority (%zu): ", *(descent->pc));
	PrintTokenValue(&descent->lexer->tokens[*(descent->pc)], descent->id_name_table);
#endif

	Node_t* node = NULL;

	if (descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[*(descent->pc)].data.val_key_word == OPEN_ROUND) {
		(*(descent->pc))++;
		node = GetExpression(descent);
		if (!node)
			LANGUAGE_SYNTAX_ERROR(descent);

		if ((descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[*(descent->pc)].data.val_key_word != CLOSE_ROUND) ||
			 descent->lexer->tokens[*(descent->pc)].type != KEYWORD)
			LANGUAGE_SYNTAX_ERROR(descent);

		(*(descent->pc))++;
		return node;
	}

	if ((node = GetNumber(descent)) != NULL)
		return node;

	if ((node = GetIdVariable(descent)) != NULL)
		return node;

	if ((node = GetOperation(descent)) != NULL)
		return node;

	if ((node = GetCall(descent)) != NULL)
		return node;

	return NULL;
}

Node_t* GetTerminator(FrontedDescent* descent) {

#ifdef PRINT_DEBUG
	printf("Terminator (%zu): ", *(descent->pc));
	PrintTokenValue(&descent->lexer->tokens[*(descent->pc)], descent->id_name_table);
#endif

	Node_t* node1 = GetPower(descent);
	if (!node1)
		LANGUAGE_SYNTAX_ERROR(descent);

	while (descent->lexer->tokens[*(descent->pc)].type == KEYWORD &&
		   (descent->lexer->tokens[*(descent->pc)].data.val_key_word == MUL || descent->lexer->tokens[*(descent->pc)].data.val_key_word == DIV)) {
		KeyWordNum key_word = descent->lexer->tokens[(*(descent->pc))++].data.val_key_word;
		Node_t* node2 = GetPower(descent);
		if (!node2)
			LANGUAGE_SYNTAX_ERROR(descent);

		if (key_word == MUL)
			node1 = _MUL(node1, node2);
		else
			node1 = _DIV(node1, node2);
	}

	return node1;
}

Node_t* GetExpression(FrontedDescent* descent) {

#ifdef PRINT_DEBUG
	printf("Expression (%zu): ", *(descent->pc));
	PrintTokenValue(&descent->lexer->tokens[*(descent->pc)], descent->id_name_table);
#endif

	Node_t* node1 = GetTerminator(descent);
	if (!node1)
		LANGUAGE_SYNTAX_ERROR(descent);

	while (descent->lexer->tokens[*(descent->pc)].type == KEYWORD &&
		   (descent->lexer->tokens[*(descent->pc)].data.val_key_word == ADD || descent->lexer->tokens[*(descent->pc)].data.val_key_word == SUB)) {
		KeyWordNum key_word = descent->lexer->tokens[(*(descent->pc))++].data.val_key_word;
		Node_t* node2 = GetTerminator(descent);
		if (!node2)
			LANGUAGE_SYNTAX_ERROR(descent);

		if (key_word == ADD)
			node1 = _ADD(node1, node2);
		else
			node1 = _SUB(node1, node2);
	}

	return node1;
}

Node_t* GetAssignment(FrontedDescent* descent) {

#ifdef PRINT_DEBUG
	printf("Assignment (%zu): ", *(descent->pc));
	PrintTokenValue(&descent->lexer->tokens[*(descent->pc)], descent->id_name_table);
#endif

	Node_t* node = GetIdVariable(descent);
	if (!node)
		return NULL;

	if ((descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[(*(descent->pc))].data.val_key_word != ASSIGNMENT)
		|| descent->lexer->tokens[*(descent->pc)].type != KEYWORD)
		LANGUAGE_SYNTAX_ERROR(descent);

	(*descent->pc)++;
	Node_t* node2 = GetExpression(descent);
	if (!node2)
		LANGUAGE_SYNTAX_ERROR(descent);

	return node = _ASSIGNMENT(node, node2);
}

Node_t* GetVariableDeclaration(FrontedDescent* descent) {

#ifdef PRINT_DEBUG
	printf("VarDeclaration (%zu): ", *(descent->pc));
	PrintTokenValue(&descent->lexer->tokens[*(descent->pc)], descent->id_name_table);
#endif

	if ((descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[(*(descent->pc))].data.val_key_word != INIT_TYPE) ||
		 descent->lexer->tokens[*(descent->pc)].type != KEYWORD)
		return NULL;

	(*(descent->pc))++;
	Node_t* node = GetIdVariable(descent);
	if (!node)
		return NULL;

	if (descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[(*(descent->pc))].data.val_key_word != ASSIGNMENT)
		return node = _VAR_DEC(node->data.val_id, _INIT_TYPE(), node);

	(*(descent->pc))++;
	Node_t* node2 = GetExpression(descent);
	if (!node2)
		LANGUAGE_SYNTAX_ERROR(descent);

	return node = _VAR_DEC(node->data.val_id, _INIT_TYPE(), _ASSIGNMENT(node, node2));
}

Node_t* GetStatement(FrontedDescent* descent) {

#ifdef PRINT_DEBUG
	printf("Statement (%zu): ", *(descent->pc));
	PrintTokenValue(&descent->lexer->tokens[*(descent->pc)], descent->id_name_table);
#endif

#define TERMINALS_CHECKER(terminal) {									 			 														 \
	if (descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[(*(descent->pc))++].data.val_key_word != terminal)	\
		LANGUAGE_SYNTAX_ERROR(descent);																										\
}

	if ((descent->lexer->tokens[*(descent->pc)].type == KEYWORD &&
		(descent->lexer->tokens[*(descent->pc)].data.val_key_word != IF && descent->lexer->tokens[*(descent->pc)].data.val_key_word != WHILE)) ||
		descent->lexer->tokens[*(descent->pc)].type != KEYWORD)
		return NULL;

	KeyWordNum key_word = descent->lexer->tokens[(*(descent->pc))].data.val_key_word;
	(*(descent->pc))++;

	TERMINALS_CHECKER(OPEN_ROUND);
	Node_t* node = GetExpression(descent);
	if (!node)
		LANGUAGE_SYNTAX_ERROR(descent);
	TERMINALS_CHECKER(CLOSE_ROUND);

	TERMINALS_CHECKER(OPEN_FIGURE);
	Node_t* node2 = GetOperator(descent);
	if (!node2)
		LANGUAGE_SYNTAX_ERROR(descent);

	TERMINALS_CHECKER(SEQUENTIAL_OP)

	node2 = _SEQUENTIAL_OP(node2, NULL);

	Node_t* prev_node = node2;
	while ((descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[(*(descent->pc))].data.val_key_word != CLOSE_FIGURE)
		   || descent->lexer->tokens[*(descent->pc)].type != KEYWORD) {
		Node_t* node3 = GetOperator(descent);
		if (!node3)
			LANGUAGE_SYNTAX_ERROR(descent);

		TERMINALS_CHECKER(SEQUENTIAL_OP);

		node3 = _SEQUENTIAL_OP(node3, NULL);
		prev_node->right = node3;
		node3->parent = prev_node;
		prev_node = node3;
	}

	TERMINALS_CHECKER(CLOSE_FIGURE);

#undef TERMINAL_CHECKER

	if (key_word == IF)
		return node = _IF(node, node2);

	if (key_word == WHILE)
		return node = _WHILE(node, node2);

	return NULL;
}

Node_t* GetReturn(FrontedDescent* descent) {

#ifdef PRINT_DEBUG
	printf("Return (%zu): ", *(descent->pc));
	PrintTokenValue(&descent->lexer->tokens[*(descent->pc)], descent->id_name_table);
#endif

	if ((descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[(*(descent->pc))].data.val_key_word != RETURN) ||
		descent->lexer->tokens[*(descent->pc)].type != KEYWORD)
		return NULL;

	(*(descent->pc))++;
	Node_t* node = GetExpression(descent);
	if (!node)
		LANGUAGE_SYNTAX_ERROR(descent);

	return node = _RETURN(NULL, node);
}

Node_t* GetOperator(FrontedDescent* descent) {

#ifdef PRINT_DEBUG
	printf("Operator (%zu): ", *(descent->pc));
	PrintTokenValue(&descent->lexer->tokens[*(descent->pc)], descent->id_name_table);
#endif

	Node_t* node = NULL;

	if ((node = GetAssignment(descent)) != NULL)
		return node;

	if ((node = GetStatement(descent)) != NULL)
		return node;

	if ((node = GetVariableDeclaration(descent)) != NULL)
		return node;

	if ((node = GetReturn(descent)) != NULL)
		return node;

	return NULL;
}

Node_t* GetDefinitionParameters(FrontedDescent* descent) {

#ifdef PRINT_DEBUG
	printf("DefinitionParameters (%zu): ", *(descent->pc));
	PrintTokenValue(&descent->lexer->tokens[*(descent->pc)], descent->id_name_table);
#endif

#define TERMINALS_CHECKER(terminal) {									 		 	 															 \
	if (descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[(*(descent->pc))++].data.val_key_word != terminal) 	\
		LANGUAGE_SYNTAX_ERROR(descent);																											\
}
	if ((descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[(*(descent->pc))].data.val_key_word != INIT_TYPE) ||
		descent->lexer->tokens[*(descent->pc)].type != KEYWORD)
		return _COMMA_OP(NULL, NULL);

	(*(descent->pc))++;
	Node_t* node = GetIdVariable(descent);
	if (!node)
		LANGUAGE_SYNTAX_ERROR(descent);
	node = _COMMA_OP(_VAR_DEC(node->data.val_id, _INIT_TYPE(), node), NULL);

	Node_t* prev_node = node;
	while ((descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[(*(descent->pc))].data.val_key_word != CLOSE_ROUND) ||
			descent->lexer->tokens[*(descent->pc)].type != KEYWORD) {
		TERMINALS_CHECKER(COMMA_OP);
		TERMINALS_CHECKER(INIT_TYPE);
		Node_t* node2 = GetIdVariable(descent);
		if (!node2)
			LANGUAGE_SYNTAX_ERROR(descent);

		node2 = _COMMA_OP(_VAR_DEC(node2->data.val_id, _INIT_TYPE(), node2), NULL);
		prev_node->right = node2;
		node2->parent = prev_node;
		prev_node = node2;
	}

#undef TERMINALS_CHECKER

	return node;
}

Node_t* GetFunctionDefinition(FrontedDescent* descent) {

#ifdef PRINT_DEBUG
	printf("FunctionDefinition (%zu): ", *(descent->pc));
	PrintTokenValue(&descent->lexer->tokens[*(descent->pc)], descent->id_name_table);
#endif

#define TERMINALS_CHECKER(terminal) {									 			 														 \
	if (descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[(*(descent->pc))++].data.val_key_word != terminal)	\
		LANGUAGE_SYNTAX_ERROR(descent);																										\
}
	if ((descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[(*(descent->pc))].data.val_key_word != INIT_TYPE) ||
		descent->lexer->tokens[*(descent->pc)].type != KEYWORD)
		return NULL;

	(*descent->pc)++;
	Node_t* node = GetIdFunction(descent);
	if (!node)
		LANGUAGE_SYNTAX_ERROR(descent);

	node = _FUNC_DEF(node->data.val_id, _INIT_TYPE(), NULL);

	TERMINALS_CHECKER(OPEN_ROUND);

	Node_t* node2 = GetDefinitionParameters(descent);
	if (!node)
		LANGUAGE_SYNTAX_ERROR(descent);
	node2 = _PARAMETERS(node2, NULL);
	node->right = node2;
	node2->parent = node;

	TERMINALS_CHECKER(CLOSE_ROUND);
	TERMINALS_CHECKER(OPEN_FIGURE);

	Node_t* node3 = GetOperator(descent);
	if (!node3)
		LANGUAGE_SYNTAX_ERROR(descent);

	if ((descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[(*(descent->pc))].data.val_key_word != SEQUENTIAL_OP) ||
		descent->lexer->tokens[*(descent->pc)].type != KEYWORD)
		LANGUAGE_SYNTAX_ERROR(descent);

	(*descent->pc)++;
	node3 = _SEQUENTIAL_OP(node3, NULL);

	Node_t* prev_node = node3;
	while ((descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[(*(descent->pc))].data.val_key_word != CLOSE_FIGURE) ||
			descent->lexer->tokens[*(descent->pc)].type != KEYWORD) {
		Node_t* node4 = GetOperator(descent);
		if (!node4)
			LANGUAGE_SYNTAX_ERROR(descent);

		if ((descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[(*(descent->pc))].data.val_key_word != SEQUENTIAL_OP) ||
			descent->lexer->tokens[*(descent->pc)].type != KEYWORD)
			LANGUAGE_SYNTAX_ERROR(descent);

		(*descent->pc)++;
		node4 = _SEQUENTIAL_OP(node4, NULL);
		prev_node->right = node4;
		node4->parent = prev_node;
		prev_node = node4;
	}

	node2->right = node3;
	node3->parent = node2;

	TERMINALS_CHECKER(CLOSE_FIGURE);

#undef TERMINALS_CHECKER

	return node;
}

Node_t* GetGrammar(FrontedDescent* descent) {

#ifdef PRINT_DEBUG
	printf("Grammar (%zu): ", *(descent->pc));
	PrintTokenValue(&descent->lexer->tokens[*(descent->pc)], descent->id_name_table);
#endif

	Node_t* global_node = NULL;
	while (true) {
		Node_t* prev_global_node = GetVariableDeclaration(descent);
		if (prev_global_node == NULL)
			break;

		if ((descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[(*(descent->pc))].data.val_key_word != SEQUENTIAL_OP) ||
			descent->lexer->tokens[*(descent->pc)].type != KEYWORD)
			LANGUAGE_SYNTAX_ERROR(descent);

		(*descent->pc)++;
		prev_global_node = _SEQUENTIAL_OP(prev_global_node, NULL);
		if (!global_node)
			global_node = prev_global_node;
		else {
			global_node->right = prev_global_node;
			prev_global_node->parent = global_node;
		}

		global_node = prev_global_node;
	}

	(*descent->pc)--;
	Node_t* node = GetFunctionDefinition(descent);
	if (!node)
		LANGUAGE_SYNTAX_ERROR(descent);

	if ((descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[(*(descent->pc))].data.val_key_word != SEQUENTIAL_OP) ||
		descent->lexer->tokens[*(descent->pc)].type != KEYWORD)
		LANGUAGE_SYNTAX_ERROR(descent);

	(*descent->pc)++;
	node = _SEQUENTIAL_OP(node, NULL);

	Node_t* prev_node = node;
	while ((*(descent->pc)) != descent->lexer->size) {
		Node_t* node2 = GetFunctionDefinition(descent);
		if (!node2)
			LANGUAGE_SYNTAX_ERROR(descent);

		if ((descent->lexer->tokens[*(descent->pc)].type == KEYWORD && descent->lexer->tokens[(*(descent->pc))].data.val_key_word != SEQUENTIAL_OP) ||
			descent->lexer->tokens[*(descent->pc)].type != KEYWORD)
			LANGUAGE_SYNTAX_ERROR(descent);

		(*descent->pc)++;
		node2 = _SEQUENTIAL_OP(node2, NULL);
		prev_node->right = node2;
		node2->parent = prev_node;
		prev_node = node2;
	}

	if ((*(descent->pc))++ != descent->lexer->size)
		LANGUAGE_SYNTAX_ERROR(descent);

	if (FindTreeRoot(global_node) != NULL) {
		node->parent = global_node;
		global_node->right = node;
	}

	if (global_node && global_node->right)
		return FindTreeRoot(global_node);

	return node;
}

BinaryTreeStatusCode CreateTreeFromFile(Tree* tree, IdNameTable* id_name_table, Lexer* lexer) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	FILE* prog_file = fopen(PROG_FILE_, "r");
	if (!prog_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

	struct stat file_info = {};
	stat(PROG_FILE_, &file_info);

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
	size_t pc = 0;
	FrontedDescent descent = {.lexer = lexer, .id_name_table = id_name_table, .pc = &pc, .cur_scope = -1};
	LEXICAL_ANALYSIS(buffer, lexer, id_name_table, size);

	tree->root = GetGrammar(&descent);
	if (!tree->root)
		TREE_ERROR_CHECK(TREE_LANGUAGE_SYNTAX_ERROR);

	NodeParentChecker(tree->root);
	NAME_TABLE_PRINT(id_name_table);
	BINARY_TREE_GRAPH_DUMP(tree, "ReadProgram", NULL, id_name_table);

	return TREE_NO_ERROR;
}

Node_t* LanguageSyntaxError(FrontedDescent* descent) {

	printf(RED("Lexer token[%zu]: "), (*(descent->pc)));
	PrintTokenValue(&descent->lexer->tokens[*(descent->pc)], descent->id_name_table);

	if (*(descent->pc) > descent->lexer->size) {
		for (size_t i = 0; i < descent->lexer->buffer_size - 1; i++)
			printf("%c", descent->lexer->buffer[i]);
		printf(" " RED("<---") "\n");
		if (fclose(stdout)) {}
		return NULL;
	}

	int new_row_flag = 0;
	int seq_op_flag = 0;
	size_t j = 0;
	int k = 2;
	if ((*(descent->pc)) > 2) {
		while (descent->lexer->tokens[(*(descent->pc)) - 2].index + j != descent->lexer->tokens[(*(descent->pc))].index) {
			if (descent->lexer->buffer[descent->lexer->tokens[(*(descent->pc)) - 2].index + j] == '\n') {
				new_row_flag = 1;
				break;
			}
			j++;
		}
		while (k > 0) {
			if (descent->lexer->tokens[((int)(*(descent->pc)) - k)].data.val_key_word == SEQUENTIAL_OP) {
				seq_op_flag = 1;
				break;
			}
			k--;
		}
	}

	if (new_row_flag && !seq_op_flag) {
		for (size_t i = 0; i < descent->lexer->buffer_size; i++) {

			if (descent->lexer->tokens[*(descent->pc) - 2].index == i) {
				while (descent->lexer->buffer[i] != '\n')
					printf("%c", descent->lexer->buffer[i++]);
				printf(" " RED("<---") "\n");
				continue;
			}

			printf("%c", descent->lexer->buffer[i]);
		}
		if (fclose(stdout)) {}
		return NULL;
	}

	for (size_t i = 0; i < descent->lexer->buffer_size; i++) {

		if (descent->lexer->tokens[*(descent->pc)].index == i) {
			while (descent->lexer->buffer[i] != '\n')
				printf("%c", descent->lexer->buffer[i++]);
			printf(" " RED("<---") "\n");
			continue;
		}

		printf("%c", descent->lexer->buffer[i]);
	}

	if (fclose(stdout)) {}
	return NULL;
}
