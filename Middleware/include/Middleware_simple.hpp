#pragma once

#include "IdNameTable.hpp"
#include "Tree_dump.hpp"

#define TREE_SIMPLIFICATION(tree, id_name_table) {								\
	tree_status = Simplification((tree)->root);									\
	BINARY_TREE_GRAPH_DUMP(tree, "Simplification", NULL, id_name_table);		\
}

int ConvolutionConstant(Node_t* node, size_t* count_of_changes);
int TrivialTransformations(Node_t* node, size_t* count_of_changes);
BinaryTreeStatusCode Simplification(Node_t* subtree_root);
Number_t Eval(Node_t* node);
