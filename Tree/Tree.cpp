#include "Tree.hpp"

wchar_t TREE_DUMP = TREE_FIRST_RUN;

BinaryTreeStatusCode TreeCtor(Tree* tree) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	tree->root = CreateNode(UNW, {.val_num = UNKNOWN_WHAT}, NULL, NULL, NULL);

	if (TREE_DUMP == TREE_ALREADY_ON)
		return TREE_NO_ERROR;

	TREE_HTML_DUMP_START();
	TREE_DUMP = TREE_ALREADY_ON;

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode TreeDtor(Node_t* node) {

	if (node->left)  TreeDtor(node->left);
	if (node->right) TreeDtor(node->right);

	if (node) {
		free(node);
		node = NULL;
	}

	return TREE_NO_ERROR;
}

Node_t* doCopySubtree(Node_t* node) {

	if (!node)
		return NULL;

	switch (node->type) {
		case NUMBER: 					return _NUM(node->data.val_num);
		case IDENTIFIER: 				return _ID(node->data.val_id);
		case FUNCTION_DEFINITION:		return _FUNC_DEF(node->data.val_func_def, cL, cR);
		case PARAMETERS:				return _PARAMETERS(cL, cR);
		case VAR_DECLARATION:			return _VAR_DEC(node->data.val_decl_var, cL, cR);
		case CALL:						return _CALL(cL, cR);
		case KEYWORD:					return _KEYWORD(node->data.val_key_word, cL, cR);
		case UNW:
		default: return NULL;
	}
}

Node_t* CreateNode(NodeType type, Data_t data, Node_t* left, Node_t* right, Node_t* parent) {

	Node_t* node = (Node_t*)calloc(1, sizeof(*node));
	node->type = type;

	switch (type) {
		case UNW:
		case NUMBER: 				{ node->data.val_num	 	= data.val_num; 		break; }
		case IDENTIFIER:  			{ node->data.val_id  		= data.val_id;  		break; }
		case KEYWORD:  				{ node->data.val_key_word 	= data.val_key_word;  	break; }
		case FUNCTION_DEFINITION:  	{ node->data.val_func_def 	= data.val_func_def;  	break; }
		case VAR_DECLARATION:  		{ node->data.val_decl_var 	= data.val_decl_var;  	break; }
		case CALL:
		case PARAMETERS:
		default: break;
	}

	if (left)   { node->left   = left;   	left->parent = node;  }
	if (right)  { node->right  = right; 	right->parent = node; }
	if (parent) { node->parent = parent; }

	return node;
}

BinaryTreeStatusCode IsRootUnknownWhat(Node_t* root) {

	if (!root)
		return TREE_NO_ERROR;

	if (root->left == NULL && \
		root->right == NULL && \
		root->type == UNW && \
		DiffCompareDouble(root->data.val_num, UNKNOWN_WHAT))
		return TREE_ROOT_IS_UNKNOWN;

	return TREE_NO_ERROR;
}

Node_t* FindTreeRoot(Node_t* node) {

	if (!node)
		return NULL;

	if (!node->parent)
		return node;

	return FindTreeRoot(node->parent);
}

BinaryTreeStatusCode NodePrintData(Node_t* node) {

	if (!node)
		TREE_ERROR_CHECK(TREE_NULL_POINTER);

	switch (node->type) {
		case UNW: 					{ printf("%s\n", "UNKOWN WHAT"); 											break; }
		case PARAMETERS:			{ printf("%s\n", "PARAMETERS");												break; }
		case CALL:					{ printf("%s\n", "CALL");													break; }
		case VAR_DECLARATION:		{ printf(GREEN("%zu") "\n", node->data.val_decl_var);						break; }
		case NUMBER:				{ printf(GREEN("%lg") "\n", node->data.val_num); 							break; }
		case IDENTIFIER:			{ printf(GREEN("%zu") "\n", node->data.val_id);  							break; }
		case KEYWORD:				{ printf(GREEN("%ls") "\n",  KeyWordsGetString(node->data.val_key_word)); 	break; }
		case FUNCTION_DEFINITION:	{ printf(GREEN("%zu") "\n", node->data.val_func_def);						break; }
		default: TREE_ERROR_CHECK(TREE_INVALID_TYPE);
	}

	return TREE_NO_ERROR;
}

const wchar_t* GetNodeType(Node_t* node) {

	if (!node)
		return NULL;

	switch (node->type) {
		case UNW: 					return L"UNW";
		case PARAMETERS:			return L"PARAMETERS";
		case CALL:					return L"CALL";
		case VAR_DECLARATION:		return L"VAR_DECLARATION";
		case NUMBER:				return L"NUMBER";
		case IDENTIFIER:			return L"IDENTIFIER";
		case KEYWORD:				return L"KEYWORD";
		case FUNCTION_DEFINITION:	return L"FUNCTION_DEFINITION";
		default: return NULL;
	}
}

BinaryTreeStatusCode ReplaceUnknownWhat(Node_t* node, Data_t data, NodeType type) {

	switch (type) {
		case PARAMETERS:			{ node->type = PARAMETERS;															break; }
		case CALL:					{ node->type = CALL; 																break; }
		case VAR_DECLARATION:		{ node->type = VAR_DECLARATION; 	node->data.val_decl_var = data.val_decl_var;	break; }
		case NUMBER:				{ node->type = NUMBER; 				node->data.val_num = data.val_num; 				break; }
		case IDENTIFIER:			{ node->type = IDENTIFIER; 			node->data.val_id = data.val_id;  				break; }
		case KEYWORD:				{ node->type = KEYWORD; 			node->data.val_key_word = data.val_key_word; 	break; }
		case FUNCTION_DEFINITION:	{ node->type = FUNCTION_DEFINITION; node->data.val_func_def = data.val_func_def;	break; }
		case UNW:
		default: TREE_ERROR_CHECK(TREE_INVALID_TYPE);
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode SetNodeValue(Node_t* node, Data_t data) {

	if (!node)
		TREE_ERROR_CHECK(TREE_NULL_POINTER);

	switch(node->type) {
		case VAR_DECLARATION:		{ node->data.val_decl_var = data.val_decl_var;	break; }
		case NUMBER:				{ node->data.val_num 	  = data.val_num; 		break; }
		case IDENTIFIER:			{ node->data.val_id 	  = data.val_id;  		break; }
		case KEYWORD:				{ node->data.val_key_word = data.val_key_word; 	break; }
		case FUNCTION_DEFINITION:	{ node->data.val_func_def = data.val_func_def;	break; }
		case PARAMETERS:
		case CALL:
		case UNW:
		default: return TREE_NO_ERROR;
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode NodeParentChecker(Node_t* node) {

	if (!node)
		return TREE_NO_ERROR;

	if (node->left)  {
		if (!node->left->parent)
			printf(RED("Node[%p] has no parent!")"\n", node->left);
		NodeParentChecker(node->left);
	}

	if (node->right)  {
		if (!node->right->parent)
			printf(RED("Node[%p] has no parent!")"\n", node->right);
		NodeParentChecker(node->right);
	}

	return TREE_NO_ERROR;
}
