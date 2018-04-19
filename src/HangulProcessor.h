#ifndef HANGUL_PROCESSOR_H_INCLUDED
#define HANGUL_PROCESSOR_H_INCLUDED
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

#include "HangulInterfaces.h"
#include "NumbersProcessor.h"




struct TSyllable
{
protected:

	void AcceptSyllable(wchar_t syllable);

public:

	TSyllable(wchar_t syllable);
	TSyllable(const TSyllable& syl);
	TSyllable& operator=(const TSyllable& syl);
	TSyllable& operator=(wchar_t syllable);
	bool operator==(const TSyllable& syl) const;
	bool operator!=(const TSyllable& syl) const;

	bool LastChamoExists() const;
	bool LastChamoExistsAndItIsNotL() const; // For ㄹ irregular verbs

	wchar_t firstChamo;
	wchar_t middleChamo;
	wchar_t lastChamo; //

	wchar_t ToUnicodeSyllable() const;
	std::wstring ToLineString() const;
};





//Xperimental
//그들을만도 -- failed!!
//Слово + множ. число + падеж + "только" + "тоже" -- failed!!

//너만을 -- correct!!
//Слово + "только" + множ. число + падеж + "тоже" 
//Нельзя комбинировать 만 и 도

//그 들 에 만 을 -- correct

struct TNounModificators
{
	bool Plural; //들
	
	enum TNounDativeModificators
	{
		NDM_NONE,
		NDM_DATIVE_PLACE,			// 에
		NDM_DATIVE_PERSON,			// 에게
		NDM_DATIVE_PERSON_VERBAL,	// 한테
		NDM_DATIVE_PLACE_FROM,		// 에서
		NDM_DATIVE_PERSON_FROM,		// 에게서
		NDM_DATIVE_PERSON_FROM_VERBAL, // 한테서
		NDM_DATIVE_PLACE_START_FROM,	// 부터
		NDM_DATIVE_PLACE_END_TO,		// 까지
	};

	TNounDativeModificators NounDativeModificators;
	
	bool Only; // Только, -만
	bool Too; // тоже, даже, -도

	TNounModificators()
		: Plural(false)
		, NounDativeModificators(NDM_NONE)
		, Only(false)
		, Too(false)
	{
	}
};

struct TNounParams
{
	
	enum TNounCase
	{
		NC_NONE,
		NC_SUBJECT,		// 이/가
		NC_TOPIC,		// 은/는
		NC_GENITIVE,	// 의
		NC_ACCUSATIVE,	// 을/를

		//Move to TNounModificators
		/*
		NC_DATIVE_PLACE,			// 에
		NC_DATIVE_PERSON,			// 에게
		NC_DATIVE_PERSON_VERBAL,	// 한테
		NC_DATIVE_PLACE_FROM,		// 에서
		NC_DATIVE_PERSON_FROM,		// 에게서
		NC_DATIVE_PERSON_FROM_VERBAL, // 한테서
		NC_DATIVE_PLACE_START_FROM,	// 부터
		NC_DATIVE_PLACE_END_TO,		// 까지
		*/

		NC_LIKE,		// 처럼
		NC_AND,			// 과/와
		NC_AND_VERBAL,	// 하고
		NC_WITH,		// 랑

	};

	TNounCase NounCase;
	TNounModificators NounModificators;
	
	TNounParams()
		: NounCase(NC_NONE)
	{
	}
};


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


struct TNoun : public GetRelatedLessonsInterface, public VerboseInterface
{
	TNounParams Params;
	std::vector<TWordRecord>::const_iterator Itr;

	virtual std::set<std::wstring> GetRelatedLessons() const;

	virtual std::vector<std::wstring> Verbose() const;
};

struct TVerbModificators
{
	enum TTenseModificator
	{
		TM_NONE,
		TM_PAST,
		TM_WILL
	};

	TTenseModificator TenseModificator;

	bool SiModificator;

	bool NegativePrefix;
	
	bool ImpossiblePrefix;

	TVerbModificators()
		: TenseModificator(TM_NONE)
		, SiModificator(false)
		, NegativePrefix(false)
		, ImpossiblePrefix(false)
	{
	}

	std::wstring Verbose() const;

};

struct TVerbParams
{
	enum TVerbStatus
	{
		VS_INFINITIVE,
		VS_PANMAL,				// 2
		VS_POLITE,				// 2 + 요
		VS_HONORIFIC,			// 1 + ㅂ니다 / 습니다
		VS_HONORIFIC_QUESTION,	// 1 + ㅂ니까 / 습니까
		
		VS_MOD_IF,				// 1 + 면 / 으면
		VS_MOD_AND,				// 1 + 고
		VS_MOD_SO,				// 1 + 서 / 아서 / 어서 / 여서 
		VS_MOD_CHI,				// 1 + 지
		VS_MOD_ALTHOUGH,		// 1 + 지만
		VS_PAST_PARTICIPLE,		// 1 + ㄴ / 은
		VS_PRESENT_PARTICIPLE,	// 1 + 는
		VS_FUTURE_PARTICIPLE,	// 1 + ㄹ / 을
		VS_TOO,					// 2 + 도
		VS_REASON,				// 1 + 니까 / 으니까
		VS_BACKGROUND,			// 1 + 은데 / 는데

		//Duplicate
		//VS_GI,				// 1 + 기

		VS_PANMAL_QUESTION,		// 1 + 냐 / 니야 / 니
		VS_EXPRESSED,			// 1 + 네요
		VS_DURING_ACTION,		// 1 + 다가
		VS_FAR_PAST_ACTION,		// 1 + 던
		VS_MULTIPLE_ACTION,		// 1 + 든지
		VS_WHEN_ACTION,			// 1 + ㄹ때 / 을때
		VS_MENTION,				// 1 + 으라고 / 라고
		VS_TEND_ACTION,			// 1 + 으려고 / 려고

		//Duplicate
		//VS_IMPERATIVE_POLITE,	// 1 + 세요 / 으세요

		VS_THE_MORE_THE,		// 1 + 을수록 / ㄹ수록
		VS_UM,					// 1 + 음
		VS_CHA,					// 1 + 자
		VS_POLITE_SURE,			// 1 + 죠 / 지요
		VS_IMPERATIVE_PANMAL,	// 2 + 라
		VS_ACTION_AS_RESULT,	// 1 + ㄹ / 을 + 게
		VS_FORBID_SHORT_PANMAL,	// 1 + 지마
		VS_FORBID_SHORT_POLITE, // 1 + 지마요
		VS_FORBID_SHORT_IMPERATIVE,	// 1 + 지마세요
		VS_GERUND,	// 1 + 기
		VS_GERUND_TOO,	// 1 + 기도
		VS_GERUND_ONLY,	// 1 + 기만
		VS_DO_IT_FOR_ME_SHORT_PANMAL, //2 + 줘
		VS_DO_IT_FOR_ME_SHORT_POLITE, //2 + 줘요
		VS_DO_IT_FOR_ME_SHORT_IMPERATIVE, //2 + 주세요
		VS_MOD_YA, //2 + 야
		
	};

	TVerbStatus VerbStatus;
	TVerbModificators VerbModificators;

	TVerbParams()
		: VerbStatus(VS_INFINITIVE)
	{
	}

	std::wstring Verbose() const;
};


struct TVerb : public GetRelatedLessonsInterface, public VerboseInterface
{
	TVerbParams Params;
	std::vector<TWordRecord>::const_iterator Itr;

	virtual std::set<std::wstring> GetRelatedLessons() const;

	virtual std::vector<std::wstring> Verbose() const;

	std::vector<std::wstring> VerboseWithoutVerbStatus() const;
};

struct TSimpleWord : public GetRelatedLessonsInterface, public VerboseInterface
{
	std::vector<TWordRecord>::const_iterator Itr;

	virtual std::set<std::wstring> GetRelatedLessons() const;

	virtual std::vector<std::wstring> Verbose() const;
};


struct TComplexVerb : public GetRelatedLessonsInterface, public VerboseInterface
{
	enum TComplexVerbType
	{
		CVT_NONE,
		CVT_SINGLE_VERB,
		CVT_CHAINED_VERBS,
		CVT_CHAINED_VERBS_WITH_WORDS_IN_MIDDLE,
	};

	enum TComplexVerbCase
	{
		CVC_NONE,
		CVC_AND,
		CVC_NEGATION,
		CVC_WANT,
		CVC_CONTINUOUS,
		CVC_CONTINUOUS_NEGATIVE,
		CVC_FORBID,
		CVC_GERUND_TOO,
		CVC_GERUND_ONLY,
		CVC_DO_IT_FOR_ME,
		CVC_SHOULD,
		CVC_SHOULD_VERBAL,
		CVC_CAN_DO,
		CVC_CANT_DO,
		//CVC_BE_GOOD_AT,
		//CVC_BE_UNABLE_AT,
		//CVC_BE_BAD_AT,
	};

	TComplexVerbType ComplexVerbType;

	TComplexVerbCase ComplexVerbCase;

	std::shared_ptr<TComplexVerb> InnerComplexVerb;
	std::vector<std::wstring> MiddleWords;

	TVerb LastVerb;

	TComplexVerb();
	TComplexVerb(const TVerb& v);
	virtual std::set<std::wstring> GetRelatedLessons() const;
	virtual std::vector<std::wstring> Verbose() const;

	std::vector<std::wstring> VerboseWithoutVerbStatus() const;
};

struct TFutureTensePhrase : public GetRelatedLessonsInterface, public VerboseInterface
{
	TComplexVerb ComplexVerb;

	enum TFutureTenseType
	{
		FTT_NONE,
		FTT_PANMAL,		//[할] 거야
		FTT_POLITE,		//[할] 거예요
		FTT_HONORIFIC,  //[할] 겁니다
		FTT_HONORIFIC_INTERROGATIVE, //[할] 겁니까
	};

	TFutureTenseType FutureTenseType;

	virtual std::set<std::wstring> GetRelatedLessons() const;
	virtual std::vector<std::wstring> Verbose() const;

	static TFutureTensePhrase::TFutureTenseType RecognizeFutureTenseType(const std::wstring& word);

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

	bool FindVerb(const std::vector<TSyllable>& verb, std::vector<TWordRecord>::const_iterator& result);
	bool FindNoun(const std::vector<TSyllable>& plainNoun, std::vector<TWordRecord>::const_iterator& result);
	bool FindAdverb(const std::vector<TSyllable>& adverb, std::vector<TWordRecord>::const_iterator& result);
	bool FindSpecial(const std::vector<TSyllable>& special, std::vector<TWordRecord>::const_iterator& result);

	void MergeWordToDictionary(std::vector<TWordRecord>& VerbTable, TWordRecord wordRecord);

	void LoadVerbFromFile(const std::string& fileName);
	void LoadNounFromFile(const std::string& fileName);
	void LoadSpecialFromFile(const std::string& fileName);

	void GetRandomWord(size_t wordCount, std::wstring& translation, std::wstring& word0, std::wstring& word1, std::wstring& word2, std::wstring& word3, size_t& correctWord);

	void GetRandomChineseNumber(int maxDigits, unsigned long long& number, std::wstring& numberStr);
};

struct NounTestElement
{
	std::function < bool(std::vector<TSyllable>&) > testFunction;
	std::wstring verbose;
	std::wstring lesson;
};

struct VerbTestElement
{
	std::function < bool(std::vector<TSyllable>&, wchar_t&) > testFunction;
	std::wstring verbose;
	std::wstring lesson;
};

struct ComplexVerbTestElement
{
	std::function < bool(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr) > testFunction;
	std::function < std::vector<std::wstring>(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr) > verboseFunction;

	std::wstring lesson;
};




class THangulProcessor
{
public:

	enum ChamoType
	{
		CT_VOWEL_LIGHT,
		CT_VOWEL_NEUTRAL,
		CT_VOWEL_DARK
	};

	static std::map<TVerbParams::TVerbStatus, VerbTestElement> VerbTestMap;
	static std::map<TNounParams::TNounCase, NounTestElement> NounTestMap;
	static std::map<TComplexVerb::TComplexVerbCase, ComplexVerbTestElement> ComplexVerbTestMap;


	THangulDictionary HangulDictionary;

	THangulProcessor();
	static std::wstring SplitSyllableToChamo(wchar_t syllable);

	static std::vector<TSyllable> WstringToSyllableArr(const std::wstring& wstr);
	static std::wstring SyllableArrToWstring(const std::vector<TSyllable>& syllableArr);

	static std::vector<TSyllable> GetFirstVerbForm(const std::vector<TSyllable>& infinitiveVerb);
	static std::vector<TSyllable> GetSecondVerbForm(const std::vector<TSyllable>& infinitiveVerb);

	static bool ChamoIsVowel(wchar_t chamo);

	static ChamoType GetChamoType(wchar_t chamo);

	static void RemoveLastSyllable(std::vector<TSyllable>& word);
	static bool ChamoAffectLIrregular(wchar_t chamo);


	//Verb Condition Map:
	void GenerateVerbConditionMap();
	//Verbs:
	static void GlueEndingForVerbInSecondFormIfPossible(std::vector<TSyllable>& verbInSecondForm);
	static void UnGlueEndingForVerbInSecondFormIfPossible(std::vector<TSyllable>& verbInSecondForm);

	//Xperimental -- they might be optimized, when counting size of word
	static bool RemoveInfiniteEndingIfPossible(std::vector<TSyllable>& verbInInfinite, wchar_t& outNextChamo);
	static bool ExtractVerbFirstForm(std::vector<TSyllable>& verbInFirstForm, wchar_t& outNextChamo);
	
	static bool RemoveEndingForPastVerbIfPossible(std::vector<TSyllable>& verbInFirstForm, wchar_t& outNextChamo);
	static bool RemoveEndingForWillVerbIfPossible(std::vector<TSyllable>& verbInFirstForm, wchar_t& outNextChamo);
	static bool RemoveSiModificatorForVerbIfPossible(std::vector<TSyllable>& verbInFirstForm, wchar_t& outNextChamo);
	static bool RemoveNegativePrefixForVerbIfPossible(std::vector<TSyllable>& verbInFirstForm, wchar_t& outNextChamo);
	static bool RemoveImpossiblePrefixForVerbIfPossible(std::vector<TSyllable>& verbInFirstForm, wchar_t& outNextChamo);
	

	static bool ReplacePoliteEndingToPanmalIfPossible(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemovePoliteEndingIfPossible(std::vector<TSyllable>& verb, wchar_t& outNextChamo);

	static bool RemoveHonorificEndingIfPossible(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveHonorificQuestionEndingIfPossible(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveModificatorIf(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveModificatorAnd(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveModificatorSo(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveModificatorChi(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveModificatorAlthough(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveModificatorPastParticiple(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveModificatorPresentParticiple(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveModificatorFutureParticiple(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveModificatorToo(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveModificatorReason(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveModificatorBackground(std::vector<TSyllable>& verb, wchar_t& outNextChamo);

	//static bool RemoveModificatorGi(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveModificatorPanmalQuestion(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveModificatorExpressed(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveDuringAction(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveFarPastAction(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveMultipleAction(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveWhenAction(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveMention(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveTendAction(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	//static bool RemoveImperativePolite(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveTheMoreThe(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveUm(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveCha(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemovePoliteSure(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveImperativePanmal(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveActionAsResult(std::vector<TSyllable>& verb, wchar_t& outNextChamo);

	static bool RemoveForbidShortPanmal(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveForbidShortPolite(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveForbidShortImperative(std::vector<TSyllable>& verb, wchar_t& outNextChamo);

	static bool RemoveGerund(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveGerundToo(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveGerundOnly(std::vector<TSyllable>& verb, wchar_t& outNextChamo);

	static bool RemoveDoItForMeShortPanmal(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveDoItForMeShortPolite(std::vector<TSyllable>& verb, wchar_t& outNextChamo);
	static bool RemoveDoItForMeShortImperative(std::vector<TSyllable>& verb, wchar_t& outNextChamo);

	static bool RemoveModificatorYa(std::vector<TSyllable>& verb, wchar_t& outNextChamo);

	//Complex verbs

	//Complex Verb Condition Map
	void GenerateComplexVerbConditionMap();

	static bool TestComplexVerbIsAnd(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
	static bool TestComplexVerbIsNegation(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);

	static bool TestComplexVerbIsWant(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
	static bool TestComplexVerbIsContinuous(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
	static bool TestComplexVerbIsContinuousNegative(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
	static bool TestComplexVerbIsForbid(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);

	static bool TestComplexVerbIsGerundToo(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
	static bool TestComplexVerbIsGerundOnly(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
	static bool TestComplexVerbIsDoItForMe(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);

	static bool TestComplexVerbIsShould(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
	static bool TestComplexVerbIsShouldVerbal(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);

	static bool TestComplexVerbIsCanDo(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
	static bool TestComplexVerbIsCantDo(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);

	//static bool TestComplexVerbIsBeGoodAt(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
	//static bool TestComplexVerbIsBeUnableAt(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
	//static bool TestComplexVerbIsBeBadAt(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);


	static std::vector<std::wstring> VerboseComplexVerb(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
	
	static std::vector<std::wstring> VerboseComplexVerbIsAnd(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
	static std::vector<std::wstring> VerboseComplexVerbIsNegation(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);

	static std::vector<std::wstring> VerboseComplexVerbIsWant(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
	static std::vector<std::wstring> VerboseComplexVerbIsContinuous(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
	static std::vector<std::wstring> VerboseComplexVerbIsContinuousNegative(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);

	static std::vector<std::wstring> VerboseComplexVerbIsForbid(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
	
	static std::vector<std::wstring> VerboseComplexVerbIsGerundToo(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
	static std::vector<std::wstring> VerboseComplexVerbIsGerundOnly(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
	static std::vector<std::wstring> VerboseComplexVerbIsDoItForMe(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
	
	static std::vector<std::wstring> VerboseComplexVerbIsShould(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
	static std::vector<std::wstring> VerboseComplexVerbIsShouldVerbal(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
	
	static std::vector<std::wstring> VerboseComplexVerbIsCanDo(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
	static std::vector<std::wstring> VerboseComplexVerbIsCantDo(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);

	//static std::vector<std::wstring> VerboseComplexVerbIsBeGoodAt(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
	//static std::vector<std::wstring> VerboseComplexVerbIsBeUnableAt(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
	//static std::vector<std::wstring> VerboseComplexVerbIsBeBadAt(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);


	//Noun Condition Map:
	void GenerateNounConditionMap();

	//Nouns:
	static bool RemoveNothing(std::vector<TSyllable>& noun);
	static bool RemoveSubjectCaseEndingIfPossible(std::vector<TSyllable>& noun);
	static bool RemoveTopicCaseEndingIfPossible(std::vector<TSyllable>& noun);
	static bool RemoveGenitiveCaseEndingIfPossible(std::vector<TSyllable>& noun);
	static bool RemoveAccusativeCaseEndingIfPossible(std::vector<TSyllable>& noun);
	
	static bool RemoveLikeCaseEndingIfPossible(std::vector<TSyllable>& noun);
	static bool RemoveAndCaseEndingIfPossible(std::vector<TSyllable>& noun);
	static bool RemoveAndVerbalCaseEndingIfPossible(std::vector<TSyllable>& noun);
	static bool RemoveWithCaseEndingIfPossible(std::vector<TSyllable>& noun);

	static bool TryExtractNounModificator_Too(std::vector<TSyllable>& noun);
	static bool TryExtractNounModificator_Only(std::vector<TSyllable>& noun);
	static bool TryExtractNounModificator_Plural(std::vector<TSyllable>& noun);
	//static bool TryExtractNounModificator_Accusative(std::vector<TSyllable>& noun);

	//Moved away to extra noun modificators
	static bool RemoveDativePlaceCaseEndingIfPossible(std::vector<TSyllable>& noun);
	static bool RemoveDativePersonCaseEndingIfPossible(std::vector<TSyllable>& noun);
	static bool RemoveDativePersonVerbalCaseEndingIfPossible(std::vector<TSyllable>& noun);
	static bool RemoveDativePlaceFromCaseEndingIfPossible(std::vector<TSyllable>& noun);
	static bool RemoveDativePersonFromCaseEndingIfPossible(std::vector<TSyllable>& noun);
	static bool RemoveDativePersonFromVerbalCaseEndingIfPossible(std::vector<TSyllable>& noun);
	static bool RemoveStartFromCaseEndingIfPossible(std::vector<TSyllable>& noun);
	static bool RemoveEndToCaseEndingIfPossible(std::vector<TSyllable>& noun);


	bool RecognizeVerbInInfinitive(std::vector<TSyllable> verb, TVerb& verbResult);
	bool RecognizeVerbInFirstForm(std::vector<TSyllable> verbInFirstForm, TVerb& verbResult);
	bool RecognizeVerbInFirstFormLIrregular(std::vector<TSyllable> verbInFirstForm, TVerb& verbResult);
	bool RecognizeVerbInFirstFormU(std::vector<TSyllable> verbInFirstForm, TVerb& verbResult);

	

	TVerbModificators RemoveBasicVerbModificators(std::vector<TSyllable>& verb, wchar_t& outNextChamo);

	bool RecognizeVerbInFirstFormWithModificators(std::vector<TSyllable> verb, TVerb& verbResult, wchar_t& outNextChamo);
	
	bool RecognizeVerbUniversal(std::vector<TSyllable> verb, TVerb& verbResult, std::function<void(std::vector<TSyllable>&)> extractFunc);
	
	bool RecognizeVerb(std::vector<TSyllable> verb, std::vector<TVerb>& verbResultArr);
	
	//Nouns:
	bool RecognizePlainNoun(std::vector<TSyllable> plainNoun, std::vector<TWordRecord>::const_iterator& result);

	bool RecognizeNoun(std::vector<TSyllable> nounWord, std::vector<TNoun>& nounResultArr);

	
	std::set<std::wstring> GetWordLessons(const boost::variant<TNoun, TComplexVerb, TSimpleWord, TChineseNumber>& result);

	bool RecognizeAndVerboseWords(const std::vector<std::wstring>& inWordArr, std::string& outVerbose, std::set<std::wstring>& outLessons);

	bool RecognizeComplexVerb(const std::vector<std::wstring>& inWordArr, std::vector<TComplexVerb>& resultArr);

	bool RecognizeComplexVerbAndFutureTense(const std::vector<std::wstring>& inWordArr, std::vector<TFutureTensePhrase>& resultArr);


	template<typename T>
	static std::vector<std::wstring> UniversalVerboseWord(const std::vector<T>& wordArr)
	{
		static_assert(std::is_base_of<VerboseInterface, T>::value, "class T must be inherited from VerboseInterface");

		std::vector<std::wstring> wOutVerboseArr;

		for (size_t i = 0; i < wordArr.size(); i++)
		{

			std::vector<std::wstring> localVerboseArr = wordArr[i].Verbose();
			wOutVerboseArr.insert(wOutVerboseArr.end(), localVerboseArr.begin(), localVerboseArr.end());
	
		}

		return wOutVerboseArr;
	}

	static std::string MergeAllVerbose(const std::vector<std::wstring>& verboseArr);


	template<typename T>
	static std::set<std::wstring> UniversalGetLessons(const std::vector<T>& wordArr)
	{
		static_assert(std::is_base_of<GetRelatedLessonsInterface, T>::value, "class T must be inherited from GetRelatedLessonsInterface");

		std::set<std::wstring> lessonsArr;

		for (size_t i = 0; i < wordArr.size(); i++)
		{

			std::set<std::wstring> localLessonsArr = wordArr[i].GetRelatedLessons();
			lessonsArr.insert(localLessonsArr.begin(), localLessonsArr.end());

		}

		return lessonsArr;
	}


};


#endif

