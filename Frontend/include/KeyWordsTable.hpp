#pragma once

#include "KeyWords.hpp"

KeyWord keywords[AMOUNT_OF_KEYWORDS] = {
	[ELSE] 			= 	{ .num = ELSE, 			.string = "else", 	    },
	[POW] 			= 	{ .num = POW, 			.string = "^", 	    	},
	[IF] 			= 	{ .num = IF, 			.string = "if", 	    },
	[WHILE] 		= 	{ .num = WHILE, 		.string = "while", 	    },
	[ASSIGNMENT] 	= 	{ .num = ASSIGNMENT, 	.string = "=", 	    	},
	[SIN] 			= 	{ .num = SIN, 			.string = "sin", 	    },
	[COS] 			= 	{ .num = COS, 			.string = "cos", 	    },
	[ADD] 			= 	{ .num = ADD, 			.string = "+", 	    	},
	[SUB] 			= 	{ .num = SUB, 			.string = "-", 	    	},
	[MUL] 			= 	{ .num = MUL, 			.string = "*", 	    	},
	[DIV] 			= 	{ .num = DIV, 			.string = "/", 	   		},
	[SQRT] 			= 	{ .num = SQRT, 			.string = "sqrt", 	    },
	[SEQUENTIAL_OP] = 	{ .num = SEQUENTIAL_OP, .string = ";", 	    	},
	[COMMA_OP] 		= 	{ .num = COMMA_OP, 		.string = ",", 	    	},
	[INIT_TYPE] 	= 	{ .num = INIT_TYPE, 	.string = "double", 	},
	[PRINTF] 		= 	{ .num = PRINTF, 		.string = "print", 	    },
	[RETURN] 		= 	{ .num = RETURN, 		.string = "return", 	},
	[BREAK] 		= 	{ .num = BREAK, 		.string = "break", 	    },
	[CONTINUE] 		= 	{ .num = CONTINUE, 		.string = "continue", 	} };
