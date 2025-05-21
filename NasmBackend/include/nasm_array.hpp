#pragma once

#include "nasm_enum.hpp"

#define DEF_CMD_(cmd, nasm) {CMD_ ## cmd, #nasm},

struct ArrayCommand {
	Commands cmd_code;
	const char* cmd_name;
};

const ArrayCommand array_commands[] = {
	#include "nasm_commands"
};
