#pragma once

#include "Standard_reader.hpp"
#include "array.hpp"

#define PROC_DATA_DIR_ 	"../Processor/data/"
#define ASM_FILE_		PROC_DATA_DIR_ "main.asm"

BinaryTreeStatusCode RunBackend(Tree* tree, IdNameTable* id_name_table);
BinaryTreeStatusCode WriteAssembleCode(Node_t* node, IdNameTable* id_name_table, FILE* asm_file);
