#pragma once

static struct Colors {
	const char* name_table_odd_tr					= "\"#FFFFFF\"";
	const char* name_table_even_tr					= "\"#F7F7F7\"";
	const char* name_table_background 				= "\"#F2F8F8\"";
	const char* dot_background 						= "\"#DCDCDC\"";
	const char* html_background 					= "\"#FFFAFA\"";
	const char* address 							= "\"#4B0082\"";
	const char* path 								= "\"#FFA500\"";
	const char* line 								= "\"#20B2AA\"";
	const char* name 								= "\"#7B68EE\"";
	const char* dump_background 					= "\"#F7F7F7\"";
	const char* dump_border							= "\"#C6C1C1\"";
	const char* number 								= "\"#BDB76B\"";
	const char* function 							= "\"#8B008B\"";
	const char* node_font 							= "\"#000000\"";
	const char* node_white_font						= "\"#ffffff\"";
	const char* left_edge 							= "\"#187AE8\"";
	const char* right_edge							= "\"#48E818\"";
	const char* new_node 							= "\"#F18805\"";
	const char* new_node_border						= "\"#AF3800\"";
	const char* unknown_what_node					= "\"#BD3458\"";
	const char* unknown_what_node_border			= "\"#7b0d1e\"";
	const char* unknown_what_edge 					= "\"#3d1308\"";
	const char* keyword_node 						= "\"#F8E807\"";
	const char* keyword_node_border					= "\"#B6AA05\"";
	const char* number_node 						= "\"#BDEF9E\"";
	const char* number_node_border					= "\"#74AF80\"";
	const char* identifier_node 					 = "\"#D5CFE1\"";
	const char* identifier_node_border				 = "\"#8797b2\"";
	const char* function_definition_node 			 = "\"#8EE3EF\"";
	const char* function_definition_node_border 	 = "\"#37718E\"";
	const char* parameters_node						= "\"#EBCBF4\"";
	const char* parameters_node_border 				= "\"#C986C3\"";
	const char* var_declaration_node				= "\"#F34F5D\"";
	const char* var_declaration_node_border			= "\"#BD132F\"";
	const char* call_node 							= "\"#1E90FF\"";
	const char* call_node_border					= "\"#1F75FE\"";;
} color;

static struct Labels {
	const char* left_sub_arrow 		= "\"\"";
	const char* right_sub_arrow		= "\"\"";
	const char* unknown_what_arrow  = "\"Если б мы знали, что это такое.\\nМы не знаем, что это такое.\"";
} labels;
