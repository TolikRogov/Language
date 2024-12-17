#pragma once

#include "Tree.hpp"

#define INIT_ID_NAME_TABLE(id_name_table) IdNameTable id_name_table = {};

#define ID_NAME_TABLE_CTOR(id_name_table) {			 	 \
	tree_status = IdNameTableCtor(id_name_table);		\
	TREE_ERROR_CHECK(tree_status);						\
}

#define ID_NAME_TABLE_DTOR(id_name_table) {			 	 \
	tree_status = IdNameTableDtor(id_name_table);		\
	TREE_ERROR_CHECK(tree_status);						\
}

#define ID_NAME_TABLE_REALLOC(id_name_table) {		 	 \
	tree_status = IdNameTableRealloc(id_name_table);	\
	TREE_ERROR_CHECK(tree_status);						\
}

enum IdType {
	ID_UNW		= -1,
	ID_VAR 		= 1,
	ID_FUNCTION = 2,
};

struct ScopeVariables {
	int* data;
	size_t size;
	size_t capacity;
};

struct Identifier  {
	int num;
	IdType type;
	char* string;
	size_t length;
	size_t define_status;
	size_t global;
	ScopeVariables scope_variables;
};

struct IdNameTable {
	Identifier* data;
	size_t capacity;
	size_t size;
	char* buffer;
};

const size_t DEFAULT_COUNT_LOCAL_VARIABLES = 10;
const size_t DEFAULT_ID_NAME_TABLE_CAPACITY = 1;

BinaryTreeStatusCode IdNameTableRealloc(IdNameTable* var_name_table);
BinaryTreeStatusCode IdNameTableCtor(IdNameTable* var_name_table);
BinaryTreeStatusCode IdNameTableDtor(IdNameTable* var_name_table);
BinaryTreeStatusCode PrintIdNameTable(IdNameTable* id_name_table);

int IdNameTableGetIdNumber(IdNameTable* id_name_table, const char* string, size_t length);
const char* IdNameTableGetIdTypeByType(IdType type);
int FindLocalVariableInScope(Identifier* scope, Identifier* var);
BinaryTreeStatusCode ScopeLocalVariablesRealloc(Identifier* scope);
int CountOfGlobalVariables(IdNameTable* id_name_table);
