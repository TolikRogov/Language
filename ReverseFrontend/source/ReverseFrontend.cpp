#include "ReverseFrontend.hpp"

static BinaryTreeStatusCode PullComparisons(Node_t* node, ReverseFrontend* revfront);
static BinaryTreeStatusCode PullIf(Node_t* node, ReverseFrontend* revfront);
static BinaryTreeStatusCode PullVariableDeclaration(Node_t* node, ReverseFrontend* revfront);
static BinaryTreeStatusCode PullWhile(Node_t* node, ReverseFrontend* revfront);
static BinaryTreeStatusCode PullDefinitionParameters(Node_t* node, ReverseFrontend* revfront);
static BinaryTreeStatusCode PullFunctionDefinition(Node_t* node, ReverseFrontend* revfront);
static BinaryTreeStatusCode PullMathFunctions(Node_t* node, ReverseFrontend* revfront);
static BinaryTreeStatusCode PullAloneFunctions(Node_t* node, ReverseFrontend* revfront);
static BinaryTreeStatusCode PullReturn(Node_t* node, ReverseFrontend* revfront);
static BinaryTreeStatusCode PullCommaOp(Node_t* node, ReverseFrontend* revfront);
static BinaryTreeStatusCode PullCallParameters(Node_t* node, ReverseFrontend* revfront);
static BinaryTreeStatusCode PullCall(Node_t* node, ReverseFrontend* revfront);
static BinaryTreeStatusCode PullIdentifier(Node_t* node, ReverseFrontend* revfront);
static BinaryTreeStatusCode PullNumber(Node_t* node, ReverseFrontend* revfront);
static BinaryTreeStatusCode PullSequentialOp(Node_t* node, ReverseFrontend* revfront);
static BinaryTreeStatusCode PullAssignment(Node_t* node, ReverseFrontend* revfront);
static BinaryTreeStatusCode PullPrintf(Node_t* node, ReverseFrontend* revfront);
static BinaryTreeStatusCode PullInput(Node_t* node, ReverseFrontend* revfront);
static BinaryTreeStatusCode PullKeyWordString(Node_t* node, ReverseFrontend* revfront);
static BinaryTreeStatusCode PullAbort(Node_t* node, ReverseFrontend* revfront);

BinaryTreeStatusCode RunReverseFrontend(Tree* tree, IdNameTable* id_name_table) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	FILE* prog_file = fopen(REVFRONT_FILE_, "w");
	if (!prog_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

	ReverseFrontend revfront = { .prog_file = prog_file, .id_name_table = id_name_table, .tabs = 0, .cur_scope = -1 };

	tree_status = WriteProgram(tree->root, &revfront);
	TREE_ERROR_CHECK(tree_status);

	if (fclose(prog_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode WriteProgram(Node_t* node, ReverseFrontend* revfront) {

	if (!node)
		return TREE_NO_ERROR;

	switch (node->type) {
		case KEYWORD: {
			switch (node->data.val_key_word) {
				case EQUAL:
				case BELOW:
				case BELOW_EQUAL:
				case ABOVE_EQUAL:
				case ABOVE: 		{ PullComparisons(node, revfront); 			break; }
				case SEQUENTIAL_OP: { PullSequentialOp(node, revfront); 		break; }
				case IF: 			{ PullIf(node, revfront); 					break; }
				case WHILE:			{ PullWhile(node, revfront); 				break; }
				case ASSIGNMENT: 	{ PullAssignment(node, revfront); 			break; }
				case INPUT: 		{ PullInput(node, revfront); 				break; }
				case PRINTF: 		{ PullPrintf(node, revfront); 				break; }
				case SUB:
				case MUL:
				case DIV:
				case ADD: 			{ PullMathFunctions(node, revfront); 		break; }
				case RETURN:		{ PullReturn(node, revfront); 				break; }
				case SQRT:
				case COS:
				case SIN: 			{ PullAloneFunctions(node, revfront); 		break; }
				case INIT_TYPE:		{ PullKeyWordString(node, revfront);		break; }
				case COMMA_OP:		{ PullCommaOp(node, revfront);				break; }
				case ABORT:			{ PullAbort(node, revfront);				break; }
				default: return TREE_NO_ERROR;
			}
			break;
		}
		case FUNCTION_DEFINITION: 	{ PullFunctionDefinition(node, revfront);	 break; }
		case VAR_DECLARATION: 		{ PullVariableDeclaration(node, revfront); 	break; }
		case NUMBER: 		 		{ PullNumber(node, revfront); 				break; }
		case IDENTIFIER: 			{ PullIdentifier(node, revfront); 			 break; }
		case CALL: 					{ PullCall(node, revfront); 				break; }
		case PARAMETERS:
		default: return TREE_NO_ERROR;
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullSequentialOp(Node_t* node, ReverseFrontend* revfront) {

#define TABS { for (size_t i = 0; i < revfront->tabs; i++) {fprintf(revfront->prog_file, "\t");} }

	WriteProgram(node->left, revfront);
	fprintf(revfront->prog_file, "%ls\n", KeyWordsGetString(node->data.val_key_word));
	WriteProgram(node->right, revfront);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullAbort(Node_t* node, ReverseFrontend* revfront) {

#define TABS { for (size_t i = 0; i < revfront->tabs; i++) {fprintf(revfront->prog_file, "\t");} }

	TABS fprintf(revfront->prog_file, "%ls ", KeyWordsGetString(node->data.val_key_word));

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullAssignment(Node_t* node, ReverseFrontend* revfront) {

#define TABS { for (size_t i = 0; i < revfront->tabs; i++) {fprintf(revfront->prog_file, "\t");} }

	if (node->parent->type == KEYWORD && node->parent->data.val_key_word == SEQUENTIAL_OP) {
		TABS
	}

	WriteProgram(node->right, revfront);
	fprintf(revfront->prog_file, "%ls ", KeyWordsGetString(node->data.val_key_word));
	WriteProgram(node->left, revfront);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullKeyWordString(Node_t* node, ReverseFrontend* revfront) {

#define TABS { for (size_t i = 0; i < revfront->tabs; i++) {fprintf(revfront->prog_file, "\t");} }

	TABS fprintf(revfront->prog_file, "%ls ", KeyWordsGetString(node->data.val_key_word));

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullInput(Node_t* node, ReverseFrontend* revfront) {

#define TABS { for (size_t i = 0; i < revfront->tabs; i++) {fprintf(revfront->prog_file, "\t");} }

	fprintf(revfront->prog_file, "%ls ", KeyWordsGetString(node->data.val_key_word));

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullVariableDeclaration(Node_t* node, ReverseFrontend* revfront) {

#define TABS { for (size_t i = 0; i < revfront->tabs; i++) {fprintf(revfront->prog_file, "\t");} }

	WriteProgram(node->left, revfront);
	WriteProgram(node->right, revfront);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullFunctionDefinition(Node_t* node, ReverseFrontend* revfront) {

#define TABS { for (size_t i = 0; i < revfront->tabs; i++) {fprintf(revfront->prog_file, "\t");} }
#define PROG_PRINTF(...) fprintf(revfront->prog_file, __VA_ARGS__);

	Identifier* cur_func_def = &revfront->id_name_table->data[node->data.val_func_def];

	WriteProgram(node->left, revfront);
	PrintNString(revfront->prog_file, cur_func_def->string, cur_func_def->length);
	PROG_PRINTF(" ");
	PullDefinitionParameters(node->right, revfront);

#undef PROG_PRINTF
#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullDefinitionParameters(Node_t* node, ReverseFrontend* revfront) {

#define TABS { for (size_t i = 0; i < revfront->tabs; i++) {fprintf(revfront->prog_file, "\t");} }
#define PROG_PRINTF(...) fprintf(revfront->prog_file, __VA_ARGS__);

	PROG_PRINTF("%ls ", KeyWordsGetString(OPEN_ROUND));
		WriteProgram(node->left, revfront);
	PROG_PRINTF("%ls ", KeyWordsGetString(CLOSE_ROUND));

	PROG_PRINTF("%ls\n", KeyWordsGetString(OPEN_FIGURE));
	revfront->tabs++;
		WriteProgram(node->right, revfront);
	revfront->tabs--;
	TABS PROG_PRINTF("%ls ", KeyWordsGetString(CLOSE_FIGURE));

#undef PROG_PRINTF
#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullCommaOp(Node_t* node, ReverseFrontend* revfront) {

#define TABS { for (size_t i = 0; i < revfront->tabs; i++) {fprintf(revfront->prog_file, "\t");} }
#define PROG_PRINTF(...) fprintf(revfront->prog_file, __VA_ARGS__);

	if (!node->right)
		return TREE_NO_ERROR;

	WriteProgram(node->right, revfront);

	if (node->left) {
		PullCommaOp(node->left, revfront);
		PROG_PRINTF("%ls ", KeyWordsGetString(node->data.val_key_word));
	}

#undef PROG_PRINTF
#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullPrintf(Node_t* node, ReverseFrontend* revfront) {

#define TABS { for (size_t i = 0; i < revfront->tabs; i++) {fprintf(revfront->prog_file, "\t");} }

	TABS fprintf(revfront->prog_file, "%ls ", KeyWordsGetString(node->data.val_key_word));
	WriteProgram(node->right, revfront);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullCall(Node_t* node, ReverseFrontend* revfront) {

#define TABS { for (size_t i = 0; i < revfront->tabs; i++) {fprintf(revfront->prog_file, "\t");} }
#define PROG_PRINTF(...) fprintf(revfront->prog_file, __VA_ARGS__);

	Identifier* cur_id = &revfront->id_name_table->data[node->right->data.val_id];

	PrintNString(revfront->prog_file, cur_id->string, cur_id->length);
	PROG_PRINTF(" ");
	PullCallParameters(node->left, revfront);

#undef PROG_PRINTF
#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullCallParameters(Node_t* node, ReverseFrontend* revfront) {

#define TABS { for (size_t i = 0; i < revfront->tabs; i++) {fprintf(revfront->prog_file, "\t");} }
#define PROG_PRINTF(...) fprintf(revfront->prog_file, __VA_ARGS__);

	PROG_PRINTF("%ls ", KeyWordsGetString(OPEN_ROUND));
		WriteProgram(node, revfront);
	PROG_PRINTF("%ls ", KeyWordsGetString(CLOSE_ROUND));

#undef PROG_PRINTF
#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullReturn(Node_t* node, ReverseFrontend* revfront) {

#define TABS { for (size_t i = 0; i < revfront->tabs; i++) {fprintf(revfront->prog_file, "\t");} }

	TABS fprintf(revfront->prog_file, "%ls ", KeyWordsGetString(node->data.val_key_word));
	WriteProgram(node->right, revfront);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullNumber(Node_t* node, ReverseFrontend* revfront) {

#define TABS { for (size_t i = 0; i < revfront->tabs; i++) {fprintf(revfront->prog_file, "\t");} }

	fprintf(revfront->prog_file, "%lg ", node->data.val_num);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullIdentifier(Node_t* node, ReverseFrontend* revfront) {

#define TABS { for (size_t i = 0; i < revfront->tabs; i++) {fprintf(revfront->prog_file, "\t");} }

	Identifier* cur_id = &revfront->id_name_table->data[node->data.val_id];

	PrintNString(revfront->prog_file, cur_id->string, cur_id->length);
	fprintf(revfront->prog_file, " ");

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullIf(Node_t* node, ReverseFrontend* revfront) {

#define TABS { for (size_t i = 0; i < revfront->tabs; i++) {fprintf(revfront->prog_file, "\t");} }
#define PROG_PRINTF(...) fprintf(revfront->prog_file, __VA_ARGS__);

	TABS PROG_PRINTF("%ls ", KeyWordsGetString(IF));
	PROG_PRINTF("%ls ", KeyWordsGetString(OPEN_ROUND));
		WriteProgram(node->left, revfront);
	PROG_PRINTF("%ls ", KeyWordsGetString(CLOSE_ROUND));

	PROG_PRINTF("%ls\n", KeyWordsGetString(OPEN_FIGURE));
	revfront->tabs++;
		WriteProgram(node->right, revfront);
	revfront->tabs--;
	TABS PROG_PRINTF("%ls ", KeyWordsGetString(CLOSE_FIGURE));

#undef PROG_PRINTF
#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullComparisons(Node_t* node, ReverseFrontend* revfront) {

#define TABS { for (size_t i = 0; i < revfront->tabs; i++) {fprintf(revfront->prog_file, "\t");} }

	WriteProgram(node->left, revfront);
		fprintf(revfront->prog_file, "%ls ", KeyWordsGetString(node->data.val_key_word));
	WriteProgram(node->right, revfront);

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullWhile(Node_t* node, ReverseFrontend* revfront) {

#define TABS { for (size_t i = 0; i < revfront->tabs; i++) {fprintf(revfront->prog_file, "\t");} }
#define PROG_PRINTF(...) fprintf(revfront->prog_file, __VA_ARGS__);

	TABS PROG_PRINTF("%ls ", KeyWordsGetString(WHILE));
	PROG_PRINTF("%ls ", KeyWordsGetString(OPEN_ROUND));
		WriteProgram(node->left, revfront);
	PROG_PRINTF("%ls ", KeyWordsGetString(CLOSE_ROUND));

	PROG_PRINTF("%ls\n", KeyWordsGetString(OPEN_FIGURE));
	revfront->tabs++;
		WriteProgram(node->right, revfront);
	revfront->tabs--;
	TABS PROG_PRINTF("%ls ", KeyWordsGetString(CLOSE_FIGURE));

#undef PROG_PRINTF
#undef TABS

	return TREE_NO_ERROR;
}

int NeedBrackets(Node_t* node_parent, Node_t* node_child) {

	if (!node_parent || !node_child)
		return 0;

	if (node_parent->type != KEYWORD || node_child->type != KEYWORD)
		return 0;

	switch (node_parent->data.val_key_word) {
		case DIV:
		case MUL: {
			switch (node_child->data.val_key_word) {
				case MUL:
				case DIV:
				case ADD:
				case SUB: 	return 1;
				default: 	return 0;
			}
		}
		default: return 0;
	}

}

BinaryTreeStatusCode PullMathFunctions(Node_t* node, ReverseFrontend* revfront) {

#define TABS { for (size_t i = 0; i < revfront->tabs; i++) {fprintf(revfront->prog_file, "\t");} }

	int need_brackets = NeedBrackets(node, node->left);

	if (need_brackets)
		fprintf(revfront->prog_file, "%ls ", KeyWordsGetString(OPEN_ROUND));
	WriteProgram(node->left, revfront);
	if (need_brackets)
		fprintf(revfront->prog_file, "%ls ", KeyWordsGetString(CLOSE_ROUND));

	fprintf(revfront->prog_file, "%ls ", KeyWordsGetString(node->data.val_key_word));

	need_brackets = NeedBrackets(node, node->right);

	if (need_brackets)
		fprintf(revfront->prog_file, "%ls ", KeyWordsGetString(OPEN_ROUND));
	WriteProgram(node->right, revfront);
	if (need_brackets)
		fprintf(revfront->prog_file, "%ls ", KeyWordsGetString(CLOSE_ROUND));

#undef TABS

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PullAloneFunctions(Node_t* node, ReverseFrontend* revfront) {

#define TABS { for (size_t i = 0; i < revfront->tabs; i++) {fprintf(revfront->prog_file, "\t");} }

	fprintf(revfront->prog_file, "%ls ", KeyWordsGetString(node->data.val_key_word));
	fprintf(revfront->prog_file, "%ls ", KeyWordsGetString(OPEN_ROUND));
		WriteProgram(node->right, revfront);
	fprintf(revfront->prog_file, "%ls ", KeyWordsGetString(CLOSE_ROUND));

#undef TABS

	return TREE_NO_ERROR;
}
