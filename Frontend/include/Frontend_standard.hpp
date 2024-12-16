#pragma once

#include "Frontend_descent.hpp"

#define TREE_STANDARD_FILE_			"tree_standard.txt"
#define NAME_TABLE_STANDARD_FILE_	"name_table_standard.txt"

#define STANDARDIZE(tree, id_name_table) {				 \
	tree_status = TreeStandard(tree);					\
	TREE_ERROR_CHECK(tree_status);						\
}

BinaryTreeStatusCode TreeStandard(Tree* tree);
BinaryTreeStatusCode WriteTreeToFile(Node_t* node, FILE* tree_standard);
BinaryTreeStatusCode PrintNodeDataStandard(Node_t* node, FILE* stream);
BinaryTreeStatusCode IdNameTableStandard(IdNameTable* id_name_table);
