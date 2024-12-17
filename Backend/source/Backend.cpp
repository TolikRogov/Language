#include "Backend.hpp"

BinaryTreeStatusCode AssembleProgram(Tree* tree) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	FILE* asm_file = fopen(ASM_FILE_, "w");
	if (!asm_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);


	if (fclose(asm_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	return TREE_NO_ERROR;
}

