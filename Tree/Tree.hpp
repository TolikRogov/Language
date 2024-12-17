#pragma once

#include "Project_utils.hpp"
#include "Tree_defines.hpp"

typedef double Number_t;

#include "KeyWords.hpp"

#define INIT_TREE(tree) Tree tree = {.info = {.name = #tree, .file_name = __FILE__, .line = __LINE__}};

#define BINARY_TREE_GRAPH_DUMP(tree, func_name, point, id_name_table) {	 																	\
	tree_status = BinaryTreeGraphDump(tree, {.file = __FILE__, .line = __LINE__, .func = func_name, .pointer = point}, id_name_table);		\
	TREE_ERROR_CHECK(tree_status);																	 					   				   \
}

#define TREE_HTML_DUMP_START() {				 \
	tree_status = BinaryTreeHtmlDumpStart();	\
	TREE_ERROR_CHECK(tree_status);				\
}

#define TREE_HTML_DUMP_FINISH() {				 \
	tree_status = BinaryTreeHtmlDumpFinish();	\
	TREE_ERROR_CHECK(tree_status);				\
}

#ifdef PRINT_DEBUG
	#define TREE_CTOR(tree, id_name_table) {						 		 \
		tree_status = TreeCtor(tree);										\
		TREE_ERROR_CHECK(tree_status);										\
		BINARY_TREE_GRAPH_DUMP(tree, "TreeCtor", NULL, id_name_table);		\
	}
#else
	#define TREE_CTOR(tree, id_name_table) {				 \
		tree_status = TreeCtor(tree);						\
		TREE_ERROR_CHECK(tree_status);						\
	}
#endif

#define TREE_DTOR(tree) {						 	 	 \
	TREE_HTML_DUMP_FINISH();							\
	tree_status = TreeDtor((tree)->root);				\
	TREE_ERROR_CHECK(tree_status);						\
}

enum TreeDumpCheck {
	TREE_FIRST_RUN,
	TREE_SECOND_RUN,
	TREE_ALREADY_ON,
};

enum NodeType {
	UNW  				= -1,
	NUMBER 				=  1,
	IDENTIFIER 			=  2,
	KEYWORD				=  3,
	FUNCTION_DEFINITION	=  4,
	PARAMETERS 			=  5,
	VAR_DECLARATION 	=  6,
	CALL 				=  7,
};

union Data_t {
	Number_t 	val_num;
	size_t 		val_id;
	KeyWordNum 	val_key_word;
	size_t 		val_func_def;
	size_t 		val_decl_var;
};

struct Node_t {
	NodeType type;
	union Data_t data;
	Node_t* left;
	Node_t* right;
	Node_t* parent;
};

struct TreeLogInfo {
	const char* name;
	const char* file_name;
	const size_t line;
};

struct Tree {
	Node_t* root;
	TreeLogInfo info;
};

const Number_t UNKNOWN_WHAT = -666;

BinaryTreeStatusCode TreeCtor(Tree* tree);
BinaryTreeStatusCode TreeDtor(Node_t* node);

BinaryTreeStatusCode BinaryTreeHtmlDumpStart();
BinaryTreeStatusCode BinaryTreeHtmlDumpFinish();

BinaryTreeStatusCode IsRootUnknownWhat(Node_t* root);
Node_t* CreateNode(NodeType type, Data_t data, Node_t* left, Node_t* right, Node_t* parent);
Node_t* FindTreeRoot(Node_t* node);
BinaryTreeStatusCode NodeParentChecker(Node_t* node);
BinaryTreeStatusCode SetNodeValue(Node_t* node, Data_t data);
BinaryTreeStatusCode NodePrintData(Node_t* node);
const char* GetNodeType(Node_t* node);
BinaryTreeStatusCode ReplaceUnknownWhat(Node_t* node, Data_t data, NodeType type);
