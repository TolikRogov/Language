#pragma once

#include "Frontend_descent.hpp"
#include "Standard.hpp"

#define STANDARDIZE(tree, id_name_table) {				 \
	tree_status = TreeStandard(tree);					\
	TREE_ERROR_CHECK(tree_status);						\
}

BinaryTreeStatusCode TreeStandard(Tree* tree);
BinaryTreeStatusCode WriteTreeToFile(Node_t* node, FILE* tree_standard);
BinaryTreeStatusCode PrintNodeDataStandard(Node_t* node, FILE* stream);
BinaryTreeStatusCode IdNameTableStandard(IdNameTable* id_name_table);
