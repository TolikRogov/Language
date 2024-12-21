#pragma once

#include "Project_utils.hpp"

#define TOC '#'

enum KeyWordNum {
	STD_CLOSE_ROUND		= -4,
	STD_OPEN_ROUND		= -3,
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
	DIFF				= 28,
	SQRT			 	= 29,
	EQUAL 				= 31,
	BELOW				= 32,
	ABOVE				= 33,
	BELOW_EQUAL			= 34,
	ABOVE_EQUAL			= 35,
	NOT_EQUAL			= 36,
	AND					= 37,
	OR					= 38,
	NOT					= 39,
	SEQUENTIAL_OP	 	= 41,
	COMMA_OP		 	= 42,
	INIT_TYPE		 	= 51,
	INPUT 			 	= 61,
	PRINTF 			 	= 62,
	RETURN 			 	= 71,
	BREAK 			 	= 72,
	CONTINUE 		 	= 73,
	ABORT				= 74,
	LEX_ALL,
	LEX_FORGET,
	LEX_JUMPED,
	LEX_CO,
	LEX_MOUNTAIN,
	LEX_WILD,
	LEX_SCREAM,
	LEX_EMIT,
	LEX_BUT,
	LEX_HAPPENED,
	LEX_NO,
	LEX_SO,
	LEX_ON,
	LEX_DINNER,
	LEX_INVITED,
	LEX_CAME_IN,
	LEX_IN,
	LEX_DEADLOCK,
	LEX_ONE,
	LEX_LEAST,
	LEX_ONLY,
	LEX_PATH,
	LEX_WENT,
	LEX_TO_HOME,
	LEX_RUMBLES,
	LEX_THUNDER,
	LEX_SPARKLES,
	LEX_LIGHTNING,
	LEX_EDGE,
	LEX_HILL,
	LEX_WAS_FALLING,
	LEX_NIGHT,
	LEX_HAPPINESS,
	LEX_GAIN,
	LEX_TIME,
	LEX_LESS,
	LEX_DEVOTES,
	LEX_INTERESTING,
	LEX_HUGE,
	LEX_FOUND,
	LEX_DONT_ENOUGH,
	LEX_POWER,
	LEX_VERY,
	LEX_STUBBORN,
	LEX_DIFFERENCE,
	LEX_FUNNY,
	LEX_ADVICE,
	LEX_HE,
	LEX_KNOW,
	LEX_VO,
	LEX_DARKNESS,
	LEX_TO_NIGHT,
	LEX_THROUGH,
	LEX_FOREST,
	LEX_WANDER,
	LEX_KOL,
	LEX_WANT,
	LEX_WILL_TELL,
	LEX_NOTHING,
	LEX_LIFE,
	LEX_GET_BACK,
	LEX_ROBBERS,
	LEX_CAME_OUT,
	LEX_CROWD,
	LEX_CLOSE,
	LEX_EYES,
	LEX_FELL_DOWN,
	LEX_DEAD,
	AMOUNT_OF_KEYWORDS,
};

struct KeyWord {
	KeyWordNum num;
	const wchar_t* string;
};

const wchar_t* KeyWordsGetString(KeyWordNum key_word_number);
KeyWordNum KeyWordsGetKeyWordNum(const wchar_t* string);

extern KeyWord keywords[];
