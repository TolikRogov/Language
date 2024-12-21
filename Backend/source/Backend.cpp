#include "Backend.hpp"

#define MAIN 					L"Путник"
#define STACK_FRAME_REGISTER 	"BX"
#define RETURN_VALUE_REGISTER 	"AX"

static BinaryTreeStatusCode PullComparisons(Node_t* node, Backend* backend);
static BinaryTreeStatusCode PullIf(Node_t* node, Backend* backend);
static BinaryTreeStatusCode PullWhile(Node_t* node, Backend* backend);
static BinaryTreeStatusCode PullDefinitionParameters(Node_t* node, Backend* backend);
static BinaryTreeStatusCode PullFunctionDefinition(Node_t* node, Backend* backend);
static BinaryTreeStatusCode PullMathFunctions(Node_t* node, Backend* backend);
static BinaryTreeStatusCode PullAloneFunctions(Node_t* node, Backend* backend);
static BinaryTreeStatusCode PullReturn(Node_t* node, Backend* backend);
static BinaryTreeStatusCode PullParameters(Node_t* node, Backend* backend);
static BinaryTreeStatusCode PullCommaOp(Node_t* node, Backend* backend);
static BinaryTreeStatusCode PullCallParameters(Node_t* node, Backend* backend);
static BinaryTreeStatusCode PullCall(Node_t* node, Backend* backend);
static BinaryTreeStatusCode PullIdentifier(Node_t* node, Backend* backend);
static BinaryTreeStatusCode PullNumber(Node_t* node, Backend* backend);
static BinaryTreeStatusCode PullSequentialOp(Node_t* node, Backend* backend);
static BinaryTreeStatusCode PullAssignment(Node_t* node, Backend* backend);
static BinaryTreeStatusCode PullInput(Node_t* node, Backend* backend);
static BinaryTreeStatusCode PullPrintf(Node_t* node, Backend* backend);

BinaryTreeStatusCode RunBackend(Tree* tree, IdNameTable* id_name_table) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	FILE* asm_file = fopen(ASM_FILE_, "w");
	if (!asm_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

	Backend backend = { .asm_file = asm_file, .cnt_if = 0, .cnt_while = 0, .id_name_table = id_name_table, .tabs = 0, .cur_scope = -1 };

	tree_status = WriteAssembleCode(tree->root, &backend);
	TREE_ERROR_CHECK(tree_status);

	if (fclose(asm_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	return TREE_NO_ERROR;
}

const char* GetCommentByKeyWordType(KeyWordNum key_word_num) {
	switch (key_word_num) {
		case INPUT: 	return "input";
		case PRINTF:	return "printf";
		case ADD:		return "add";
		case SUB:		return "sub";
		case MUL:		return "mul";
		case SQRT:		return "sqrt";
		case DIV:		return "div";
		case SIN:		return "sin";
		case RETURN:	return "return";
		default:		return NULL;
	}
}

Commands GetCmdByKeyWordType(KeyWordNum key_word_num) {
	switch (key_word_num) {
		case ABOVE: 	return CMD_JBE;
		case EQUAL:		return CMD_JNE;
		case INPUT: 	return CMD_IN;
		case PRINTF:	return CMD_OUT;
		case ADD:		return CMD_ADD;
		case SUB:		return CMD_SUB;
		case MUL:		return CMD_MUL;
		case SQRT:		return CMD_SQRT;
		case DIV:		return CMD_DIV;
		case SIN:		return CMD_SIN;
		case RETURN:	return CMD_RET;
		default:		return COUNT_OF_COMMANDS;
	}
}

BinaryTreeStatusCode PullComparisons(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	WriteAssembleCode(node->left, backend);
	WriteAssembleCode(node->right, backend);
	TABS fprintf(backend->asm_file, "%s ", array_commands[GetCmdByKeyWordType(node->data.val_key_word)].cmd_name);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullIf(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	(backend->cnt_if)++;
	size_t old_num = (backend->cnt_if);

	TABS fprintf(backend->asm_file, "#if-condition\n");

		WriteAssembleCode(node->left, backend);

	fprintf(backend->asm_file, "end_if%zu:\n", (backend->cnt_if));

	(backend->tabs)++;

		TABS fprintf(backend->asm_file, "#if-body\n");
		WriteAssembleCode(node->right, backend);

	(backend->tabs)--;

	TABS fprintf(backend->asm_file, "end_if%zu:\n", old_num);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullWhile(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	(backend->cnt_while)++;
	size_t old_num = (backend->cnt_while);

	TABS fprintf(backend->asm_file, "while%zu:\n", (backend->cnt_while));

		(backend->tabs)++;
		TABS fprintf(backend->asm_file, "#while-condition\n");
		WriteAssembleCode(node->left,  backend);

		fprintf(backend->asm_file,      "end_while%zu:\n", (backend->cnt_while));
		TABS fprintf(backend->asm_file, "#while-body\n");
		WriteAssembleCode(node->right, backend);

		TABS fprintf(backend->asm_file, "%s while%zu:\n", array_commands[CMD_JMP].cmd_name, old_num);
		(backend->tabs)--;

	TABS fprintf(backend->asm_file, "end_while%zu:\n", old_num);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullDefinitionParameters(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	int cur_parameter = 0;
	Node_t* cur_node = node;
	while (cur_node->left) {
		if (!cur_node->left->right)
			break;

		TABS fprintf(backend->asm_file, "%s [%s+%d]\n", array_commands[CMD_POP].cmd_name,
					STACK_FRAME_REGISTER, cur_parameter);

		cur_parameter++;
		cur_node = cur_node->left;
	}

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullFunctionDefinition(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }
#define ASM_PRINTF(...) fprintf(backend->asm_file, __VA_ARGS__);

	Identifier* cur_func = &backend->id_name_table->data[node->data.val_func_def];
	int old_scope = backend->cur_scope;
	backend->cur_scope = (int)node->data.val_func_def;

	if (StrNCmp(MAIN, cur_func->string, StrLen(MAIN), (int)cur_func->length) == 0) {
		ASM_PRINTF("#bx - amount of global variables default\n");
			ASM_PRINTF("%s %d\n", 	array_commands[CMD_PUSH].cmd_name, 	CountOfGlobalVariables(backend->id_name_table));
			ASM_PRINTF("%s %s\n", 	array_commands[CMD_POP].cmd_name, 	STACK_FRAME_REGISTER);
			ASM_PRINTF("%s %s\n\n", array_commands[CMD_PUSH].cmd_name, 	STACK_FRAME_REGISTER);

		ASM_PRINTF("#main\n");
			ASM_PRINTF("%s %ls:\n", array_commands[CMD_CALL].cmd_name, MAIN);
			ASM_PRINTF("%s\n\n", 	array_commands[CMD_HLT].cmd_name);
	}

	TABS PrintNString(backend->asm_file, cur_func->string, cur_func->length);
	ASM_PRINTF(":\n");

	(backend->tabs)++;
	WriteAssembleCode(node->right, backend);
	(backend->tabs)--;
	backend->cur_scope = old_scope;

#undef ASM_PRINTF
#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullMathFunctions(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	TABS fprintf(backend->asm_file, "#%s\n", GetCommentByKeyWordType(node->data.val_key_word));
		WriteAssembleCode(node->left, backend);
		WriteAssembleCode(node->right, backend);
	TABS fprintf(backend->asm_file, "%s\n", array_commands[GetCmdByKeyWordType(node->data.val_key_word)].cmd_name);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullAloneFunctions(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	TABS fprintf(backend->asm_file, "#%s\n", GetCommentByKeyWordType(node->data.val_key_word));
		WriteAssembleCode(node->right, backend);
	TABS fprintf(backend->asm_file, "%s\n", array_commands[GetCmdByKeyWordType(node->data.val_key_word)].cmd_name);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullReturn(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	TABS fprintf(backend->asm_file, "#%s\n", GetCommentByKeyWordType(node->data.val_key_word));
		WriteAssembleCode(node->right, backend);

	TABS fprintf(backend->asm_file, "\n#write return value to register\n");
	TABS fprintf(backend->asm_file, "%s %s\n\n", array_commands[CMD_POP].cmd_name, RETURN_VALUE_REGISTER);

	TABS fprintf(backend->asm_file, "%s\n", array_commands[GetCmdByKeyWordType(node->data.val_key_word)].cmd_name);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullParameters(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	PullDefinitionParameters(node, backend);
	WriteAssembleCode(node->right, backend);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullCommaOp(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	if (node->left) PullCommaOp(node->left, backend);
	WriteAssembleCode(node->right, backend);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullCallParameters(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }
#define ASM_PRINTF(...) fprintf(backend->asm_file, __VA_ARGS__);

	Identifier* cur_scope = &backend->id_name_table->data[backend->cur_scope];

	TABS ASM_PRINTF("#push previous value of stack frame register before new call\n");
		TABS ASM_PRINTF("%s %s\n\n", array_commands[CMD_PUSH].cmd_name, STACK_FRAME_REGISTER);

	PullCommaOp(node->left->left, backend);

	TABS ASM_PRINTF("#stack frame register change value\n");
		TABS ASM_PRINTF("%s %s\n", 		array_commands[CMD_PUSH].cmd_name, 	STACK_FRAME_REGISTER);
		TABS ASM_PRINTF("%s %zu\n", 	array_commands[CMD_PUSH].cmd_name, 	cur_scope->scope_variables.size);
		TABS ASM_PRINTF("%s\n", 		array_commands[CMD_ADD].cmd_name);
		TABS ASM_PRINTF("%s %s\n\n",	array_commands[CMD_POP].cmd_name, 	STACK_FRAME_REGISTER);

#undef ASM_PRINTF
#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullCall(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }
#define ASM_PRINTF(...) fprintf(backend->asm_file, __VA_ARGS__);

	Identifier* cur_id = &backend->id_name_table->data[node->right->data.val_id];

	PullCallParameters(node, backend);

	TABS ASM_PRINTF("#call function\n");
		TABS ASM_PRINTF("%s ", array_commands[CMD_CALL].cmd_name);
		PrintNString(backend->asm_file, cur_id->string, cur_id->length);
		fprintf(backend->asm_file, ":\n");

	TABS ASM_PRINTF("#Pull back stack frame register value as before calling function\n");
		TABS ASM_PRINTF("%s %s\n\n", array_commands[CMD_POP].cmd_name, STACK_FRAME_REGISTER);

	TABS ASM_PRINTF("#push return value of function\n");
		TABS ASM_PRINTF("%s %s\n", array_commands[CMD_PUSH].cmd_name, RETURN_VALUE_REGISTER);

#undef ASM_PRINTF
#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullIdentifier(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	Identifier* cur_id = &backend->id_name_table->data[node->data.val_id];

	if (backend->cur_scope == -1 || cur_id->global == 1) {
		TABS fprintf(backend->asm_file, "%s [%d] #", array_commands[CMD_PUSH].cmd_name, cur_id->num);
	}
	else {
		TABS fprintf(backend->asm_file, "%s [%s+%d] #", array_commands[CMD_PUSH].cmd_name, STACK_FRAME_REGISTER,
		GetLocalNumberOfVariable(backend->id_name_table, backend->cur_scope, cur_id->num));
	}

	PrintNString(backend->asm_file, cur_id->string, cur_id->length);
	fprintf(backend->asm_file, "\n");

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullNumber(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	TABS fprintf(backend->asm_file, "%s %lg\n", array_commands[CMD_PUSH].cmd_name, node->data.val_num);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullSequentialOp(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	WriteAssembleCode(node->left, backend);
	fprintf(backend->asm_file, "\n");
	WriteAssembleCode(node->right, backend);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullAssignment(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	TABS fprintf(backend->asm_file, "#assignment\n");
	WriteAssembleCode(node->left, backend);

	Identifier* cur_id = &backend->id_name_table->data[node->right->data.val_id];

	if (backend->cur_scope == -1 || cur_id->global == 1) {
		TABS fprintf(backend->asm_file, "%s [%d] #", array_commands[CMD_POP].cmd_name, cur_id->num);
	}
	else {
		TABS fprintf(backend->asm_file, "%s [%s+%d] #", array_commands[CMD_POP].cmd_name, STACK_FRAME_REGISTER,
		GetLocalNumberOfVariable(backend->id_name_table, backend->cur_scope, cur_id->num));
	}

	PrintNString(backend->asm_file, cur_id->string, cur_id->length);
	fprintf(backend->asm_file, "\n");

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullInput(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	TABS fprintf(backend->asm_file, "#input\n");
	TABS fprintf(backend->asm_file, "%s\n", array_commands[CMD_IN].cmd_name);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullPrintf(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	TABS fprintf(backend->asm_file, "#printf\n");
	WriteAssembleCode(node->right, backend);
	TABS fprintf(backend->asm_file, "%s\n", array_commands[CMD_OUT].cmd_name);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode WriteAssembleCode(Node_t* node, Backend* backend) {

	if (!node)
		return TREE_NO_ERROR;

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	switch (node->type) {
		case KEYWORD: {
			switch (node->data.val_key_word) {
				case EQUAL:
				case ABOVE: 		{ PullComparisons(node, backend); 			break; }
				case SEQUENTIAL_OP: { PullSequentialOp(node, backend); 			break; }
				case IF: 			{ PullIf(node, backend); 					break; }
				case WHILE:			{ PullWhile(node, backend); 				break; }
				case ASSIGNMENT: 	{ PullAssignment(node, backend); 			break; }
				case INPUT: 		{ PullInput(node, backend); 				break; }
				case PRINTF: 		{ PullPrintf(node, backend); 				break; }
				case SUB:
				case MUL:
				case SQRT:
				case DIV:
				case ADD: 			{ PullMathFunctions(node, backend); 		break; }
				case RETURN:		{ PullReturn(node, backend); 				break; }
				case COS:
				case SIN: 			{ PullAloneFunctions(node, backend); 		break; }
				default: return TREE_NO_ERROR;
			}
			break;
		}
		case FUNCTION_DEFINITION: 	{ PullFunctionDefinition(node, backend);	 break; }
		case VAR_DECLARATION: 		{ WriteAssembleCode(node->right, backend); 	break; }
		case NUMBER: 		 		{ PullNumber(node, backend); 				break; }
		case IDENTIFIER: 			{ PullIdentifier(node, backend); 			 break; }
		case CALL: 					{ PullCall(node, backend); 					break; }
		case PARAMETERS:			{ PullParameters(node, backend);			break; }
		default: return TREE_NO_ERROR;
	}

#undef TABS

	return TREE_NO_ERROR;
}

