#pragma once

#include "Standard_reader.hpp"

#define REVFRONT_FILE_		DATA_DIR_ "reverse_main.txt"

struct ReverseFrontend {
	IdNameTable* id_name_table;
	FILE* prog_file;
	size_t tabs;
	int cur_scope;
};

BinaryTreeStatusCode RunReverseFrontend(Tree* tree, IdNameTable* id_name_table);
BinaryTreeStatusCode WriteProgram(Node_t* node, ReverseFrontend* revfront);
