#include "Backend.hpp"

#define MAIN 					L"Путник"
#define STACK_FRAME_REGISTER 	"BX"
#define RETURN_VALUE_REGISTER 	"AX"

BinaryTreeStatusCode RunBackend(Tree* tree, IdNameTable* id_name_table) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	FILE* asm_file = fopen(ASM_FILE_, "w");
	if (!asm_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

	Backend backend = { .asm_file = asm_file, .cnt_if = 0, .cnt_while = 0, .id_name_table = id_name_table, .tabs = 0, .cur_scope = -1 };

	fprintf(asm_file, "#bx - amount of global variables default\n");
	fprintf(asm_file, "%s %d\n", array_commands[CMD_PUSH].cmd_name, CountOfGlobalVariables(id_name_table));
	fprintf(asm_file, "%s %s\n\n", array_commands[CMD_POP].cmd_name, STACK_FRAME_REGISTER);

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

BinaryTreeStatusCode GetComparisons(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	WriteAssembleCode(node->left, backend);
	WriteAssembleCode(node->right, backend);
	TABS fprintf(backend->asm_file, "%s ", array_commands[GetCmdByKeyWordType(node->data.val_key_word)].cmd_name);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode GetIf(Node_t* node, Backend* backend) {

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

BinaryTreeStatusCode GetWhile(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	(backend->cnt_while)++;
	size_t old_num = (backend->cnt_while);
	TABS fprintf(backend->asm_file, "while%zu:\n", (backend->cnt_while));
	(backend->tabs)++;
	TABS fprintf(backend->asm_file, "#while-condition\n");
	WriteAssembleCode(node->left, backend);
	fprintf(backend->asm_file, "end_while%zu:\n", (backend->cnt_while));
	TABS fprintf(backend->asm_file, "#while-body\n");
	WriteAssembleCode(node->right, backend);
	TABS fprintf(backend->asm_file, "%s while%zu:\n", array_commands[CMD_JMP].cmd_name, old_num);
	(backend->tabs)--;
	TABS fprintf(backend->asm_file, "end_while%zu:\n", old_num);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode GetFunctionDefinition(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	if (StrNCmp(MAIN, backend->id_name_table->data[node->data.val_func_def].string, StrLen(MAIN), (int)backend->id_name_table->data[node->data.val_func_def].length) == 0) {
		TABS fprintf(backend->asm_file, "#main\n");
		TABS fprintf(backend->asm_file, "%s %ls:\n", array_commands[CMD_CALL].cmd_name, MAIN);
		TABS fprintf(backend->asm_file, "%s\n\n", array_commands[CMD_HLT].cmd_name);
	}

	TABS PrintNString(backend->asm_file, backend->id_name_table->data[node->data.val_func_def].string, backend->id_name_table->data[node->data.val_func_def].length);
	fprintf(backend->asm_file, ":\n");
	(backend->tabs)++;
	WriteAssembleCode(node->right, backend);
	(backend->tabs)--;

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode GetMathFunctions(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	TABS fprintf(backend->asm_file, "#%s\n", GetCommentByKeyWordType(node->data.val_key_word));
	WriteAssembleCode(node->left, backend);
	WriteAssembleCode(node->right, backend);
	TABS fprintf(backend->asm_file, "%s\n", array_commands[GetCmdByKeyWordType(node->data.val_key_word)].cmd_name);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode GetAloneFunctions(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	TABS fprintf(backend->asm_file, "#%s\n", GetCommentByKeyWordType(node->data.val_key_word));
	WriteAssembleCode(node->right, backend);
	TABS fprintf(backend->asm_file, "%s\n", array_commands[GetCmdByKeyWordType(node->data.val_key_word)].cmd_name);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode GetReturn(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	TABS fprintf(backend->asm_file, "#%s\n", GetCommentByKeyWordType(node->data.val_key_word));
	WriteAssembleCode(node->right, backend);
	TABS fprintf(backend->asm_file, "%s %s\n", array_commands[CMD_POP].cmd_name, RETURN_VALUE_REGISTER);
	TABS fprintf(backend->asm_file, "%s\n", array_commands[GetCmdByKeyWordType(node->data.val_key_word)].cmd_name);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode GetParameters(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	WriteAssembleCode(node->left, backend);
	WriteAssembleCode(node->right, backend);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode GetCall(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	TABS fprintf(backend->asm_file, "#stack_frame_register\n");
	TABS fprintf(backend->asm_file, "%s %s\n", array_commands[CMD_PUSH].cmd_name, STACK_FRAME_REGISTER);
	TABS fprintf(backend->asm_file, "%s %s\n", array_commands[CMD_PUSH].cmd_name, STACK_FRAME_REGISTER);
	TABS fprintf(backend->asm_file, "%s %zu\n", array_commands[CMD_PUSH].cmd_name, backend->id_name_table->data[node->right->data.val_id].scope_variables.size);
	TABS fprintf(backend->asm_file, "%s\n", array_commands[CMD_ADD].cmd_name);
	TABS fprintf(backend->asm_file, "%s %s\n\n", array_commands[CMD_POP].cmd_name, STACK_FRAME_REGISTER);

	TABS fprintf(backend->asm_file, "#stack_frame\n");
	GetParameters(node->left, backend);

	TABS fprintf(backend->asm_file, "%s ", array_commands[CMD_CALL].cmd_name);
	PrintNString(backend->asm_file, backend->id_name_table->data[node->right->data.val_id].string, backend->id_name_table->data[node->right->data.val_id].length);
	fprintf(backend->asm_file, ":\n");
	TABS fprintf(backend->asm_file, "%s %s\n", array_commands[CMD_POP].cmd_name, STACK_FRAME_REGISTER);
	TABS fprintf(backend->asm_file, "%s %s\n", array_commands[CMD_PUSH].cmd_name, RETURN_VALUE_REGISTER);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode GetIdentifier(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	TABS fprintf(backend->asm_file, "%s [%d]\n", array_commands[CMD_PUSH].cmd_name, backend->id_name_table->data[node->data.val_id].num);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode GetNumber(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	TABS fprintf(backend->asm_file, "%s %lg\n", array_commands[CMD_PUSH].cmd_name, node->data.val_num);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode GetSequentialOp(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	WriteAssembleCode(node->left, backend);
	fprintf(backend->asm_file, "\n");
	WriteAssembleCode(node->right, backend);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode GetAssignment(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	TABS fprintf(backend->asm_file, "#assignment\n");
	WriteAssembleCode(node->left, backend);
	TABS fprintf(backend->asm_file, "%s [%d]\n", array_commands[CMD_POP].cmd_name, backend->id_name_table->data[node->right->data.val_id].num);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode GetInput(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	TABS fprintf(backend->asm_file, "#input\n");
	TABS fprintf(backend->asm_file, "%s\n", array_commands[CMD_IN].cmd_name);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode GetPrintf(Node_t* node, Backend* backend) {

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
				case ABOVE: 		{ GetComparisons(node, backend); 			break; }
				case SEQUENTIAL_OP: { GetSequentialOp(node, backend); 			break; }
				case IF: 			{ GetIf(node, backend); 					break; }
				case WHILE:			{ GetWhile(node, backend); 					break; }
				case ASSIGNMENT: 	{ GetAssignment(node, backend); 			break; }
				case INPUT: 		{ GetInput(node, backend); 					break; }
				case PRINTF: 		{ GetPrintf(node, backend); 				break; }
				case SUB:
				case MUL:
				case SQRT:
				case DIV:
				case ADD: 			{ GetMathFunctions(node, backend); 			break; }
				case RETURN:		{ GetReturn(node, backend); 				break; }
				case COS:
				case SIN: 			{ GetAloneFunctions(node, backend); 		break; }
				default: return TREE_NO_ERROR;
			}
			break;
		}
		case FUNCTION_DEFINITION: 	{ GetFunctionDefinition(node, backend);		 break; }
		case VAR_DECLARATION: 		{ WriteAssembleCode(node->right, backend); 	break; }
		case NUMBER: 		 		{ GetNumber(node, backend); 				break; }
		case IDENTIFIER: 			{ GetIdentifier(node, backend); 			 break; }
		case CALL: 					{ GetCall(node, backend); 					break; }
		case PARAMETERS:			{ WriteAssembleCode(node->right, backend);	break; }
		default: return TREE_NO_ERROR;
	}

#undef TABS

	return TREE_NO_ERROR;
}

