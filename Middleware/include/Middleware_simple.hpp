#pragma once

#include "IdNameTable.hpp"
#include "Tree_dump.hpp"

#define TREE_SIMPLIFICATION(tree, id_name_table) {								 \
	tree_status = Simplification((tree)->root, id_name_table);					 \
	TREE_ERROR_CHECK(tree_status);											  	\
	BINARY_TREE_GRAPH_DUMP(tree, "Simplification", NULL, id_name_table);		 \
	tree_status = NodeParentChecker((tree)->root);							  	\
	TREE_ERROR_CHECK(tree_status);												\
}

int ConvolutionConstant(Node_t* node, size_t* count_of_changes, IdNameTable* id_name_table);
int TrivialTransformations(Node_t* node, size_t* count_of_changes);
Node_t* doDifferentiation(Node_t* node, IdNameTable* id_name_table);
BinaryTreeStatusCode Simplification(Node_t* subtree_root, IdNameTable* id_name_table);
Number_t Eval(Node_t* node);
