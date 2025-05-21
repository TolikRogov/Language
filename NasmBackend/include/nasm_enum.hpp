#pragma once

#define DEF_CMD_(cmd, nasm) CMD_ ## cmd,

enum Commands: char {
	#include "nasm_commands"
	COUNT_OF_COMMANDS
};

#undef DEF_CMD_
