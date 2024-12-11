#include "IdNameTable.hpp"
#include "Tree_dump.hpp"
#include "KeyWordsTable.hpp"

int main() {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	INIT_ID_NAME_TABLE(id_name_table);
	ID_NAME_TABLE_CTOR(&id_name_table);

	INIT_TREE(tree);
	TREE_CTOR(&tree, &id_name_table);

	ID_NAME_TABLE_DTOR(&id_name_table);
	TREE_DTOR(&tree);

	OPEN_HTML_FILE();

	return 0;
}
