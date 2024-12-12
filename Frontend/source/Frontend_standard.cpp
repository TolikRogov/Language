#include "Frontend_standard.hpp"

BinaryTreeStatusCode TreeStandard(Tree* tree) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	FILE* tree_standard = fopen(FRONT_DATA_DIR_ TREE_STANDARD_FILE_, "w");
	if (!tree_standard)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

	tree_status = WriteTreeToFile(tree->root, tree_standard);
	TREE_ERROR_CHECK(tree_status);

	if (fclose(tree_standard))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode WriteTreeToFile(Node_t* node, FILE* tree_standard) {

	if (!node) {
		fprintf(tree_standard, "_ ");
		return TREE_NO_ERROR;
	}

	fprintf(tree_standard, "( ");
	fprintf(tree_standard, "%d ", node->type);
	PrintNodeDataStandard(node, tree_standard);

	WriteTreeToFile(node->left, tree_standard);
	WriteTreeToFile(node->right, tree_standard);

	fprintf(tree_standard, ") ");

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PrintNodeDataStandard(Node_t* node, FILE* stream) {

	if (!node)
		TREE_ERROR_CHECK(TREE_NULL_POINTER);

	switch (node->type) {
		case NUMBER:				{ fprintf(stream, "%lg ", node->data.val_num); 			break; }
		case IDENTIFIER:			{ fprintf(stream, "%zu ", node->data.val_id);  			break; }
		case KEYWORD:				{ fprintf(stream, "%d ",  node->data.val_key_word); 	break; }
		case FUNCTION_DEFINITION:	{ fprintf(stream, "%zu ", node->data.val_func_def);		break; }
		case VAR_DECLARATION:		{ fprintf(stream, "%zu ", node->data.val_decl_var);		break; }
		case PARAMETERS:
		case CALL:
		case UNW:
		default: return TREE_NO_ERROR;
	}

	return TREE_NO_ERROR;
}
