#ifndef LUA_HELPER_H_INCLUDED
#define LUA_HELPER_H_INCLUDED
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <set>

#include <boost/property_tree/ptree.hpp>

#include "luabind/luabind.hpp"
#include "HangulDictionary.h"

namespace LH
{

	std::vector<SyllableWord> SplitPhrase(std::wstring phrase);


	struct DictStruct
	{
		std::wstring base;
		std::vector<std::wstring> words;

		boost::property_tree::ptree Serialize();
	};

	struct WordStruct
	{
		DictStruct dictStruct;
		std::wstring verbose;
		std::vector<int> lessons;
		std::vector<std::wstring> modificators;
		std::string wordType;
		std::wstring originalWord;

		boost::property_tree::ptree Serialize();
	};

	struct ComplexVerbTranslateResult
	{
		WordStruct mainWordStruct;
		WordStruct secondaryWordStruct;
		std::wstring verbose;
		std::wstring originalPhrase;
		std::vector<int> lessons;
		std::string complexVerbType;

		boost::property_tree::ptree Serialize();
	};


	WordStruct ConvertToWordStruct(luabind::object table);

	ComplexVerbTranslateResult ConvertToComplexVerbTranslateResult(luabind::object table);

	struct HangulResult
	{
		std::vector<std::vector<WordStruct>> resultTable;

		std::vector<ComplexVerbTranslateResult> complexVerbResultArr;

		boost::property_tree::ptree Serialize();
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
