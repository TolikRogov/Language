#pragma once

#include "IdNameTable.hpp"

#define INIT_LEXER(lexer) Lexer lexer = {};

#define LEXICAL_ANALYSIS(buffer, lexer, id_name_table, size) {			 \
	tree_status = LexicalAnalysis(buffer, lexer, id_name_table, size);	\
	TREE_ERROR_CHECK(tree_status);										\
}

#define SKIP_EXTRA(buffer, token_start, size) {				 \
	tree_status = SkipExtra(buffer, token_start, size);		\
	TREE_ERROR_CHECK(tree_status);							\
}

#define LEXER_CTOR(lexer) {					 \
	tree_status = LexerCtor(lexer);			\
	TREE_ERROR_CHECK(tree_status);			\
}

#define LEXER_DTOR(lexer) {					 \
	tree_status = LexerDtor(lexer);			\
	TREE_ERROR_CHECK(tree_status);			\
}

#define LEXER_REALLOC(lexer) {				 \
	tree_status = LexerRealloc(lexer);		\
	TREE_ERROR_CHECK(tree_status);			\
}

#define LEXER_PRINT(lexer, id_name_table) {				 \
	tree_status = PrintLexer(lexer, id_name_table);		\
	TREE_ERROR_CHECK(tree_status);						\
}

struct Token {
	NodeType type;
	union Data_t data;
	size_t index;
};

struct Lexer {
	Token* tokens;
	size_t capacity;
	size_t size;

	wchar_t* buffer;
	size_t buffer_size;
};

struct LexicalAnalysis_t {
	IdNameTable* id_name_table;
	size_t size;
	size_t* token_start;
	size_t* token_end;
	int* op_find;
	int* var_find;
	wchar_t** token_end_pointer;
};

const size_t LEXER_DEFAULT_CAPACITY = 4;

BinaryTreeStatusCode LexerCtor(Lexer* lexer);
BinaryTreeStatusCode LexerDtor(Lexer* lexer);
BinaryTreeStatusCode LexerRealloc(Lexer* lexer);
BinaryTreeStatusCode PrintLexer(Lexer* lexer, IdNameTable* id_name_table);
BinaryTreeStatusCode SkipExtra(const wchar_t* buffer, size_t* token_start, size_t size);
BinaryTreeStatusCode LexicalAnalysis(wchar_t* buffer, Lexer* lexer, IdNameTable* id_name_table, size_t size);
BinaryTreeStatusCode PrintTokenValue(Token* token, IdNameTable* id_name_table);

BinaryTreeStatusCode FindOperation(wchar_t* buffer, Lexer* lexer, LexicalAnalysis_t* lxa);
BinaryTreeStatusCode UnaryMinus(Lexer* lexer);
int GetNumber(wchar_t* buffer, Lexer* lexer, LexicalAnalysis_t* lxa);
BinaryTreeStatusCode FindVariable(wchar_t* buffer, Lexer* lexer, LexicalAnalysis_t* lxa);
BinaryTreeStatusCode AddNewVariable(wchar_t* buffer, Lexer* lexer, LexicalAnalysis_t* lxa);
BinaryTreeStatusCode ReplacingShift(Lexer* lexer, size_t i, size_t cnt);
BinaryTreeStatusCode ReplaceToken(Token* token, Data_t data);
BinaryTreeStatusCode SecondRun(Lexer* lexer);
BinaryTreeStatusCode LexerFillByEmpty(Lexer* lexer, size_t cnt);
size_t LexerFindSize(Lexer* lexer);
