#include "NasmBackend.hpp"

#define STACK_FRAME_REGISTER 	"rbx"
#define RETURN_VALUE_REGISTER 	"rax"
#define ASSIGNMENT_REGISTER		"r8"

static BinaryTreeStatusCode EmitComparisons(Node_t* node, Backend* backend);
static BinaryTreeStatusCode EmitIf(Node_t* node, Backend* backend);
static BinaryTreeStatusCode EmitWhile(Node_t* node, Backend* backend);
static BinaryTreeStatusCode EmitDefinitionParameters(Node_t* node, Backend* backend);
static BinaryTreeStatusCode EmitFunctionDefinition(Node_t* node, Backend* backend);
static BinaryTreeStatusCode EmitMathFunctions(Node_t* node, Backend* backend);
static BinaryTreeStatusCode EmitAloneFunctions(Node_t* node, Backend* backend);
static BinaryTreeStatusCode EmitReturn(Node_t* node, Backend* backend);
static BinaryTreeStatusCode EmitParameters(Node_t* node, Backend* backend);
static BinaryTreeStatusCode EmitCommaOp(Node_t* node, Backend* backend);
static BinaryTreeStatusCode EmitCallParameters(Node_t* node, Backend* backend);
static BinaryTreeStatusCode EmitCall(Node_t* node, Backend* backend);
static BinaryTreeStatusCode EmitIdentifier(Node_t* node, Backend* backend);
static BinaryTreeStatusCode EmitNumber(Node_t* node, Backend* backend);
static BinaryTreeStatusCode EmitSequentialOp(Node_t* node, Backend* backend);
static BinaryTreeStatusCode EmitAssignment(Node_t* node, Backend* backend);
static BinaryTreeStatusCode EmitInput(Node_t* node, Backend* backend);
static BinaryTreeStatusCode EmitPrintf(Node_t* node, Backend* backend);
static BinaryTreeStatusCode EmitAbort(Node_t* node, Backend* backend);

BinaryTreeStatusCode RunBackend(Tree* tree, IdNameTable* id_name_table) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	FILE* asm_file = fopen(ASM_FILE_, "w");
	if (!asm_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

	Backend backend = { .id_name_table = id_name_table,
						.asm_file = asm_file,
						.tabs = 0,
						.cnt_if = 0,
						.cnt_while = 0,
						.cur_scope = -1 };

	tree_status = SectionsDataAndText(&backend, id_name_table);
	TREE_ERROR_CHECK(tree_status);

	tree_status = WriteAssembleCode(tree->root, &backend);
	TREE_ERROR_CHECK(tree_status);

	if (fclose(asm_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode SectionsDataAndText(Backend* backend, IdNameTable* id_name_table) {

	fprintf(backend->asm_file, "section .data\n");

	for (size_t i = 0; i < id_name_table->size - 1; i++) {
		Identifier cur_id = id_name_table->data[i];

		if (cur_id.type != ID_VAR || !cur_id.global)
			continue;

		fprintf(backend->asm_file, "\t_%zu dq 0 ;", i);
		PrintNString(backend->asm_file, cur_id.string, cur_id.length);
		fprintf(backend->asm_file, "\n");
	}

	fprintf(backend->asm_file, "\n");
	fprintf(backend->asm_file, "section .text\n");
	fprintf(backend->asm_file, "\tglobal main\n");
	fprintf(backend->asm_file, "\textern %s\n", array_commands[CMD_IN].cmd_name);
	fprintf(backend->asm_file, "\textern %s\n", array_commands[CMD_OUT].cmd_name);

	fprintf(backend->asm_file, "\n");
	fprintf(backend->asm_file, "main:\n");
	(backend->tabs)++;

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
		case ABOVE: 		return CMD_JLE;
		case EQUAL:			return CMD_JNE;
		case NOT_EQUAL:		return CMD_JE;
		case BELOW_EQUAL: 	return CMD_JG;
		case ABOVE_EQUAL:	return CMD_JL;
		case BELOW:			return CMD_JGE;
		case INPUT: 		return CMD_IN;
		case PRINTF:		return CMD_OUT;
		case ADD:			return CMD_ADD;
		case SUB:			return CMD_SUB;
		case MUL:			return CMD_MUL;
		case SQRT:			return CMD_SQRT;
		case DIV:			return CMD_DIV;
		case SIN:			return CMD_SIN;
		case RETURN:		return CMD_RET;
		case ABORT:			return CMD_HLT;
		default:			return COUNT_OF_COMMANDS;
	}
}

BinaryTreeStatusCode EmitComparisons(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	Commands cur_cmd = GetCmdByKeyWordType(node->data.val_key_word);

	WriteAssembleCode(node->left, backend);
	WriteAssembleCode(node->right, backend);
	TABS fprintf(backend->asm_file, "%s ", array_commands[cur_cmd].cmd_name);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode EmitAbort(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	Commands cur_cmd = GetCmdByKeyWordType(node->data.val_key_word);
	TABS fprintf(backend->asm_file, "%s\n", array_commands[cur_cmd].cmd_name);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode EmitIf(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	(backend->cnt_if)++;
	size_t old_num = (backend->cnt_if);

	TABS fprintf(backend->asm_file, ";if-condition\n");

		WriteAssembleCode(node->left, backend);

	fprintf(backend->asm_file, "end_if%zu:\n", (backend->cnt_if));

	(backend->tabs)++;

		TABS fprintf(backend->asm_file, ";if-body\n");
		WriteAssembleCode(node->right, backend);

	(backend->tabs)--;

	TABS fprintf(backend->asm_file, "end_if%zu:\n", old_num);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode EmitWhile(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	(backend->cnt_while)++;
	size_t old_num = (backend->cnt_while);

	TABS fprintf(backend->asm_file, "while%zu:\n", (backend->cnt_while));

		(backend->tabs)++;
		TABS fprintf(backend->asm_file, ";while-condition\n");
		WriteAssembleCode(node->left,  backend);

		fprintf(backend->asm_file,      "end_while%zu:\n", (backend->cnt_while));
		TABS fprintf(backend->asm_file, ";while-body\n");
		WriteAssembleCode(node->right, backend);

		TABS fprintf(backend->asm_file, "%s while%zu:\n", array_commands[CMD_JMP].cmd_name, old_num);
		(backend->tabs)--;

	TABS fprintf(backend->asm_file, "end_while%zu:\n", old_num);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode EmitDefinitionParameters(Node_t* node, Backend* backend) {

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

BinaryTreeStatusCode EmitFunctionDefinition(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }
#define ASM_PRINTF(...) fprintf(backend->asm_file, __VA_ARGS__);

	static int func_num = 0;

	Identifier* cur_func = &backend->id_name_table->data[node->data.val_func_def];
	int old_scope = backend->cur_scope;
	backend->cur_scope = (int)node->data.val_func_def;

	if (func_num) {
		ASM_PRINTF("_%d: ;", cur_func->num);
		TABS PrintNString(backend->asm_file, cur_func->string, cur_func->length);
		ASM_PRINTF("\n");
		(backend->tabs)++;
	}

	func_num++;

	WriteAssembleCode(node->right, backend);
	(backend->tabs)--;
	backend->cur_scope = old_scope;

#undef ASM_PRINTF
#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode EmitMathFunctions(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	Commands cur_cmd = GetCmdByKeyWordType(node->data.val_key_word);

	TABS fprintf(backend->asm_file, ";%s\n", GetCommentByKeyWordType(node->data.val_key_word));
		WriteAssembleCode(node->left, backend);
		WriteAssembleCode(node->right, backend);
	TABS fprintf(backend->asm_file, "%s\n", array_commands[cur_cmd].cmd_name);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode EmitAloneFunctions(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	Commands cur_cmd = GetCmdByKeyWordType(node->data.val_key_word);

	TABS fprintf(backend->asm_file, ";%s\n", GetCommentByKeyWordType(node->data.val_key_word));
		WriteAssembleCode(node->right, backend);
	TABS fprintf(backend->asm_file, "%s\n", array_commands[cur_cmd].cmd_name);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode EmitReturn(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	Commands cur_cmd = GetCmdByKeyWordType(node->data.val_key_word);

	TABS fprintf(backend->asm_file, ";%s\n", GetCommentByKeyWordType(node->data.val_key_word));
		WriteAssembleCode(node->right, backend);

	TABS fprintf(backend->asm_file, "\n;write return value to register\n");
	TABS fprintf(backend->asm_file, "%s %s\n\n", array_commands[CMD_POP].cmd_name, RETURN_VALUE_REGISTER);

	TABS fprintf(backend->asm_file, "%s\n", array_commands[cur_cmd].cmd_name);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode EmitParameters(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	EmitDefinitionParameters(node, backend);
	WriteAssembleCode(node->right, backend);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode EmitCommaOp(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	if (node->left) EmitCommaOp(node->left, backend);
	WriteAssembleCode(node->right, backend);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode EmitCallParameters(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }
#define ASM_PRINTF(...) fprintf(backend->asm_file, __VA_ARGS__);

	TABS ASM_PRINTF(";push previous value of stack frame register before new call\n");
		TABS ASM_PRINTF("%s rbp, rsp\n", array_commands[CMD_MOV].cmd_name);

	EmitCommaOp(node->left, backend);

#undef ASM_PRINTF
#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode EmitCall(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }
#define ASM_PRINTF(...) fprintf(backend->asm_file, __VA_ARGS__);

	Identifier* cur_id = &backend->id_name_table->data[node->right->data.val_id];

	EmitCallParameters(node, backend);

	TABS ASM_PRINTF(";call function\n");
		TABS ASM_PRINTF("%s _%d ;", array_commands[CMD_CALL].cmd_name, cur_id->num);
		PrintNString(backend->asm_file, cur_id->string, cur_id->length);
		fprintf(backend->asm_file, "\n");

	TABS ASM_PRINTF(";Emit back stack frame register value as before calling function\n");
		TABS ASM_PRINTF("%s rsp, rbp\n", array_commands[CMD_MOV].cmd_name);

	TABS ASM_PRINTF(";push return value of function\n");
		TABS ASM_PRINTF("%s %s\n", array_commands[CMD_PUSH].cmd_name, RETURN_VALUE_REGISTER);

#undef ASM_PRINTF
#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode EmitIdentifier(Node_t* node, Backend* backend) {

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

BinaryTreeStatusCode EmitNumber(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	TABS fprintf(backend->asm_file, "%s %s, %lg\n", array_commands[CMD_MOV].cmd_name, ASSIGNMENT_REGISTER, node->data.val_num);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode EmitSequentialOp(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	WriteAssembleCode(node->left, backend);
	fprintf(backend->asm_file, "\n");
	WriteAssembleCode(node->right, backend);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode EmitAssignment(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	TABS fprintf(backend->asm_file, ";assignment\n");
	WriteAssembleCode(node->left, backend);

	Identifier* cur_id = &backend->id_name_table->data[node->right->data.val_id];

	if (backend->cur_scope == -1 || cur_id->global == 1) {
		TABS fprintf(backend->asm_file, "%s [_%d], %s ;", array_commands[CMD_MOV].cmd_name, cur_id->num, ASSIGNMENT_REGISTER);
	}
	else {
		TABS fprintf(backend->asm_file, "%s [%s+%d] ;", array_commands[CMD_POP].cmd_name, STACK_FRAME_REGISTER,
		GetLocalNumberOfVariable(backend->id_name_table, backend->cur_scope, cur_id->num));
	}

	PrintNString(backend->asm_file, cur_id->string, cur_id->length);
	fprintf(backend->asm_file, "\n");

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode EmitInput(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	TABS fprintf(backend->asm_file, ";input\n");
	TABS fprintf(backend->asm_file, "%s ", array_commands[CMD_CALL].cmd_name);
	fprintf(backend->asm_file, "%s\n", array_commands[CMD_IN].cmd_name);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode EmitPrintf(Node_t* node, Backend* backend) {

#define TABS { for (size_t i = 0; i < backend->tabs; i++) {fprintf(backend->asm_file, "\t");} }

	TABS fprintf(backend->asm_file, ";printf\n");
	WriteAssembleCode(node->right, backend);
	TABS fprintf(backend->asm_file, "%s ", array_commands[CMD_CALL].cmd_name);
	fprintf(backend->asm_file, "%s\n", array_commands[CMD_OUT].cmd_name);

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
				case BELOW:
				case NOT_EQUAL:
				case BELOW_EQUAL:
				case ABOVE_EQUAL:
				case ABOVE: 		{ EmitComparisons(node, backend); 			break; }
				case SEQUENTIAL_OP: { EmitSequentialOp(node, backend); 			break; }
				case IF: 			{ EmitIf(node, backend); 					break; }
				case WHILE:			{ EmitWhile(node, backend); 				break; }
				case ASSIGNMENT: 	{ EmitAssignment(node, backend); 			break; }
				case INPUT: 		{ EmitInput(node, backend); 				break; }
				case PRINTF: 		{ EmitPrintf(node, backend); 				break; }
				case SUB:
				case MUL:
				case SQRT:
				case DIV:
				case ADD: 			{ EmitMathFunctions(node, backend); 		break; }
				case RETURN:		{ EmitReturn(node, backend); 				break; }
				case COS:
				case SIN: 			{ EmitAloneFunctions(node, backend); 		break; }
				case ABORT:			{ EmitAbort(node, backend);					break; }
				default: return TREE_NO_ERROR;
			}
			break;
		}
		case FUNCTION_DEFINITION: 	{ EmitFunctionDefinition(node, backend);	 break; }
		case VAR_DECLARATION: 		{ WriteAssembleCode(node->right, backend); 	break; }
		case NUMBER: 		 		{ EmitNumber(node, backend); 				break; }
		case IDENTIFIER: 			{ EmitIdentifier(node, backend); 			 break; }
		case CALL: 					{ EmitCall(node, backend); 					break; }
		case PARAMETERS:			{ EmitParameters(node, backend);			break; }
		default: return TREE_NO_ERROR;
	}

#undef TABS

	return TREE_NO_ERROR;
}
