#pragma once

#include "IdNameTable.hpp"

#define DATA_DIR_					"../data/"
#define PROG_FILE_					DATA_DIR_ "main.txt"
#define TREE_STANDARD_FILE_			DATA_DIR_ "tree_standard.txt"
#define NAME_TABLE_STANDARD_FILE_	DATA_DIR_ "name_table_standard.txt"

#define STANDARDIZE(tree) {				 \
	tree_status = TreeStandard(tree);	\
	TREE_ERROR_CHECK(tree_status);		\
}

BinaryTreeStatusCode TreeStandard(Tree* tree);
BinaryTreeStatusCode WriteTreeToFile(Node_t* node, FILE* tree_standard);
BinaryTreeStatusCode PrintNodeDataStandard(Node_t* node, FILE* stream);
BinaryTreeStatusCode IdNameTableStandard(IdNameTable* id_name_table);
