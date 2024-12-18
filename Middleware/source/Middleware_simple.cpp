#include "Middleware_simple.hpp"

Node_t* doDifferentiation(Node_t* node, IdNameTable* id_name_table) {

	if (!node)
		return NULL;

	Node_t* new_node = NULL;

	switch (node->type) {
		case NUMBER: 		return _NUM(0);
		case IDENTIFIER: 	return _NUM(1);
		case KEYWORD: {
			switch (node->data.val_key_word) {
				case ADD: 	{ new_node = _ADD(dL, dR); break; }
				case SUB: 	{ new_node = _SUB(dL, dR); break; }
				case MUL: 	{ new_node = _ADD(_MUL(dL, cR), _MUL(cL, dR)); break; }
				case DIV: 	{ new_node = _DIV(_SUB(_MUL(dL, cR), _MUL(cL, dR)), _POW(cR, _NUM(2))); break; }
				case POW:
				{
					size_t left_subtree = NumberOfVariablesInSubtree(node->left, id_name_table);
					size_t right_subtree = NumberOfVariablesInSubtree(node->right, id_name_table);

					if (left_subtree && !right_subtree) { new_node = _MUL(_MUL(cR, _POW(cL, _SUB(cR, _NUM(1)))), dL); break; }
					if (right_subtree && !left_subtree) { new_node = cP; break; }
					if (left_subtree && right_subtree)  { new_node = cP; break; }
				}
				case SIN: 	{ new_node = _MUL(_COS(NULL, cL), dL); break; }
				case COS: 	{ new_node = _MUL(_MUL( _NUM(-1), _SIN(NULL, cL)), dL); break; }
				case SQRT:	{ new_node = _MUL(_DIV(_NUM(1), _MUL(_NUM(2), _SQRT(NULL, cL))), dL); break; }
				default: return NULL;
			}
			Simplification(new_node, id_name_table);
			return new_node;
		}
		default: return NULL;
	}
}

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

BinaryTreeStatusCode Simplification(Node_t* subtree_root, IdNameTable* id_name_table) {

	size_t count_of_changes = 0;

	do {
		count_of_changes = 0;
		ConvolutionConstant(subtree_root, &count_of_changes, id_name_table);
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

#define NUMBER_AS_RESULT(event_pos, event, result) {									 				 	 					 \
	if ((node->event_pos && node->event_pos->type == NUMBER && DiffCompareDouble(node->event_pos->data.val_num, event))) {		\
		node->type = NUMBER;																									\
		node->data.val_num = result;																							\
		TreeDtor(node->left); TreeDtor(node->right);																			\
		node->left = node->right = NULL;																						\
		(*count_of_changes)++;																									\
		return TREE_NO_ERROR;																									\
	}																															\
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
					if (node->right) REBINDING(left, right, 0, node->right->data);
					if (node->left) REBINDING(right, left, 0, node->left->data);
					break;
				}
				case SUB: {
					if (node->left) REBINDING(right, left, 0, node->left->data);
					CHANGE_OPERATION(left, 0, MUL, -1);
					break;
				}
				case DIV: {
					if (node->left) REBINDING(right, left, 1, node->left->data);
					NUMBER_AS_RESULT(left, 0, 0);
					break;
				}
				case MUL: {
					if (node->right) REBINDING(left, right, 1, node->right->data);
					if (node->left) REBINDING(right, left, 1, node->left->data);
					NUMBER_AS_RESULT(left, 0, 0);
					NUMBER_AS_RESULT(right, 0, 0);
					break;
				}
				case POW: {
					if (node->right) REBINDING(left, right, 1, node->right->data);
					if (node->left) REBINDING(right, left, 1, node->left->data);
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

int ConvolutionConstant(Node_t* node, size_t* count_of_changes, IdNameTable* id_name_table) {

	if (!node)
		return TREE_NO_ERROR;

	int cnt = 0;
	if (node->left)  cnt += ConvolutionConstant(node->left, count_of_changes, id_name_table);
	if (node->right) cnt += ConvolutionConstant(node->right, count_of_changes, id_name_table);

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
				case DIFF: {
					Node_t* diff_subtree = doDifferentiation(node->right, id_name_table);
					if (node->parent->right == node) node->parent->right = diff_subtree;
					if (node->parent->left == node)	 node->parent->left  = diff_subtree;
					TreeDtor(node);
				}
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
