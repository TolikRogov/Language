#pragma once

#include "Frontend_lexer.hpp"

#define FRONT_DATA_DIR_			"../data/"
#define FRONT_PROG_FILE_		FRONT_DATA_DIR_ "main.txt"

#define CREATE_TREE_FROM_FILE(tree, id_name_table, lexer) {		 		 \
	tree_status = CreateTreeFromFile(tree, id_name_table, lexer);		\
	TREE_ERROR_CHECK(tree_status);							   			\
	tree_status = IdNameTableStandard(id_name_table);					\
	TREE_ERROR_CHECK(tree_status);										\
}

#define LANGUAGE_SYNTAX_ERROR(descent) {			 \
	TREE_ERROR_MESSAGE(TREE_LANGUAGE_SYNTAX_ERROR);	\
	return LanguageSyntaxError(descent);			\
}

struct FrontedDescent {
	Lexer* lexer;
	size_t* pc;
	IdNameTable* id_name_table;
	int cur_scope;
};

BinaryTreeStatusCode CreateTreeFromFile(Tree* tree, IdNameTable* id_name_table, Lexer* lexer);
Node_t* LanguageSyntaxError(FrontedDescent* descent);
