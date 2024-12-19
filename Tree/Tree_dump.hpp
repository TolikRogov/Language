#pragma once

#include "IdNameTable.hpp"

#define DUMP_DIR_ 		"dump/"
#define IMG_DIR_		"img/"

#define DOT_FILE_ 		DUMP_DIR_ "graph.dot"
#define IMG_FILE_  		IMG_DIR_  "graph"
#define HTML_FILE_ 		DUMP_DIR_ "tree.html"
#define CSS_FILE_		DUMP_DIR_ "styles.css"
#define BASH_FILE_		DUMP_DIR_ "script.bash"

#define IMG_EXTENSION 	"svg"

#ifdef PRINT_DEBUG
	#define OPEN_HTML_FILE() system(OPEN HTML_FILE_);
#else
	#define OPEN_HTML_FILE()
#endif

#ifdef UNKNOWN_WHAT_IN_TREE
	#define UNKNOWN_WHAT_(...) __VA_ARGS__
#else
	#define UNKNOWN_WHAT_(...) NULL
#endif

#define NAME_TABLE_PRINT(id_name_table) {				 \
	tree_status = IdNameTablePrint(id_name_table); 		\
	TREE_ERROR_CHECK(tree_status);						\
}

struct DumpLogInfo {
	const char* file;
	const size_t line;
	const char* func;
	const Node_t* pointer;
};

BinaryTreeStatusCode BinaryTreeCssFile();
BinaryTreeStatusCode BinaryTreeCreateDumpDir();
BinaryTreeStatusCode BinaryTreeBashScript(Tree* tree, DumpLogInfo* dump_info);
BinaryTreeStatusCode NodeGraphDump(Node_t* cur_root, FILE* dot_file, DumpLogInfo* dump_info, IdNameTable* id_name_table);
BinaryTreeStatusCode KeyWordsTablePrint();
BinaryTreeStatusCode IdNameTablePrint(IdNameTable* id_name_table);
BinaryTreeStatusCode ColorLegendPrint();
BinaryTreeStatusCode BinaryTreeGraphDump(Tree* tree, DumpLogInfo dump_info, IdNameTable* id_name_table);
