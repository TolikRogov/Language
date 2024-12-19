#include "Backend.hpp"

#define MAIN "main"

BinaryTreeStatusCode RunBackend(Tree* tree, IdNameTable* id_name_table) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	FILE* asm_file = fopen(ASM_FILE_, "w");
	if (!asm_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

	tree_status = WriteAssembleCode(tree->root, id_name_table, asm_file);
	TREE_ERROR_CHECK(tree_status);

	if (fclose(asm_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode WriteAssembleCode(Node_t* node, IdNameTable* id_name_table, FILE* asm_file) {

	if (!node)
		return TREE_NO_ERROR;

	static size_t tabs = 0;

#define TABS { for (size_t i = 0; i < tabs; i++) {fprintf(asm_file, "\t");} }

	switch (node->type) {
		case KEYWORD: {
			switch (node->data.val_key_word) {
				case SEQUENTIAL_OP: {
					WriteAssembleCode(node->left, id_name_table, asm_file);
					fprintf(asm_file, "\n");
					WriteAssembleCode(node->right, id_name_table, asm_file);
					break;
				}
				case COMMA_OP: {
					break;
				}
				case ASSIGNMENT: {
					WriteAssembleCode(node->left, id_name_table, asm_file);
					TABS fprintf(asm_file, "%s [%d]\n", array_commands[CMD_POP].cmd_name, id_name_table->data[node->right->data.val_id].num);
					break;
				}
				case INPUT: {
					TABS fprintf(asm_file, "%s\n", array_commands[CMD_IN].cmd_name);
					break;
				}
				case PRINTF: {
					WriteAssembleCode(node->right, id_name_table, asm_file);
					TABS fprintf(asm_file, "%s\n", array_commands[CMD_OUT].cmd_name);
					break;
				}
				case ADD: {
					WriteAssembleCode(node->left, id_name_table, asm_file);
					WriteAssembleCode(node->right, id_name_table, asm_file);
					TABS fprintf(asm_file, "%s\n", array_commands[CMD_ADD].cmd_name);
					break;
				}
				case RETURN: {
					WriteAssembleCode(node->right, id_name_table, asm_file);
					TABS fprintf(asm_file, "%s\n", array_commands[CMD_RET].cmd_name);
					break;
				}
				default: return TREE_NO_ERROR;
			}
			break;
		}
		case FUNCTION_DEFINITION: {
			if (StrNCmp(MAIN, id_name_table->data[node->data.val_func_def].string, StrLen(MAIN), (int)id_name_table->data[node->data.val_func_def].length)) {
				TABS fprintf(asm_file, "%s %s:\n", array_commands[CMD_CALL].cmd_name, MAIN);
				TABS fprintf(asm_file, "%s\n\n", array_commands[CMD_HLT].cmd_name);
				TABS fprintf(asm_file, "%s:\n", MAIN);
				tabs++;
				WriteAssembleCode(node->right, id_name_table, asm_file);
				tabs--;
				break;
			}

			break;
		}
		case PARAMETERS: {
			WriteAssembleCode(node->left, id_name_table, asm_file);
			WriteAssembleCode(node->right, id_name_table, asm_file);
			return TREE_NO_ERROR;
		}
		case VAR_DECLARATION: {
			WriteAssembleCode(node->right, id_name_table, asm_file);
			break;
		}
		case NUMBER: {
			TABS fprintf(asm_file, "%s %lg\n", array_commands[CMD_PUSH].cmd_name, node->data.val_num);
			break;
		}
		case IDENTIFIER: {
			TABS fprintf(asm_file, "%s [%d]\n", array_commands[CMD_PUSH].cmd_name, id_name_table->data[node->data.val_id].num);
			break;
		}
		default: return TREE_NO_ERROR;
	}

#undef TABS

	return TREE_NO_ERROR;
}

