#pragma once

#include "IdNameTable.hpp"
#include "Tree_dump.hpp"
#include "Standard.hpp"
#include "Lexer.hpp"

#define STANDARD_READER(tree, id_name_table, lexer) {				 \
	tree_status = ReadNameTableStandard(id_name_table);				\
	TREE_ERROR_CHECK(tree_status);									\
	tree_status = ReadTreeStandard(tree, id_name_table, lexer);		\
	TREE_ERROR_CHECK(tree_status);									\
}

BinaryTreeStatusCode ReadTreeStandard(Tree* tree, IdNameTable* id_name_table, Lexer* lexer);
BinaryTreeStatusCode ReadNameTableStandard(IdNameTable* id_name_table);
