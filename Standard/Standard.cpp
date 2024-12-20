#include "Standard.hpp"

BinaryTreeStatusCode IdNameTableStandard(IdNameTable* id_name_table) {

	FILE* name_table_standard = fopen(NAME_TABLE_STANDARD_FILE_, "w");
	if (!name_table_standard)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

	fprintf(name_table_standard, "%zu\n", id_name_table->size);
	for (size_t i = 0; i < id_name_table->size; i++){
		PrintNString(name_table_standard, id_name_table->data[i].string, id_name_table->data[i].length);
		fprintf(name_table_standard, "\n");
	}

	fprintf(name_table_standard, "\n");

	size_t function_count = 0;
	for (size_t i = 0; i < id_name_table->size; i++) {
		if (id_name_table->data[i].type == ID_FUNCTION)
			function_count++;
	}
	fprintf(name_table_standard, "%zu\n", function_count + 1);

	fprintf(name_table_standard, "\n");

	fprintf(name_table_standard, "%d -1\n", CountOfGlobals(id_name_table));
	for (size_t i = 0; i < id_name_table->size; i++) {
		if (id_name_table->data[i].global)
			fprintf(name_table_standard, "%d %d\n", id_name_table->data[i].num, id_name_table->data[i].type);
	}

	fprintf(name_table_standard, "\n");

	for (size_t i = 0; i < id_name_table->size; i++) {
		if (id_name_table->data[i].type == ID_FUNCTION) {
			fprintf(name_table_standard, "%zu %d\n", id_name_table->data[i].scope_variables.size, id_name_table->data[i].num);

			for (size_t j = 0; j < id_name_table->data[i].scope_variables.size; j++) {
				fprintf(name_table_standard, "%d %d\n", id_name_table->data[i].scope_variables.data[j],
														id_name_table->data[id_name_table->data[i].scope_variables.data[j]].type);
			}

			fprintf(name_table_standard, "\n");
		}
	}

	if (fclose(name_table_standard))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode TreeStandard(Tree* tree) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	FILE* tree_standard = fopen(TREE_STANDARD_FILE_, "w");
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
