#include "NumbersProcessor.h"
#include <fstream>
#include "boost/algorithm/string.hpp"

#include "boost/regex.hpp"


//const std::vector<std::wstring> CONST_CHINESE_NUMBERS_ZERO{L"공", L"영", L"제로"};

const std::vector<std::wstring> CONST_CHINESE_NUMBERS_1_TO_9{
	L"",
	L"일",
	L"이",
	L"삼",
	L"사",
	L"오",
	L"육",
	L"칠",
	L"팔",
	L"구",
};

const std::vector<std::wstring> CONST_CHINESE_NUMBERS_AFTER_10{
	L"",
	L"십",
	L"백",
	L"천",
	L"만",
	L"억",
	L"조",
};


const std::vector<unsigned long long> CONST_CHINESE_NUMBER_MULTIPLICATOR{
	1,
	10,
	100,
	1000,
	10000,
	100000000,
};



const std::vector<std::wstring> CONST_KOREAN_NUMBERS_1_TO_9{
	L"",
	L"하나",
	L"둘",
	L"셋",
	L"넷",
	L"다섯",
	L"여섯",
	L"일곱",
	L"여덟",
	L"아홉",
};

const std::vector<std::wstring> CONST_KOREAN_NUMBERS_AFTER_10{
	L"",
	L"열",
	L"스물",
	L"서른",
	L"마흔",
	L"예순",
	L"일흔",
	L"여든",
	L"아흔",
};

const std::vector<std::wstring> CONST_ADJECTIVE_KOREAN_NUMBERS_1_TO_9{
	L"",
	L"한",
	L"두",
	L"세",
	L"네",
	L"다섯",
	L"여섯",
	L"일곱",
	L"여덟",
	L"아홉",
};

const std::vector<std::wstring> CONST_ADJECTIVE_KOREAN_NUMBERS_AFTER_10{
	L"",
	L"열",
	L"스무",
	L"서른",
	L"마흔",
	L"예순",
	L"일흔",
	L"여든",
	L"아흔",
};


//==============================================
std::set<std::wstring> TChineseNumber::GetRelatedLessons() const
{
	return{ CONST_LESSON_CHINESE_NUMBERS };
}

std::vector<std::wstring> TChineseNumber::Verbose() const
{
	std::vector<std::wstring> resultArr;

	resultArr.push_back(L"Число " + boost::lexical_cast<std::wstring>(number) + L", записанное китайскими числительными\n");

	return resultArr;
}


//============== Chinese =======================

std::wstring UintToChineseNumberStringNoOne(unsigned long long i);

std::wstring UintToChineseNumberString(unsigned long long i)
{
	if (i >= 0 && i <= 9)
	{
		return CONST_CHINESE_NUMBERS_1_TO_9[i];
	}

	std::wstring result;


	for (int m = CONST_CHINESE_NUMBER_MULTIPLICATOR.size() - 1; m >= 0; m--)
	{
		unsigned long long digit = i / CONST_CHINESE_NUMBER_MULTIPLICATOR[m];

		if (digit > 0)
		{
			std::wstring innerPart;

			bool mostRightDigit = (CONST_CHINESE_NUMBER_MULTIPLICATOR[m] == 1);

			//Xperimental
			if (mostRightDigit)
			{
				innerPart = UintToChineseNumberString(digit);
			}
			else
			{
				innerPart = UintToChineseNumberStringNoOne(digit);
			}

			result = result + innerPart + CONST_CHINESE_NUMBERS_AFTER_10[m];

			i = i % CONST_CHINESE_NUMBER_MULTIPLICATOR[m];

		}
	}

	return result;

}

std::wstring UintToChineseNumberStringNoOne(unsigned long long i)
{
	if (i == 1)
	{
		return L"";
	}

	return UintToChineseNumberString(i);
}


//============== Korean =======================

std::wstring UintToKoreanNumberStringLessThan100(unsigned long long i)
{

	unsigned long long digitRight = i % 10;

	unsigned long long digitTen = i / 10;


	return CONST_KOREAN_NUMBERS_AFTER_10[digitTen] + CONST_KOREAN_NUMBERS_1_TO_9[digitRight];

}


std::wstring UintToKoreanChineseNumberString(unsigned long long i)
{
	if (i >= 0 && i <= 9)
	{
		return CONST_CHINESE_NUMBERS_1_TO_9[i];
	}

	std::wstring result;

	bool mostLeftDigit = true;

	for (int m = CONST_CHINESE_NUMBER_MULTIPLICATOR.size() - 1; m >= 2; m--)
	{
		unsigned long long digit = i / CONST_CHINESE_NUMBER_MULTIPLICATOR[m];

		if (digit > 0)
		{
			std::wstring innerPart;

			if (mostLeftDigit)
			{
				innerPart = UintToChineseNumberStringNoOne(digit);
			}
			else
			{
				innerPart = UintToChineseNumberString(digit);
			}

			result = result + innerPart + CONST_CHINESE_NUMBERS_AFTER_10[m];

			i = i % CONST_CHINESE_NUMBER_MULTIPLICATOR[m];

			mostLeftDigit = false;
		}
	}

	//Leftover:
	unsigned long long digit = i % 100;

	result += UintToKoreanNumberStringLessThan100(digit);

	return result;
}

std::wstring UintToKoreanNumberString(unsigned long long i)
{

	if (i >= 1 && i <= 100)
	{
		return UintToKoreanNumberStringLessThan100(i);
	}

	return UintToKoreanChineseNumberString(i);
}


//============== Korean Adjective =======================

std::wstring UintToAdjectiveKoreanNumberStringLessThan100(unsigned long long i)
{

	unsigned long long digitRight = i % 10;

	unsigned long long digitTen = i / 10;


	return CONST_ADJECTIVE_KOREAN_NUMBERS_AFTER_10[digitTen] + CONST_ADJECTIVE_KOREAN_NUMBERS_1_TO_9[digitRight];

}


std::wstring UintToAdjectiveKoreanChineseNumberString(unsigned long long i)
{
	if (i >= 0 && i <= 9)
	{
		return CONST_CHINESE_NUMBERS_1_TO_9[i];
	}

	std::wstring result;

	bool mostLeftDigit = true;

	for (int m = CONST_CHINESE_NUMBER_MULTIPLICATOR.size() - 1; m >= 2; m--)
	{
		unsigned long long digit = i / CONST_CHINESE_NUMBER_MULTIPLICATOR[m];

		if (digit > 0)
		{
			std::wstring innerPart;

			if (mostLeftDigit)
			{
				innerPart = UintToChineseNumberStringNoOne(digit);
			}
			else
			{
				innerPart = UintToChineseNumberString(digit);
			}

			result = result + innerPart + CONST_CHINESE_NUMBERS_AFTER_10[m];

			i = i % CONST_CHINESE_NUMBER_MULTIPLICATOR[m];

			mostLeftDigit = false;
		}
	}

	//Leftover:
	unsigned long long digit = i % 100;

	result += UintToAdjectiveKoreanNumberStringLessThan100(digit);

	return result;
}

std::wstring UintToAdjectiveKoreanNumberString(unsigned long long i)
{

	if (i >= 1 && i <= 100)
	{
		return UintToAdjectiveKoreanNumberStringLessThan100(i);
	}

	return UintToAdjectiveKoreanChineseNumberString(i);
}

//========= Recognize Chinese Number ===================



bool RecognizeChineseNumberDigit(std::wstring str, unsigned long long& result)
{

	if (str == L"일")
	{

		result = 1;

		return true;
	}

	if (str == L"이")
	{

		result = 2;

		return true;
	}

	if (str == L"삼")
	{

		result = 3;

		return true;
	}

	if (str == L"사")
	{

		result = 4;

		return true;
	}

	if (str == L"오")
	{

		result = 5;

		return true;
	}

	if (str == L"육")
	{

		result = 6;

		return true;
	}

	if (str == L"칠")
	{

		result = 7;

		return true;
	}

	if (str == L"팔")
	{

		result = 8;

		return true;
	}

	if (str == L"구")
	{

		result = 9;

		return true;
	}


	return false;
}

bool RecognizeChineseNumberMultiplierAndDigit(std::wstring str, unsigned long long& result)
{
	if (str.size() == 0)
	{
		return false;
	}

	//([STR]?억)?([STR]?만)?([STR]?천)?([STR]?백)?([STR]?십)?([STR]?)

	if (str[str.size() - 1] == L'십')
	{
		str.erase(str.end()-1);
		result = 10;
	}
	else if (str[str.size() - 1] == L'백')
	{
		str.erase(str.end() - 1);
		result = 100;
	}
	else if (str[str.size() - 1] == L'천')
	{
		str.erase(str.end() - 1);
		result = 1000;
	}
	else if (str[str.size() - 1] == L'만')
	{
		str.erase(str.end() - 1);
		result = 10000;
	}
	else if (str[str.size() - 1] == L'억')
	{
		str.erase(str.end() - 1);
		result = (long long)10000 * (long long)10000;
	}
	else if (str[str.size() - 1] == L'조')
	{
		str.erase(str.end() - 1);
		result = (long long)10000 * (long long)10000 * (long long)10000;
	}
	else
	{
		return false;
	}


	if (str.size() == 0)
	{
		return true;
	}



	unsigned long long leftover;

	bool leftoverResult = RecognizeChineseNumberDigit(str, leftover);

	if (leftoverResult)
	{
		result *= leftover;

		return true;
	}

	return false;




}



bool RecognizeChineseNumber(std::wstring str, unsigned long long& result)
{

	static std::wstring CONST_CHINESE_ONE_TO_NINE = L"일이삼사오육칠팔구";

	static std::wstring regexpStr = boost::replace_all_copy(std::wstring(L"([STR]?조)?([STR]?억)?([STR]?만)?([STR]?천)?([STR]?백)?([STR]?십)?([STR])?"), std::wstring(L"STR"), CONST_CHINESE_ONE_TO_NINE);

	static boost::wregex xRegEx(regexpStr);


	if (str.size() == 0)
	{
		return false;
	}
	
	result = 0;

	try
	{

		bool foundSomething = false;


		/*
		boost::wsmatch what;

		if (boost::regex_match(str, what, xRegEx, boost::match_extra))
		{

			
			for (size_t i = 0; i < what.size(); ++i)
			{
				std::cout << "      $" << i << "\n";
				//std::cout << "      $" << i << " = \"" << SE::wstring_to_string(what[i]) << "\"\n";
			}



		}

		return false;*/

		const int subgroups[] = { 1,2,3,4,5,6, 7 };

		boost::wsregex_token_iterator xItFull(str.begin(), str.end(), xRegEx, subgroups);
		boost::wsregex_token_iterator xInvalidIt;

		unsigned long long intermediateResult = 0;


		while (xItFull != xInvalidIt)
		{
			std::wstring part = *xItFull;

			xItFull++;

			if (part.size() == 0)
			{
				continue;
			}


			if (RecognizeChineseNumberMultiplierAndDigit(part, intermediateResult) || RecognizeChineseNumberDigit(part, intermediateResult))
			{
				result += intermediateResult;
				foundSomething = true;
			}
			else
			{
				return false;
			}

			
		}

		
		return foundSomething;


		/*
		std::vector<std::wstring> availableChineseNumbersAfter10 = CONST_CHINESE_NUMBERS_AFTER_10;
		std::vector<unsigned long long> availableChineseMultiplicator = CONST_CHINESE_NUMBER_MULTIPLICATOR;


		if (str.size() == 0)
		{
			return false;
		}

		bool numberFound = false;

		while (str.size() > 0)
		{

			wchar_t ch = str[0];

			str.erase(str.begin());

			auto itr = std::find(CONST_CHINESE_NUMBERS_1_TO_9.begin(), CONST_CHINESE_NUMBERS_1_TO_9.end(), std::wstring() + ch);


			size_t number = 1;

			if (itr != CONST_CHINESE_NUMBERS_1_TO_9.end())
			{
				numberFound = true;
				number = itr - CONST_CHINESE_NUMBERS_1_TO_9.begin();
			}


			if (str.size() > 0)
			{
				wchar_t ch_ten = str[0];
				str.erase(str.begin());

				auto itr_ten = std::find(availableChineseNumbersAfter10.begin(), availableChineseNumbersAfter10.end(), std::wstring() + ch_ten);


				if (itr_ten == availableChineseNumbersAfter10.end())
				{
					return false;
				}

				size_t number_ten = itr_ten - availableChineseNumbersAfter10.begin();

				availableChineseNumbersAfter10 = std::vector<std::wstring>(availableChineseNumbersAfter10.begin(), availableChineseNumbersAfter10.begin() + number_ten);
				availableChineseMultiplicator = std::vector<unsigned long long>(availableChineseMultiplicator.begin(), availableChineseMultiplicator.begin() + number_ten);


			}

			result += number;

		}
*/
	}
	catch (std::exception e)
	{
		SE::WriteToLog("Exception in RecognizeChineseNumber");
	}

	return false;
}

bool RecognizeChineseNumber(std::wstring str, TChineseNumber& n)
{
	try
	{

		std::vector<std::wstring> availableChineseNumbersAfter10 = CONST_CHINESE_NUMBERS_AFTER_10;
		std::vector<unsigned long long> availableChineseMultiplicator = CONST_CHINESE_NUMBER_MULTIPLICATOR;

		unsigned long long result = 0;

		if (str.size() == 0)
		{
			return false;
		}

		while (str.size() > 0)
		{

			wchar_t ch = str[0];

			str.erase(str.begin());

			auto itr = std::find(CONST_CHINESE_NUMBERS_1_TO_9.begin(), CONST_CHINESE_NUMBERS_1_TO_9.end(), std::wstring() + ch);

			if (itr == CONST_CHINESE_NUMBERS_1_TO_9.end())
			{
				return false;
			}

			size_t number = itr - CONST_CHINESE_NUMBERS_1_TO_9.begin();


			if (str.size() > 0)
			{
				wchar_t ch_ten = str[0];
				str.erase(str.begin());

				auto itr_ten = std::find(availableChineseNumbersAfter10.begin(), availableChineseNumbersAfter10.end(), std::wstring() + ch_ten);

				if (itr_ten == availableChineseNumbersAfter10.end())
				{
					return false;
				}

				size_t number_ten = itr_ten - availableChineseNumbersAfter10.begin();

				number *= availableChineseMultiplicator[number_ten];

				availableChineseNumbersAfter10 = std::vector<std::wstring>(availableChineseNumbersAfter10.begin(), availableChineseNumbersAfter10.begin() + number_ten);
				availableChineseMultiplicator = std::vector<unsigned long long>(availableChineseMultiplicator.begin(), availableChineseMultiplicator.begin() + number_ten);


			}

			result += number;

		}

		n.number = result;

		return true;
	}
	catch (std::exception e)
	{
		SE::WriteToLog("Exception in RecognizeChineseNumber");
	}

	return false;


}

