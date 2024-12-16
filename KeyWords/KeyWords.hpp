#pragma once

#include "Project_utils.hpp"

enum KeyWordNum {
	INVALID_KEY_WORD 	= -1,
	CLOSE_ROUND		 	= 1,
	OPEN_FIGURE		 	= 2,
	CLOSE_FIGURE	 	= 3,
	ELSE 			 	= 5,
	POW 		 	 	= 6,
	ENDOF			 	= 7,
	OPEN_ROUND		 	= 9,
	IF 				 	= 11,
	WHILE 			 	= 12,
	ASSIGNMENT 		 	= 13,
	SIN 			 	= 21,
	COS  			 	= 22,
	FLOOR				= 23,
	ADD				 	= 24,
	SUB   			 	= 25,
	MUL				 	= 26,
	DIV				 	= 27,
	SQRT			 	= 29,
	SEQUENTIAL_OP	 	= 41,
	COMMA_OP		 	= 42,
	INIT_TYPE		 	= 51,
	INPUT 			 	= 61,
	PRINTF 			 	= 62,
	RETURN 			 	= 71,
	BREAK 			 	= 72,
	CONTINUE 		 	= 73,
	AMOUNT_OF_KEYWORDS,
};

struct KeyWord {
	KeyWordNum num;
	const char* string;
};

extern KeyWord keywords[];
