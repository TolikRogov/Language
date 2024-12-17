#pragma once

#include "IdNameTable.hpp"
#include "Tree_dump.hpp"
#include "Standard.hpp"
#include "Lexer.hpp"

BinaryTreeStatusCode ReadTreeStandard(Tree* tree, IdNameTable* id_name_table, Lexer* lexer);
