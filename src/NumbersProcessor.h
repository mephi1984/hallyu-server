#ifndef NUMBERS_PROCESSOR_H_INCLUDED
#define NUMBERS_PROCESSOR_H_INCLUDED
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <set>
#include "boost/variant.hpp"
#include "SE/misc.h"
#include "boost/regex.hpp"
#include "boost/algorithm/string/regex.hpp"
#include "boost/random.hpp"

#include "HangulInterfaces.h"


extern const std::wstring CONST_LESSON_CHINESE_NUMBERS; //In HangulProcessor.cpp

struct TChineseNumber : public GetRelatedLessonsInterface, public VerboseInterface
{
	unsigned long long number;

	virtual std::set<std::wstring> GetRelatedLessons() const;
	virtual std::vector<std::wstring> Verbose() const;
};


std::wstring UintToChineseNumberString(unsigned long long i);

std::wstring UintToKoreanNumberString(unsigned long long i);

std::wstring UintToAdjectiveKoreanNumberString(unsigned long long i);


bool RecognizeChineseNumber(std::wstring str, unsigned long long& result);

bool RecognizeChineseNumber(std::wstring str, TChineseNumber& n);


#endif
