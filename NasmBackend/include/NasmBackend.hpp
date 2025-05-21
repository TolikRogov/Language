#pragma once

#include "Standard_reader.hpp"
#include "array.hpp"

#define ASM_FILE_ "nasm.asm"

struct Backend {
	IdNameTable* id_name_table;
	FILE* asm_file;
	size_t tabs;
	size_t cnt_if;
	size_t cnt_while;
	int cur_scope;
};

BinaryTreeStatusCode RunBackend(Tree* tree, IdNameTable* id_name_table);
BinaryTreeStatusCode WriteAssembleCode(Node_t* node, Backend* backend);

const char* GetCommentByKeyWordType(KeyWordNum key_word_num);
Commands GetCmdByKeyWordType(KeyWordNum key_word_num);
