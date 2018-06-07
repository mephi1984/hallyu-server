#include "LuaHelper.h"
#include "SE/misc.h"

#include "luabind/luabind.hpp"
#include "boost/algorithm/string.hpp"

#include "lua.hpp"

#include "NumbersProcessor.h"

namespace LH
{

	boost::property_tree::ptree DictStruct::Serialize()
	{

		boost::property_tree::ptree result;

		result.put("base", SE::wstring_to_string(base));

		boost::property_tree::ptree wordsArray;

		for (size_t i = 0; i < words.size(); i++) {

			boost::property_tree::ptree node;
			node.put("", SE::wstring_to_string(words[i]));
			wordsArray.push_back(std::make_pair("", node));
		}

		result.add_child("words", wordsArray);


		return result;
	}

	boost::property_tree::ptree WordStruct::Serialize()
	{
	
		boost::property_tree::ptree result;

		result.add_child("dictStruct", dictStruct.Serialize());

		result.put("verbose", SE::wstring_to_string(verbose));

		result.put("wordType", wordType);


		


		boost::property_tree::ptree lessonsArray;

		for (size_t i = 0; i < lessons.size(); i++) {

			boost::property_tree::ptree node;
			node.put("", SE::wstring_to_string(lessons[i]));
			lessonsArray.push_back(std::make_pair("", node));
		}

		result.add_child("lessons", lessonsArray);



		boost::property_tree::ptree modificatorsArray;

		for (size_t i = 0; i < modificators.size(); i++) {

			boost::property_tree::ptree node;
			node.put("", SE::wstring_to_string(modificators[i]));
			lessonsArray.push_back(std::make_pair("", node));
		}

		result.add_child("modificators", modificatorsArray);

		return result;
	}

	boost::property_tree::ptree ComplexVerbTranslateResult::Serialize()
	{

		boost::property_tree::ptree result;

		result.add_child("mainWordStruct", mainWordStruct.Serialize());
		result.add_child("secondaryWordStruct", secondaryWordStruct.Serialize());

		result.put("verbose", SE::wstring_to_string(verbose));



		boost::property_tree::ptree lessonsArray;

		for (size_t i = 0; i < lessons.size(); i++) {

			boost::property_tree::ptree node;
			node.put("", SE::wstring_to_string(lessons[i]));
			lessonsArray.push_back(std::make_pair("", node));
		}

		result.add_child("lessons", lessonsArray);

		result.put("complexVerbType", complexVerbType);

		return result;
	}

	boost::property_tree::ptree HangulResult::Serialize()
	{

		boost::property_tree::ptree result;


		boost::property_tree::ptree resultTableArray;

		for (size_t i = 0; i < resultTable.size(); i++) {

			boost::property_tree::ptree node;

			boost::property_tree::ptree resultListArray;

			for (size_t j = 0; j < resultTable[i].size(); j++) {

				resultListArray.push_back(std::make_pair("", resultTable[i][j].Serialize()));
			}

			node.add_child("words", resultListArray);

			resultTableArray.push_back(std::make_pair("", node));
		}

		result.add_child("resultTable", resultTableArray);




		boost::property_tree::ptree complexVerbResultArray;

		for (size_t i = 0; i < complexVerbResultArr.size(); i++) {

			complexVerbResultArray.push_back(std::make_pair("", complexVerbResultArr[i].Serialize()));
		}

		result.add_child("complexVerbResultArr", complexVerbResultArray);

		return result;
	}

	void OutputPrint(std::string s)
	{
		std::wstring wordTypew = SE::string_to_wstring(s);

		std::cout << s << std::endl;
	}


	std::vector<SyllableWord> SplitPhrase(std::wstring phrase)
	{
		std::vector<SyllableWord> result;

		boost::trim(phrase);

		if (phrase.size() > 0)
		{
			std::vector<std::wstring> wWordToTranslateArr;

			boost::split(wWordToTranslateArr, phrase, boost::is_any_of(L" "), boost::token_compress_on);

			bool hangulWord = true;

			for (auto& wWord : wWordToTranslateArr)
			{

				for (wchar_t& wc : wWord)
				{
					if (wc < 44032 || wc > 55203)
					{
						hangulWord = false;
					}
				}

			}

			if (hangulWord)
			{
				for (auto& wWord : wWordToTranslateArr)
				{
					result.push_back(SyllableWord(wWord));
				}
			}
			else
			{
				SE::WriteToLog("Request contains non-hangul characters");
			}
		}
		else
		{
			SE::WriteToLog("Request is empty");
		}

		return result;
	}

	luabind::object LuaHelper::FindVerb(SyllableWord verb)
	{
		int pos = HangulDictionary.FindVerb(verb);

		if (pos != -1)
		{

			TWordRecord& wordRecord = HangulDictionary.VerbTable[pos];

			luabind::object r = luabind::newtable(L);

			r["base"] = SE::wstring_to_string(wordRecord.base);

			luabind::object words = luabind::newtable(L);

			int i = 1;

			for (auto w : wordRecord.words)
			{
				words[i] = SE::wstring_to_string(w.meaning);
				i++;
			}

			r["words"] = words;

			return r;
		}

		return luabind::object();
	}

	luabind::object LuaHelper::FindNoun(SyllableWord noun)
	{
		int pos = HangulDictionary.FindNoun(noun);

		if (pos != -1)
		{

			TWordRecord& wordRecord = HangulDictionary.NounTable[pos];

			luabind::object r = luabind::newtable(L);

			r["base"] = SE::wstring_to_string(wordRecord.base);

			luabind::object words = luabind::newtable(L);

			int i = 1;

			for (auto w : wordRecord.words)
			{
				words[i] = SE::wstring_to_string(w.meaning);
				i++;
			}

			r["words"] = words;

			return r;
		}

		return luabind::object();
	}

	luabind::object LuaHelper::FindAdverb(SyllableWord adverb)
	{
		int pos = HangulDictionary.FindAdverb(adverb);

		if (pos != -1)
		{

			TWordRecord& wordRecord = HangulDictionary.AdverbTable[pos];

			luabind::object r = luabind::newtable(L);

			r["base"] = SE::wstring_to_string(wordRecord.base);

			luabind::object words = luabind::newtable(L);

			int i = 1;

			for (auto w : wordRecord.words)
			{
				words[i] = SE::wstring_to_string(w.meaning);
				i++;
			}

			r["words"] = words;

			return r;
		}

		return luabind::object();
	}

	luabind::object LuaHelper::FindSpecial(SyllableWord special)
	{
		int pos = HangulDictionary.FindSpecial(special);

		if (pos != -1)
		{

			TWordRecord& wordRecord = HangulDictionary.SpecialTable[pos];

			luabind::object r = luabind::newtable(L);

			r["base"] = SE::wstring_to_string(wordRecord.base);

			luabind::object words = luabind::newtable(L);

			int i = 1;

			for (auto w : wordRecord.words)
			{
				words[i] = SE::wstring_to_string(w.meaning);
				i++;
			}

			r["words"] = words;

			return r;
		}

		return luabind::object();
	}

	luabind::object LuaHelper::FindChineseNumber(SyllableWord number)
	{
		std::wstring wNumber = number.ToWString();
		unsigned long long num;
		
		bool success = RecognizeChineseNumber(wNumber, num);


		if (success)
		{
			luabind::object r = luabind::newtable(L);

			r["base"] = SE::wstring_to_string(wNumber);

			luabind::object words = luabind::newtable(L);

			words[1] = boost::lexical_cast<std::string>(num);

			r["words"] = words;

			return r;
		}
		else
		{
			return luabind::object();
		}
	}


	WordStruct ConvertToWordStruct(luabind::object table)
	{
		using namespace luabind;

		WordStruct r;

		luabind::object verbose = table["verbose"];

		if (type(verbose) == LUA_TSTRING)
		{
			r.verbose = SE::string_to_wstring(object_cast<std::string>(verbose));
		}

		luabind::object lessons = table["lessons"];

		if (type(lessons) == LUA_TTABLE)
		{
			for (luabind::iterator j(lessons), end; j != end; ++j)
			{
				r.lessons.push_back(SE::string_to_wstring(object_cast<std::string>((*j))));
			}
		}

		luabind::object wordType = table["wordType"];

		if (type(wordType) == LUA_TSTRING)
		{
			r.wordType = object_cast<std::string>((wordType));

		}


		luabind::object modificators = table["modificators"];

		if (type(modificators) == LUA_TTABLE)
		{
			for (luabind::iterator j(modificators), end; j != end; ++j)
			{
				r.modificators.push_back(SE::string_to_wstring(object_cast<std::string>((*j))));
			}
		}

		luabind::object dictStructObj = table["dictStruct"];

		if (type(dictStructObj) == LUA_TTABLE)
		{
			r.dictStruct.base = SE::string_to_wstring(object_cast<std::string>(dictStructObj["base"]));

			luabind::object wordsObj = dictStructObj["words"];

			if (type(wordsObj) == LUA_TTABLE)
			{
				for (luabind::iterator j(wordsObj), end; j != end; ++j)
				{
					r.dictStruct.words.push_back(SE::string_to_wstring(object_cast<std::string>((*j))));
				}
			}
		}


		return r;
	}

	ComplexVerbTranslateResult ConvertToComplexVerbTranslateResult(luabind::object table)
	{
		using namespace luabind;

		ComplexVerbTranslateResult r;

		r.mainWordStruct = ConvertToWordStruct(table["mainWordStruct"]);

		r.secondaryWordStruct = ConvertToWordStruct(table["secondaryWordStruct"]);

		r.verbose = SE::string_to_wstring(object_cast<std::string>(table["verbose"]));

		luabind::object lessons = table["lessons"];

		if (type(lessons) == LUA_TTABLE)
		{
			for (luabind::iterator j(lessons), end; j != end; ++j)
			{
				r.lessons.push_back(SE::string_to_wstring(object_cast<std::string>((*j))));
			}
		}

		r.complexVerbType = object_cast<std::string>(table["complexVerbType"]);

		return r;

	}


	LuaHelper::LuaHelper()
	{
		using namespace luabind;

		L = luaL_newstate();

		luaL_openlibs(L);

		open(L);

		module(L)[
			class_<Syllable>("Syllable")
				.def(constructor<wchar_t>())
				.def("hasPatchim", &Syllable::hasPatchim)
				.def("hasPatchimNotL", &Syllable::hasPatchimNotL)
				.def("PrintOut", &Syllable::PrintOut)
				.def("firstChamoStr", &Syllable::firstChamoStr)
				.def("middleChamoStr", &Syllable::middleChamoStr)
				.def("lastChamoStr", &Syllable::lastChamoStr)
				.def("setFirstChamoStr", &Syllable::setFirstChamoStr)
				.def("setMiddleChamoStr", &Syllable::setMiddleChamoStr)
				.def("setLastChamoStr", &Syllable::setLastChamoStr)
				.def("removePatchim", &Syllable::removePatchim)
				.def("patchimIsSS", &Syllable::patchimIsSS)
				.def("isLight", &Syllable::isLight)
				.def("str", &Syllable::str),
				class_<SyllableWord>("SyllableWord")
				.def(constructor<>())
				.def("GetSyllable", &SyllableWord::GetSyllable)
				.def("size", &SyllableWord::size)
				.def("first", &SyllableWord::first)
				.def("last", &SyllableWord::last)
				.def("removeFirst", &SyllableWord::removeFirst)
				.def("pop", &SyllableWord::pop)
				.def("push", &SyllableWord::push)
				.def("clone", &SyllableWord::clone)
				.def("PrintOut", &SyllableWord::PrintOut)
				.def("str", &SyllableWord::str)
				.def("cutEnd", &SyllableWord::cutEnd)
				.def("replaceEnd", &SyllableWord::replaceEnd)
				.def("cutVerbEndVarious", &SyllableWord::cutVerbEndVarious)
				.def("cutNounEndVarious", &SyllableWord::cutNounEndVarious)
				.def("cutBegin", &SyllableWord::cutBegin)
				.def("isValid", &SyllableWord::isValid)
				.def("glueIrregularL", &SyllableWord::glueIrregularL),
				class_<LuaHelper>("LuaHelper")
				.def("FindVerb", &LuaHelper::FindVerb)
				.def("FindNoun", &LuaHelper::FindNoun)
				.def("FindAdverb", &LuaHelper::FindAdverb)
				.def("FindSpecial", &LuaHelper::FindSpecial)
				.def("FindChineseNumber", &LuaHelper::FindChineseNumber),
				def("CreateSyllable", &CreateSyllable),
				def("EmptySyllableWord", &EmptySyllableWord),
				def("OutputPrint", &OutputPrint)
		];


		luabind::globals(L)["luaHelper"] = this;

		//Xperimental
#ifdef _WIN32
		int ret = luaL_dofile(L, "../scripts/main.lua");
#else
		int ret = luaL_dofile(L, "/home/ubuntu/work/hallyu_server/scripts/main.lua");
#endif
		if (ret != 0){

			SE::WriteToLog("Error when running main.lua, code = " + boost::lexical_cast<std::string>(ret) + "\n");

			SE::WriteToLog(lua_tostring(L, -1));

		}
	}

	LuaHelper::~LuaHelper()
	{
		lua_gc(L, LUA_GCCOLLECT, 0);  // collected garbage

		lua_close(L);
	}

	luabind::object LuaHelper::CreateRequestedPhrase(const std::wstring& request)
	{
		std::vector<SyllableWord> phraseArr = SplitPhrase(request);

		luabind::object r = luabind::newtable(L);

		for (size_t i = 0; i < phraseArr.size(); i++)
		{
			r[i + 1] = phraseArr[i];
		}

		return r;

	}

	HangulResult LuaHelper::ProcessString(const std::wstring& request)
	{
		using namespace luabind;

		luabind::object requestedPhrase = CreateRequestedPhrase(request);

		luabind::object luaResult = luabind::call_function<luabind::object>(L, "RecognizeRequestPhraseNew", requestedPhrase);

		HangulResult hangulResult;

		if (type(luaResult) == LUA_TTABLE)
		{

			luabind::object phraseTable = luaResult["phraseTable"];

			if (phraseTable != luabind::object())
			{

				if (type(phraseTable) == LUA_TTABLE)
				{
					for (luabind::iterator wordStructItr(phraseTable), end; wordStructItr != end; ++wordStructItr)
					{
						std::vector<WordStruct> resultArr;

						if (type(*wordStructItr) == LUA_TTABLE)
						{

							for (luabind::iterator i(*wordStructItr), end; i != end; ++i)
							{

								WordStruct r = ConvertToWordStruct(*i);
								resultArr.push_back(r);
							}
						}

						if (resultArr.size() > 0)
						{
							hangulResult.resultTable.push_back(resultArr);
						}
					}
				}


			}

			luabind::object complexVerbArr = luaResult["complexVerbArr"];

			if (complexVerbArr != luabind::object())
			{

				if (type(complexVerbArr) == LUA_TTABLE)
				{
					for (luabind::iterator i(complexVerbArr), end; i != end; ++i)
					{

						ComplexVerbTranslateResult r = ConvertToComplexVerbTranslateResult(*i);

						hangulResult.complexVerbResultArr.push_back(r);

					}
				}


			}

		}

		return hangulResult;
	}
}
