#pragma once

#define _NUM(val) 								CreateNode(NUMBER, 					{.val_num 		= val}, 			NULL, NULL,  NULL)
#define _ID(id_num) 							CreateNode(IDENTIFIER, 				{.val_id  		= id_num}, 			NULL, NULL,  NULL)
#define _FUNC_DEF(id_func_num, left, right) 	CreateNode(FUNCTION_DEFINITION, 	{.val_func_def  = id_func_num}, 	left, right, NULL)
#define _PARAMETERS(left, right) 				CreateNode(PARAMETERS, 				{}, 								left, right, NULL)
#define _VAR_DEC(var_id, left, right) 			CreateNode(VAR_DECLARATION, 		{.val_decl_var	= var_id}, 			left, right, NULL)
#define _CALL(left, right) 						CreateNode(CALL, 					{},					 				left, right, NULL)

#define _ELSE(left, right)						CreateNode(KEYWORD,					{.val_key_word = ELSE},				left, right, NULL)
#define _POW(left, right) 						CreateNode(KEYWORD, 				{.val_key_word = POW}, 				left, right, NULL)
#define _IF(left, right) 						CreateNode(KEYWORD, 				{.val_key_word = IF}, 				left, right, NULL)
#define _WHILE(left, right) 					CreateNode(KEYWORD, 				{.val_key_word = WHILE},			left, right, NULL)
#define _ASSIGNMENT(left, right) 				CreateNode(KEYWORD, 				{.val_key_word = ASSIGNMENT}, 		left, right, NULL)
#define _SIN(left) 								CreateNode(KEYWORD,  				{.val_key_word = SIN}, 				left, NULL,  NULL)
#define _COS(left) 								CreateNode(KEYWORD,  				{.val_key_word = COS}, 				left, NULL,  NULL)
#define _ADD(left, right) 						CreateNode(KEYWORD,  				{.val_key_word = ADD}, 				left, right, NULL)
#define _SUB(left, right) 						CreateNode(KEYWORD,  				{.val_key_word = SUB}, 				left, right, NULL)
#define _MUL(left, right) 						CreateNode(KEYWORD,  				{.val_key_word = MUL}, 				left, right, NULL)
#define _DIV(left, right) 						CreateNode(KEYWORD,  				{.val_key_word = DIV}, 				left, right, NULL)
#define _SQRT(left)								CreateNode(KEYWORD,  				{.val_key_word = SQRT}, 			left, NULL,  NULL)
#define _SEQUENTIAL_OP(left, right) 			CreateNode(KEYWORD,  				{.val_key_word = SEQUENTIAL_OP}, 	left, right, NULL)
#define _COMMA_OP(left, right) 					CreateNode(KEYWORD,  				{.val_key_word = COMMA_OP}, 		left, right, NULL)
#define _INIT_TYPE() 							CreateNode(KEYWORD,  				{.val_key_word = INIT_TYPE}, 		NULL, NULL,  NULL)
#define _INPUT() 								CreateNode(KEYWORD,  				{.val_key_word = INPUT}, 			NULL, NULL,  NULL)
#define _PRINTF(left) 							CreateNode(KEYWORD,  				{.val_key_word = PRINTF}, 			left, NULL,  NULL)
#define _RETURN(left) 							CreateNode(KEYWORD,  				{.val_key_word = RETURN}, 			left, NULL,  NULL)
#define _BREAK() 								CreateNode(KEYWORD,  				{.val_key_word = BREAK}, 			NULL, NULL,  NULL)
#define _CONTINUE()								CreateNode(KEYWORD,  				{.val_key_word = CONTINUE}, 		NULL, NULL,  NULL)
