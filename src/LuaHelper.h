#ifndef LUA_HELPER_H_INCLUDED
#define LUA_HELPER_H_INCLUDED
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <set>


#include "luabind/luabind.hpp"
#include "HangulDictionary.h"

namespace LH
{

	std::vector<SyllableWord> SplitPhrase(std::wstring phrase);


	struct DictStruct
	{
		std::wstring base;
		std::vector<std::wstring> words;
	};

	struct WordStruct
	{
		DictStruct dictStruct;
		std::wstring verbose;
		std::vector<std::wstring> lessons;
		std::vector<std::wstring> modificators;
	};

	struct ComplexVerbTranslateResult
	{
		WordStruct mainWordStruct;
		WordStruct secondaryWordStruct;
		std::wstring verbose;
		std::vector<std::wstring> lessons;
		std::string complexVerbType;

	};


	WordStruct ConvertToWordStruct(luabind::object table);

	ComplexVerbTranslateResult ConvertToComplexVerbTranslateResult(luabind::object table);

	struct HangulResult
	{
		std::vector<std::vector<WordStruct>> resultTable;

		std::vector<ComplexVerbTranslateResult> complexVerbResultArr;
	};

	struct LuaHelper
	{
	protected:
		lua_State* L;

		luabind::object CreateRequestedPhrase(const std::wstring& request);

		luabind::object FindNoun(SyllableWord noun);
		luabind::object FindVerb(SyllableWord verb);
		luabind::object FindAdverb(SyllableWord adverb);
		luabind::object FindSpecial(SyllableWord special);

		luabind::object FindChineseNumber(SyllableWord number);

	public:
		THangulDictionary HangulDictionary;

		LuaHelper();
		LuaHelper(const LuaHelper&) = delete;
		LuaHelper & operator=(const LuaHelper&) = delete;
		~LuaHelper();

		HangulResult ProcessString(const std::wstring& request);
	};
	
}




#endif
