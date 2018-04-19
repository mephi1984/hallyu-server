#ifndef HANGUL_DICTIONARY_H_INCLUDED
#define HANGUL_DICTIONARY_H_INCLUDED
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <set>

#include <type_traits>

#include "boost/variant.hpp"
#include "SE/misc.h"
#include "boost/regex.hpp"
#include "boost/algorithm/string/regex.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/random.hpp"

namespace LH
{
	extern const wchar_t CONST_EMPTY_LAST_CHAMO;

	bool ChamoIsVowel(wchar_t chamo);

	bool SymbolIsSyllable(wchar_t symbol);

	bool SymbolIsLastChamo(wchar_t symbol);

	struct Syllable
	{
	protected:

		void AcceptSyllable(wchar_t syllable);

	public:

		Syllable(wchar_t syllable);

		bool hasPatchim() const;
		bool hasPatchimNotL() const; // For ㄹ irregular verbs

		wchar_t firstChamo;
		wchar_t middleChamo;
		wchar_t lastChamo;

		wchar_t ToUnicodeSyllable() const;

		std::string str() const;

		void PrintOut() const;

		std::string firstChamoStr() const;
		std::string middleChamoStr() const;
		std::string lastChamoStr() const;

		void setFirstChamoStr(std::string c);
		void setMiddleChamoStr(std::string c);
		void setLastChamoStr(std::string c);
		void removePatchim();
		bool patchimIsSS();

		bool isLight() const;
	};

	struct SyllableWord
	{
	protected:
		std::vector<Syllable> syllableArr;
	public:
		SyllableWord();
		SyllableWord(const std::wstring& wstr);
		SyllableWord(const std::vector<Syllable>& newSyllableArr);
		Syllable GetSyllable(int position);
		int size();
		Syllable last();
		Syllable first();
		std::wstring ToWString() const;

		SyllableWord removeFirst();
		SyllableWord pop();
		SyllableWord push(Syllable s);
		SyllableWord clone() const;
		void PrintOut() const;
		std::string str() const;
	
		SyllableWord cutEnd(std::string s) const;
		SyllableWord replaceEnd(std::string str, std::string newStr) const;

		
		SyllableWord cutEndVarious(std::string sOnPatchimExist, std::string sOnNoPatchimExist, bool ignoreTypeLPatchim) const;

		SyllableWord cutVerbEndVarious(std::string sOnPatchimExist, std::string sOnNoStrongPatchimExist) const;

		SyllableWord cutNounEndVarious(std::string sOnPatchimExist, std::string sOnNoPatchimExist) const;

		SyllableWord cutBegin(std::string s) const;

		SyllableWord glueIrregularL() const;

		bool isValid() const;
		
	};

	Syllable CreateSyllable(std::string s);

	SyllableWord EmptySyllableWord();

	std::vector<Syllable> WstringToSyllableArr(const std::wstring& wstr);
	std::wstring SyllableArrToWstring(const std::vector<Syllable>& syllableArr);

	struct TMeaningAndComment
	{
		std::wstring meaning;
		std::wstring comment;

		bool operator<(const TMeaningAndComment& other) const;
	};

	struct TWordRecord
	{
		int priorityNum;

		std::wstring base;
		std::set<std::wstring> relatedLessons;
		std::set<TMeaningAndComment> words;

		TWordRecord();

		TWordRecord(const std::wstring& w, const std::wstring& t);

		TWordRecord(const std::wstring& line);
	};


	class THangulDictionary
	{
	public:
		std::vector<TWordRecord> NounTable;
		std::vector<TWordRecord> VerbTable;
		std::vector<TWordRecord> AdverbTable; //Наречие
		std::vector<TWordRecord> SpecialTable;

		std::vector<TWordRecord> AllWords; //For cards game
		boost::random::mt19937 RandomGenerator;

		THangulDictionary();

		int FindVerb(const SyllableWord& verb);
		int FindNoun(const SyllableWord& noun);
		int FindAdverb(const SyllableWord& adverb);
		int FindSpecial(const SyllableWord& special);

		void MergeWordToDictionary(std::vector<TWordRecord>& VerbTable, TWordRecord wordRecord);

		void LoadVerbFromFile(const std::string& fileName);
		void LoadNounFromFile(const std::string& fileName);
		void LoadSpecialFromFile(const std::string& fileName);

		void GetRandomWord(size_t wordCount, std::wstring& translation, std::wstring& word0, std::wstring& word1, std::wstring& word2, std::wstring& word3, size_t& correctWord);

		std::vector<TWordRecord> SearchWordByRussian(std::wstring wRussianWord);

		void GetRandomChineseNumber(int maxDigits, unsigned long long& number, std::wstring& numberStr);
	};


}

#endif

