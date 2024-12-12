#include "Frontend_utils.hpp"

BinaryTreeStatusCode PrintNString(FILE* stream, const char* string, size_t n) {

	for (size_t i = 0; i < n; i++)
		fprintf(stream, "%c", string[i]);

	return TREE_NO_ERROR;
}

const char* BinaryTreeErrorsMessenger(BinaryTreeStatusCode status) {
	switch(status) {
		case TREE_NO_ERROR:						return "TREE ERROR - NO ERROR";
		case TREE_ALLOC_ERROR:					return "TREE ERROR - MEMORY ALLOCATION ERROR";
		case TREE_NULL_POINTER:					return "TREE ERROR - POINTER IS NULL";
		case TREE_READ_ERROR:					return "TREE ERROR - ERROR READ FROM FILE";
		case TREE_INVALID_TYPE:					return "TREE ERROR - INVALID TYPE OF INPUT ELEMENT";
		case TREE_LANGUAGE_SYNTAX_ERROR:		return "TREE ERROR - INVALID SYNTAX OF LANGUAGE";
		case TREE_EXPRESSION_COMMENTS_ERROR:	return "TREE ERROR - THERE IS NOT SECOND COMMENT TERMINAL";
		case TREE_ROOT_IS_UNKNOWN:				return "TREE ERROR - ROOT IS UNKOWN";
		case TREE_NODE_NON_EXISTENT:			return "TREE ERROR - NODE IS NON EXISTENT";
		case TREE_NODE_ALREADY_EXISTENT:		return "TREE ERROR - NODE HAS ALREADY BEEN IN TREE";
		case TREE_NOTHING_TO_READ:				return "TREE ERROR - EXPRESSION FILE HASN'T READABLE INFORMATION";
		case TREE_FILE_OPEN_ERROR:				return "TREE ERROR - FILE WAS NOT OPENED";
		case TREE_FILE_CLOSE_ERROR:				return "TREE ERROR - FILE WAS NOT CLOSED";
		case TREE_DIR_CLOSE_ERROR:				return "TREE ERROR - DIRECTORY WAS NOT CLOSED";
		case TREE_UNDEFINED_ERROR:
		default: 								return "UNDEFINED ERROR";
	}
}

int StrCmp(const char* str1, const char* str2) {

	if (!str1 || !str2)
		return -1;

	size_t i = 0;
	for (; *(str1 + i) != '\0' && *(str2 + i) != '\0'; i++) {
		if (*(str1 + i) != *(str2 + i))
			break;
	}
	return (*(str1 + i) - *(str2 + i));
}

int DiffCompareDouble(double var1, double var2) {
	return (fabs(var1 - var2) <= DIFF_EPS);
}

int StrLen(const char* str) {

	if (!str)
		return -1;

	int i = 0;
	for (; *(str + i) != '\0'; i++) {}

	return i;
}
