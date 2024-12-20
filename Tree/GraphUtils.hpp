#pragma once

static struct Colors {
	const wchar_t* name_table_odd_tr					= L"\"#FFFFFF\"";
	const wchar_t* name_table_even_tr					= L"\"#F7F7F7\"";
	const wchar_t* name_table_background 				= L"\"#F2F8F8\"";
	const wchar_t* dot_background 						= L"\"#DCDCDC\"";
	const wchar_t* html_background 						= L"\"#FFFAFA\"";
	const wchar_t* address 								= L"\"#4B0082\"";
	const wchar_t* path 								= L"\"#FFA500\"";
	const wchar_t* line 								= L"\"#20B2AA\"";
	const wchar_t* name 								= L"\"#7B68EE\"";
	const wchar_t* dump_background 						= L"\"#F7F7F7\"";
	const wchar_t* dump_border							= L"\"#C6C1C1\"";
	const wchar_t* number 								= L"\"#BDB76B\"";
	const wchar_t* function 							= L"\"#8B008B\"";
	const wchar_t* node_font 							= L"\"#000000\"";
	const wchar_t* node_white_font						= L"\"#ffffff\"";
	const wchar_t* left_edge 							= L"\"#187AE8\"";
	const wchar_t* right_edge							= L"\"#48E818\"";
	const wchar_t* new_node 							= L"\"#F18805\"";
	const wchar_t* new_node_border						= L"\"#AF3800\"";
	const wchar_t* unknown_what_node					= L"\"#BD3458\"";
	const wchar_t* unknown_what_node_border				= L"\"#7b0d1e\"";
	const wchar_t* unknown_what_edge 					= L"\"#3d1308\"";
	const wchar_t* keyword_node 						= L"\"#F8E807\"";
	const wchar_t* keyword_node_border					= L"\"#B6AA05\"";
	const wchar_t* number_node 							= L"\"#BDEF9E\"";
	const wchar_t* number_node_border					= L"\"#74AF80\"";
	const wchar_t* identifier_node 					 	 = L"\"#D5CFE1\"";
	const wchar_t* identifier_node_border				 = L"\"#8797b2\"";
	const wchar_t* function_definition_node 			 = L"\"#8EE3EF\"";
	const wchar_t* function_definition_node_border 	 	 = L"\"#37718E\"";
	const wchar_t* parameters_node						= L"\"#EBCBF4\"";
	const wchar_t* parameters_node_border 				= L"\"#C986C3\"";
	const wchar_t* var_declaration_node					= L"\"#F34F5D\"";
	const wchar_t* var_declaration_node_border			= L"\"#BD132F\"";
	const wchar_t* call_node 							= L"\"#1E90FF\"";
	const wchar_t* call_node_border						= L"\"#1F75FE\"";;
} color;

static struct Labels {
	const wchar_t* left_sub_arrow 		= L"\"\"";
	const wchar_t* right_sub_arrow		= L"\"\"";
	const wchar_t* unknown_what_arrow  	= L"\"Если б мы знали, что это такое.\\nМы не знаем, что это такое.\"";
} labels;
