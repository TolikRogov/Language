#include "IdNameTable.hpp"

BinaryTreeStatusCode IdNameTableRealloc(IdNameTable* id_name_table) {

	if (id_name_table->capacity == id_name_table->size) {
		size_t old_capacity = id_name_table->capacity;
		id_name_table->data = (Identifier*)realloc(id_name_table->data, (id_name_table->capacity *= 2) * sizeof(Identifier));
		if (!id_name_table->data)
			return TREE_ALLOC_ERROR;

		for (size_t i = old_capacity; i < id_name_table->capacity; i++) {
			id_name_table->data[i].length = 0;
			id_name_table->data[i].num = 0;
			id_name_table->data[i].string = NULL;
			id_name_table->data[i].type = ID_UNW;
			id_name_table->data[i].define_status = 0;
		}
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode IdNameTableCtor(IdNameTable* id_name_table) {

	id_name_table->capacity = DEFAULT_ID_NAME_TABLE_CAPACITY;
	id_name_table->data = (Identifier*)calloc(id_name_table->capacity, sizeof(Identifier));
	if (!id_name_table->data)
		return TREE_ALLOC_ERROR;

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode IdNameTableDtor(IdNameTable* id_name_table) {

	id_name_table->capacity = 0;
	id_name_table->size = 0;

	if (id_name_table->data) {

		for (size_t i = 0; i < id_name_table->size; i++) {
			if (id_name_table->data[i].function_local_variables) {
				free(id_name_table->data[i].function_local_variables);
				id_name_table->data[i].function_local_variables = NULL;
			}
		}

		free(id_name_table->data);
		id_name_table->data = NULL;
	}

	return TREE_NO_ERROR;
}

int IdNameTableGetIdNumber(IdNameTable* id_name_table, const char* string, size_t length) {

	for (size_t i = 0; i < id_name_table->size; i++) {
		int equal = 1;
		for (size_t j = 0; j < length && id_name_table->data[i].string[j] != '\0'; j++) {
			if (*(string + j) != id_name_table->data[i].string[j]) {
				equal = 0;
				break;
			}
		}
		if (equal)
			return id_name_table->data[i].num;
	}

	return -1;
}

const char* IdNameTableGetIdTypeByType(IdType type) {
	switch (type) {
		case ID_FUNCTION: return RET_STRING(ID_FUNCTION);
		case ID_VAR: return RET_STRING(ID_VAR);
		case ID_UNW:
		default: return RET_STRING(ID_UNW);
	}
}

BinaryTreeStatusCode PrintIdNameTable(IdNameTable* id_name_table) {

	printf("\n");
	printf(BLUE("Struct address:") " " GREEN("%p") "\n", id_name_table);
	printf(BLUE("Table capacity:") " " GREEN("%zu") "\n", id_name_table->capacity);
	printf(BLUE("Table size:") 	   " " GREEN("%zu") "\n", id_name_table->size);
	printf(BLUE("data address:")   " " GREEN("%p") "\n", id_name_table->data);

	for (size_t i = 0; i < id_name_table->capacity; i++) {
		printf(BLUE("Identifier[%zu]:") " number - " GREEN("%d") " type - " GREEN("%s") " def_status - " GREEN("%zu") " length - " GREEN("%zu") " string - ",
			   i, id_name_table->data[i].num, IdNameTableGetIdTypeByType(id_name_table->data[i].type), id_name_table->data[i].define_status, id_name_table->data[i].length);
		for (size_t j = 0; j < id_name_table->data[i].length; j++)
			printf(GREEN("%c"), id_name_table->data[i].string[j]);
		printf("\n");
	}

	return TREE_NO_ERROR;
}
