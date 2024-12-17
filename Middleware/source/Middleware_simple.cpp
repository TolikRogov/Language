#include "Middleware_simple.hpp"

Number_t Eval(Node_t* node) {

	if (!node)
		TREE_ERROR_MESSAGE(TREE_NULL_POINTER);

	switch (node->type) {
		case NUMBER: 		return node->data.val_num;
		case KEYWORD: {
			switch (node->data.val_key_word) {
				case ADD: 	return eL + eR;
				case SUB: 	return eL - eR;
				case MUL: 	return eL * eR;
				case DIV: 	{
					Number_t denominator = eR;
					if (!DiffCompareDouble(denominator, 0))
						return eL / denominator;
					else { TREE_ERROR_MESSAGE(TREE_LANGUAGE_SYNTAX_ERROR); return 0; }
				}
				case SIN: 	return sin(eL);
				case COS: 	return cos(eL);
				case SQRT:  return sqrt(eL);
				case POW:	return pow(eL, eR);
				default: break;
			}
		}
		default:  	return 0;
	}
}

BinaryTreeStatusCode Simplification(Node_t* subtree_root) {

	size_t count_of_changes = 0;

	do {
		count_of_changes = 0;
		ConvolutionConstant(subtree_root, &count_of_changes);
		TrivialTransformations(subtree_root, &count_of_changes);
	} while (count_of_changes);

	return TREE_NO_ERROR;
}

int TrivialTransformations(Node_t* node, size_t* count_of_changes) {

	if (!node)
		return TREE_NO_ERROR;

	if (node->left) TrivialTransformations(node->left, count_of_changes);
	if (node->right) TrivialTransformations(node->right, count_of_changes);

#define REBINDING(from, to, event, new_data) {									 				 \
	if (node->from->type == NUMBER && DiffCompareDouble(node->from->data.val_num, event)) {		\
		node->type = node->to->type;															\
		SetNodeValue(node, new_data);															\
		Node_t* left = doCopySubtree(node->to->left);											\
		Node_t* right = doCopySubtree(node->to->right);											\
		TreeDtor(node->left); TreeDtor(node->right);											\
		node->left = left;	  node->right = right;												\
		(*count_of_changes)++;																	\
		return TREE_NO_ERROR;																	\
	}																							\
}

#define NUMBER_AS_RESULT(event_pos, event, result) {									 				 	 \
	if ((node->event_pos->type == NUMBER && DiffCompareDouble(node->event_pos->data.val_num, event))) {		\
		node->type = NUMBER;																				\
		node->data.val_num = result;																		\
		TreeDtor(node->left); TreeDtor(node->right);														\
		node->left = node->right = NULL;																	\
		(*count_of_changes)++;																				\
		return TREE_NO_ERROR;																				\
	}																										\
}

#define CHANGE_OPERATION(event_pos, event, new_op, new_data) {												 \
	if ((node->event_pos->type == NUMBER && DiffCompareDouble(node->event_pos->data.val_num, event))) {		\
		node->data.val_key_word = new_op;																	\
		node->event_pos->data.val_num = new_data;															\
		(*count_of_changes)++;																				\
		return TREE_NO_ERROR;																				\
	}																										\
}

	switch (node->type) {
		case KEYWORD: {
			switch (node->data.val_key_word) {
				case ADD: {
					REBINDING(left, right, 0, node->right->data);
					REBINDING(right, left, 0, node->left->data);
					break;
				}
				case SUB: {
					REBINDING(right, left, 0, node->left->data);
					CHANGE_OPERATION(left, 0, MUL, -1);
					break;
				}
				case DIV: {
					REBINDING(right, left, 1, node->left->data);
					NUMBER_AS_RESULT(left, 0, 0);
					break;
				}
				case MUL: {
					REBINDING(left, right, 1, node->right->data);
					REBINDING(right, left, 1, node->left->data);
					NUMBER_AS_RESULT(left, 0, 0);
					NUMBER_AS_RESULT(right, 0, 0);
					break;
				}
				case POW: {
					REBINDING(left, right, 1, node->right->data);
					REBINDING(right, left, 1, node->left->data);
					NUMBER_AS_RESULT(right, 0, 1);
					NUMBER_AS_RESULT(left, 1, 1);
					break;
				}
				default: return TREE_NO_ERROR;
			}
			break;
		}
		case NUMBER:
		case IDENTIFIER:
		case FUNCTION_DEFINITION:
		case PARAMETERS:
		case VAR_DECLARATION:
		case CALL:
		case UNW:
		default: return TREE_NO_ERROR;
	}
#undef REBINDING
#undef NUMBER_AS_RESULT
#undef CHANGE_OPERATION

	return TREE_NO_ERROR;
}

int ConvolutionConstant(Node_t* node, size_t* count_of_changes) {

	if (!node)
		return TREE_NO_ERROR;

	int cnt = 0;
	if (node->left)  cnt += ConvolutionConstant(node->left, count_of_changes);
	if (node->right) cnt += ConvolutionConstant(node->right, count_of_changes);

	switch (node->type) {
		case NUMBER:		return 1;
		case KEYWORD:
		{
			switch (node->data.val_key_word) {
				case ADD:
				case SUB:
				case MUL:
				case POW:
				case DIV:	break;
				default: 	return 0;
			}

			if (cnt > 1) {
				Number_t result = Eval(node);
				node->type = NUMBER;
				SetNodeValue(node, {.val_num = result});
				if (node->right) { TreeDtor(node->right); node->right = NULL; }
				if (node->left)  { TreeDtor(node->left);  node->left = NULL;  }
				(*count_of_changes)++;
				return 1;
			}
		}
		case IDENTIFIER:
		case FUNCTION_DEFINITION:
		case PARAMETERS:
		case VAR_DECLARATION:
		case CALL:
		case UNW:
		default: return 0;
	}
}
