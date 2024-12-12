#pragma once

#include "Frontend_lexer.hpp"

#define FRONT_DATA_DIR_			"../data/"
#define FRONT_PROG_FILE_		FRONT_DATA_DIR_ "main.txt"

#define READ_PROGRAM(tree, id_name_table, lexer) {		      	 \
	tree_status = ReadProgram(tree, id_name_table, lexer);		\
	TREE_ERROR_CHECK(tree_status);							    \
}

BinaryTreeStatusCode ReadProgram(Tree* tree, IdNameTable* id_name_table, Lexer* lexer);
