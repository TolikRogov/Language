#include "ReverseFrontend.hpp"
#include "KeyWordsTable.hpp"

int main() {

	setlocale(LC_ALL, "");

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	INIT_LEXER(lexer);
	LEXER_CTOR(&lexer);

	INIT_ID_NAME_TABLE(id_name_table);
	ID_NAME_TABLE_CTOR(&id_name_table);

	INIT_TREE(tree);
	TREE_CTOR(&tree, &id_name_table);

	STANDARD_READER(&tree, &id_name_table, &lexer);

	TREE_DTOR(&tree);
	ID_NAME_TABLE_DTOR(&id_name_table);
	LEXER_DTOR(&lexer);
	OPEN_HTML_FILE();

	return 0;
}
