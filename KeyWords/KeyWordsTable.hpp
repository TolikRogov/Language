#pragma once

#include "KeyWords.hpp"

KeyWord keywords[] = {
	{ .num = CLOSE_ROUND, 	.string = ")", 			},
	{ .num = OPEN_FIGURE, 	.string = "{", 			},
	{ .num = CLOSE_FIGURE, 	.string = "}", 			},
 	{ .num = ELSE, 			.string = "else", 	    },
 	{ .num = POW, 			.string = "^", 	    	},
	{ .num = OPEN_ROUND, 	.string = "(", 			},
 	{ .num = IF, 			.string = "if", 	    },
 	{ .num = WHILE, 		.string = "while", 	    },
 	{ .num = ASSIGNMENT, 	.string = "=", 	    	},
 	{ .num = SIN, 			.string = "sin", 	    },
 	{ .num = COS, 			.string = "cos", 	    },
	{ .num = FLOOR,			.string = "floor",		 },
 	{ .num = ADD, 			.string = "+", 	    	},
 	{ .num = SUB, 			.string = "-", 	    	},
 	{ .num = MUL, 			.string = "*", 	    	},
 	{ .num = DIV, 			.string = "/", 	   		},
 	{ .num = SQRT, 			.string = "sqrt", 	    },
 	{ .num = SEQUENTIAL_OP, .string = ";", 	    	},
 	{ .num = COMMA_OP, 		.string = ",", 	    	},
 	{ .num = INIT_TYPE, 	.string = "double", 	},
 	{ .num = PRINTF, 		.string = "print", 	    },
 	{ .num = RETURN, 		.string = "return", 	},
 	{ .num = BREAK, 		.string = "break", 	    },
 	{ .num = CONTINUE, 		.string = "continue", 	},
	{ .num = AMOUNT_OF_KEYWORDS }};
