#include "Lexer.hpp"

const size_t MAX_LENGTH_OF_TOKEN	= 6;

struct Lexeme {
	KeyWordNum array[MAX_LENGTH_OF_TOKEN];
	KeyWordNum changed;
};

Lexeme ReplaceTable[] = {{{ LEX_ALL, LEX_FORGET }, 										CLOSE_ROUND		},
							{{ LEX_JUMPED, LEX_CO, LEX_MOUNTAIN },      				OPEN_FIGURE		},
							{{ LEX_WILD, LEX_SCREAM, LEX_EMIT },       					CLOSE_FIGURE 	},
							{{ LEX_BUT, LEX_HAPPENED, LEX_ALL, LEX_NO, LEX_SO },       	ELSE 			},
							{{ LEX_ON, LEX_DINNER, LEX_INVITED },       				POW 			},
							{{ LEX_CAME_IN, LEX_IN, LEX_DEADLOCK },       				OPEN_ROUND 		},
							{{ LEX_ONE, LEX_LEAST, LEX_ONLY, LEX_PATH },       			IF 				},
							{{ LEX_WENT, LEX_TO_HOME },       							WHILE 			},
							{{ LEX_RUMBLES, LEX_THUNDER },       						SIN 			},
							{{ LEX_SPARKLES, LEX_LIGHTNING },       					COS				},
							{{ LEX_ON, LEX_EDGE },       								FLOOR			},
							{{ LEX_ON, LEX_HILL },       								DIFF			},
							{{ LEX_WAS_FALLING, LEX_NIGHT },       						SQRT			},
							{{ LEX_HAPPINESS, LEX_GAIN },       						EQUAL			},
							{{ LEX_TIME, LEX_ALL, LEX_LESS, LEX_DEVOTES },       		BELOW			},
							{{ LEX_INTERESTING, LEX_HUGE, LEX_FOUND },       			ABOVE			},
							{{ LEX_NO, LEX_DONT_ENOUGH, LEX_POWER },       				BELOW_EQUAL		},
							{{ LEX_VERY, LEX_STUBBORN },       							ABOVE_EQUAL		},
							{{ LEX_DIFFERENCE, LEX_FOUND },       						NOT_EQUAL		},
							{{ LEX_FUNNY, LEX_ADVICE },       							OR				},
							{{ LEX_HE, LEX_NO, LEX_KNOW },       						NOT				},
							{{ LEX_VO, LEX_DARKNESS, LEX_TO_NIGHT },       				COMMA_OP		},
							{{ LEX_THROUGH, LEX_FOREST, LEX_WANDER },       			INIT_TYPE		},
							{{ LEX_KOL, LEX_WANT, LEX_WILL_TELL },       				PRINTF			},
							{{ LEX_NOTHING, LEX_IN, LEX_LIFE, LEX_NO, LEX_GET_BACK },    RETURN			},
							{{ LEX_ROBBERS, LEX_CAME_OUT, LEX_CROWD },      			BREAK			},
							{{ LEX_CLOSE, LEX_EYES },      								CONTINUE		},
							{{ LEX_FELL_DOWN, LEX_DEAD },      							ABORT			},  };
