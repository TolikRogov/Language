#include "KeyWords.hpp"

const wchar_t* KeyWordsGetString(KeyWordNum key_word_number) {

	for (size_t i = 0; keywords[i].num != AMOUNT_OF_KEYWORDS; i++) {
		if (keywords[i].num == key_word_number)
			return keywords[i].string;
	}

	return	L"INVALID_KEY_WORD";
}

KeyWordNum KeyWordsGetKeyWordNum(const wchar_t* string) {

	for (size_t i = 0; keywords[i].num != AMOUNT_OF_KEYWORDS; i++) {
		if (StrCmp(string, keywords[i].string) == 0)
			return keywords[i].num;
	}

	return INVALID_KEY_WORD;
}
