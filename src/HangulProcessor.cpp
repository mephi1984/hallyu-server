#include "HangulProcessor.h"



const TSyllable& CONST_VERB_END_SYLLABLE(L'다');

const TSyllable& CONST_DO_VERB_SYLLABLE(L'하');
const TSyllable& CONST_DO_VERB_PT_SYLLABLE(L'해');
const TSyllable& CONST_DO_VERB_PAST_SYLLABLE(L'했');

const TSyllable& CONST_FUNCTION_VERB_SYLLABLE(L'되');
const TSyllable& CONST_FUNCTION_VERB_PT_SYLLABLE(L'돼');
const TSyllable& CONST_FUNCTION_VERB_PAST_SYLLABLE(L'됐');

const TSyllable& CONST_FORBID_VERB_SYLLABLE(L'말');
const TSyllable& CONST_FORBID_VERB_PT_SYLLABLE(L'마');



const wchar_t CONST_EMPTY_LAST_CHAMO = 0x11A7;


const wchar_t CONST_LIGHT_VOWEL_CHAMO_ARR[] = { L'ᅡ', L'ᅢ', L'ᅣ', L'ᅤ', L'ᅩ', L'ᅪ', L'ᅫ', L'ᅬ', L'ᅭ' };

const wchar_t CONST_NEUTRAL_VOWEL_CHAMO_ARR[] = { L'ᅳ', L'ᅴ', L'ᅵ' };

const wchar_t CONST_DARK_VOWEL_CHAMO_ARR[] = { L'ᅥ', L'ᅦ', L'ᅧ', L'ᅨ', L'ᅮ', L'ᅯ', L'ᅰ', L'ᅱ' };

const std::set<wchar_t> CONST_LIGHT_VOWEL_CHAMO_SET(std::begin(CONST_LIGHT_VOWEL_CHAMO_ARR), std::end(CONST_LIGHT_VOWEL_CHAMO_ARR));
const std::set<wchar_t> CONST_NEUTRAL_VOWEL_CHAMO_SET(std::begin(CONST_NEUTRAL_VOWEL_CHAMO_ARR), std::end(CONST_NEUTRAL_VOWEL_CHAMO_ARR));
const std::set<wchar_t> CONST_DARK_VOWEL_CHAMO_SET(std::begin(CONST_DARK_VOWEL_CHAMO_ARR), std::end(CONST_DARK_VOWEL_CHAMO_ARR));

const std::wstring CONST_LESSON_CHINESE_NUMBERS = L"lessons/korean014.html";

const std::wstring CONST_LESSON_NOUN_SUBJECT = L"lessons/korean009.html";
const std::wstring CONST_LESSON_NOUN_TOPIC = L"lessons/korean009.html";
const std::wstring CONST_LESSON_NOUN_ACCUSATIVE = L"lessons/korean013.html";
const std::wstring CONST_LESSON_NOUN_LOCATION = L"lessons/korean017.html";

const std::wstring CONST_LESSON_NOUN_GENITIVE = L"lessons/korean028.html";
const std::wstring CONST_LESSON_NOUN_DATIVE_PLACE = L"lessons/korean017.html";
const std::wstring CONST_LESSON_NOUN_FROM_TO = L"lessons/korean024.html";
const std::wstring CONST_LESSON_NOUN_DATIVE_PERSON = L"lessons/korean030.html";
const std::wstring CONST_LESSON_NOUN_AND_WITH = L"lessons/korean031.html";
const std::wstring CONST_LESSON_NOUN_TOO = L"lessons/korean032.html";
const std::wstring CONST_LESSON_NOUN_ONLY = L"lessons/korean033.html";

const std::wstring CONST_LESSON_VERB_INFINITIVE = L"lessons/korean010.html";

const std::wstring CONST_LESSON_VERB_SIMPLE_PRESENT_POLITE = L"lessons/korean011.html";
const std::wstring CONST_LESSON_VERB_PAST = L"lessons/korean015.html";

const std::wstring CONST_LESSON_VERB_NEGATIVE = L"lessons/korean018.html";
const std::wstring CONST_LESSON_VERB_I_WANT_TO = L"lessons/korean020.html";
const std::wstring CONST_LESSON_VERB_FUTURE_POLITE = L"lessons/korean027.html";

const std::wstring CONST_LESSON_VERB_CONTINUOUS = L"lessons/korean029.html";


const std::wstring CONST_LESSON_VERB_TOO = L"lessons/korean032.html";
const std::wstring CONST_LESSON_VERB_ONLY = L"lessons/korean033.html";
const std::wstring CONST_LESSON_VERB_DO_IT_FOR_ME = L"lessons/korean035.html";
const std::wstring CONST_LESSON_VERB_IF = L"lessons/korean036.html";
const std::wstring CONST_LESSON_VERB_FORBID = L"lessons/korean038.html";
const std::wstring CONST_LESSON_VERB_PARTICIPLE = L"lessons/korean041.html";
const std::wstring CONST_LESSON_VERB_CAN = L"lessons/korean042.html";


std::map<TVerbParams::TVerbStatus, VerbTestElement> THangulProcessor::VerbTestMap;
std::map<TNounParams::TNounCase, NounTestElement> THangulProcessor::NounTestMap;
std::map<TComplexVerb::TComplexVerbCase, ComplexVerbTestElement> THangulProcessor::ComplexVerbTestMap;





TSyllable::TSyllable(wchar_t syllable)
{
	AcceptSyllable(syllable);
}

TSyllable::TSyllable(const TSyllable& syl)
{
	firstChamo = syl.firstChamo;
	middleChamo = syl.middleChamo;
	lastChamo = syl.lastChamo;
}

void TSyllable::AcceptSyllable(wchar_t syllable)
{
	if (syllable < 0xAC00 || syllable > 0xD7A3)
	{
		//throw SE::ErrorToLog("symbol is not hangul syllable or is a single chamo");
		SE::WriteToLog("TSyllable::AcceptSyllable: symbol is not hangul syllable or is a single chamo");
		syllable = L'가';
	}

	firstChamo = ((syllable - 0xAC00) / 0x24C) + 0x1100;

	if (firstChamo < 0x1100 || firstChamo > 0x1112)
	{
		//throw SE::ErrorToLog("first chamo is obsolete or incorrect");
		SE::WriteToLog("TSyllable::AcceptSyllable: first chamo is obsolete or incorrect");
		syllable = L'가';
		firstChamo = 0x1100;
	}

	middleChamo = ((syllable - 0xAC00) % 0x24C) / 0x1C + 0x1161;

	if (!THangulProcessor::ChamoIsVowel(middleChamo))
	{
		syllable = L'가';
		firstChamo = 0x1100;
		middleChamo = 0x1161;
	}

	lastChamo = ((syllable - 0xAC00) % 0x24C) % 0x1C + CONST_EMPTY_LAST_CHAMO;

	if (lastChamo < CONST_EMPTY_LAST_CHAMO || lastChamo > 0x11C2)
	{
		//throw SE::ErrorToLog("last chamo is obsolete or incorrect");
		SE::WriteToLog("TSyllable::AcceptSyllable: last chamo is obsolete or incorrect");
		syllable = L'가';
		lastChamo = CONST_EMPTY_LAST_CHAMO;
		firstChamo = 0x1100;
		middleChamo = 0x1161;
	}
}

TSyllable& TSyllable::operator=(const TSyllable& syl)
{
	firstChamo = syl.firstChamo;
	middleChamo = syl.middleChamo;
	lastChamo = syl.lastChamo;

	return *this;
}

TSyllable& TSyllable::operator=(wchar_t syllable)
{
	AcceptSyllable(syllable);

	return *this;
}

bool TSyllable::operator==(const TSyllable& syl) const
{
	return (firstChamo == syl.firstChamo &&
		middleChamo == syl.middleChamo &&
		lastChamo == syl.lastChamo);
}

bool TSyllable::operator!=(const TSyllable& syl) const
{
	return !(*this == syl);
}

bool TSyllable::LastChamoExists() const
{
	return lastChamo != CONST_EMPTY_LAST_CHAMO;
}

bool TSyllable::LastChamoExistsAndItIsNotL() const
{
	return lastChamo != CONST_EMPTY_LAST_CHAMO && lastChamo != L'ᆯ';
}

wchar_t TSyllable::ToUnicodeSyllable() const
{
	return ((firstChamo - 0x1100) * 0x24C)
		+ ((middleChamo - 0x1161) * 0x1C)
		+ (lastChamo - CONST_EMPTY_LAST_CHAMO)
		+ 0xAC00;
}


std::wstring TSyllable::ToLineString() const
{
	if (lastChamo == 0x11A7)
	{
		return std::wstring(1, firstChamo) + middleChamo;
	}
	else
	{
		return (std::wstring(1, firstChamo) + middleChamo) + lastChamo;
	}
}

//========================================================


bool TMeaningAndComment::operator<(const TMeaningAndComment& other) const
{
	if (meaning != other.meaning)
	{
		return meaning < other.meaning;
	}
	else
	{
		return comment < other.comment;
	}
}


TWordRecord::TWordRecord()
{
}

TWordRecord::TWordRecord(const std::wstring& w, const std::wstring& t)
{
	base = w;
	words.insert(TMeaningAndComment{ t, L"" });
}

TWordRecord::TWordRecord(const std::wstring& line)
{
	std::vector<std::wstring> lineArr;

	boost::split_regex(lineArr, line, boost::wregex(L"	"));

	if (lineArr[0] == L"")
	{
		priorityNum = 0;
	}
	else
	{
		priorityNum = boost::lexical_cast<int>(lineArr[0]);
	}

	base = lineArr[1];

	std::vector<std::wstring> relatedLessonsStrArr;

	relatedLessons.clear();

	if (lineArr[2] != L"")
	{
		boost::erase_all(lineArr[2], " ");
		boost::split(relatedLessonsStrArr, lineArr[2], boost::is_any_of(L","));


		for (auto& relatedLessonStr : relatedLessonsStrArr)
		{
			relatedLessons.insert(relatedLessonStr);
		}

	}


	for (size_t i = 3; i < lineArr.size(); i++)
	{
		if (lineArr[i] != L"")
		{
			words.insert(TMeaningAndComment{ lineArr[i], L"" });
		}
	}
}


TWordRecord CreateWord(const std::wstring& word, const std::wstring& translation)
{
	return TWordRecord(word, translation);
}
//========================================================

std::set<std::wstring> TNoun::GetRelatedLessons() const
{
	std::set<std::wstring> result = Itr->relatedLessons;

	if (Params.NounModificators.Plural)
	{
		//result.insert();
	}
	if (Params.NounModificators.NounDativeModificators == TNounModificators::NDM_DATIVE_PLACE)
	{
		result.insert(CONST_LESSON_NOUN_DATIVE_PLACE);
	}
	if (Params.NounModificators.NounDativeModificators == TNounModificators::NDM_DATIVE_PERSON)
	{
		result.insert(CONST_LESSON_NOUN_DATIVE_PERSON);
	}
	if (Params.NounModificators.NounDativeModificators == TNounModificators::NDM_DATIVE_PERSON_VERBAL)
	{
		result.insert(CONST_LESSON_NOUN_DATIVE_PERSON);
	}
	if (Params.NounModificators.NounDativeModificators == TNounModificators::NDM_DATIVE_PLACE_FROM)
	{
		result.insert(CONST_LESSON_NOUN_DATIVE_PLACE);
	}
	if (Params.NounModificators.NounDativeModificators == TNounModificators::NDM_DATIVE_PERSON_FROM)
	{
		result.insert(CONST_LESSON_NOUN_DATIVE_PERSON);
	}
	if (Params.NounModificators.NounDativeModificators == TNounModificators::NDM_DATIVE_PERSON_FROM_VERBAL)
	{
		result.insert(CONST_LESSON_NOUN_DATIVE_PERSON);
	}
	if (Params.NounModificators.NounDativeModificators == TNounModificators::NDM_DATIVE_PLACE_START_FROM)
	{
		result.insert(CONST_LESSON_NOUN_FROM_TO);
	}
	if (Params.NounModificators.NounDativeModificators == TNounModificators::NDM_DATIVE_PLACE_END_TO)
	{
		result.insert(CONST_LESSON_NOUN_FROM_TO);
	}

	if (Params.NounModificators.Only)
	{
		result.insert(CONST_LESSON_NOUN_ONLY);
	}
	if (Params.NounModificators.Too)
	{
		result.insert(CONST_LESSON_NOUN_TOO);
	}

	return result;
}


std::vector<std::wstring> TNoun::Verbose() const
{
	std::vector<std::wstring> resultArr;

	for (auto& w : Itr->words)
	{
		std::wstring word;

		word = L"Существительное \"" + w.meaning + L"\"\n";

		word += THangulProcessor::NounTestMap[Params.NounCase].verbose;

		
		if (Params.NounModificators.Plural)
		{
			word += L"В множественном числе\n";
		}
		if (Params.NounModificators.NounDativeModificators == TNounModificators::NDM_DATIVE_PLACE)
		{
			word += L"В местном падеже (куда? где?)\n";
		}
		if (Params.NounModificators.NounDativeModificators == TNounModificators::NDM_DATIVE_PERSON)
		{
			word += L"В местном падеже лица (кому? К кому? Для кого?) в письменной форме\n";
		}
		if (Params.NounModificators.NounDativeModificators == TNounModificators::NDM_DATIVE_PERSON_VERBAL)
		{
			word += L"В местном падеже лица (кому? К кому? Для кого?) в разговорной форме\n";
		}
		if (Params.NounModificators.NounDativeModificators == TNounModificators::NDM_DATIVE_PLACE_FROM)
		{
			word += L"В местном исходящем падеже (откуда? где?)\n";
		}
		if (Params.NounModificators.NounDativeModificators == TNounModificators::NDM_DATIVE_PERSON_FROM)
		{
			word += L"В местном исходящем падеже лица (от кого?) в письменной форме\n";
		}
		if (Params.NounModificators.NounDativeModificators == TNounModificators::NDM_DATIVE_PERSON_FROM_VERBAL)
		{
			word += L"В местном исходящем падеже лица (от кого?) в разговорной форме\n";
		}
		if (Params.NounModificators.NounDativeModificators == TNounModificators::NDM_DATIVE_PLACE_START_FROM)
		{
			word += L"В местном падеже с обозначением места начала (откуда? От чего?)\n";
		}
		if (Params.NounModificators.NounDativeModificators == TNounModificators::NDM_DATIVE_PLACE_END_TO)
		{
			word += L"В местном падеже с обозначением места конца (докуда? До чего?)\n";
		}

		if (Params.NounModificators.Only)
		{
			word += L"С модификатором \"только\", \"лишь\"\n";
		}
		if (Params.NounModificators.Too)
		{
			word += L"С модификатором \"тоже\", \"также\"\n";
		}


		


		resultArr.push_back(word);
	}

	return resultArr;
}


//========================================================


std::wstring TVerbModificators::Verbose() const
{
	std::wstring word;

	if (SiModificator)
	{
		word += L"С модификатором повышенной вежливости\n";
	}
	if (TenseModificator == TVerbModificators::TM_PAST)
	{
		word += L"В прошедшем времени\n";
	}
	if (TenseModificator == TVerbModificators::TM_WILL)
	{
		word += L"С модификатором намерения\n";
	}

	if (NegativePrefix)
	{
		word += L"С приставкой отрицания\n";
	}

	if (ImpossiblePrefix)
	{
		word += L"С приставкой \"невозможно сделать\"\n";
	}


	return word;
}

std::wstring TVerbParams::Verbose() const
{
	std::wstring word;

	word += THangulProcessor::VerbTestMap[VerbStatus].verbose;

	word += VerbModificators.Verbose();

	
	return word;
}

std::set<std::wstring> TVerb::GetRelatedLessons() const
{
	std::set<std::wstring> result;

	result.insert(Itr->relatedLessons.begin(), Itr->relatedLessons.end());

	result.insert(THangulProcessor::VerbTestMap[Params.VerbStatus].lesson);

	if (Params.VerbModificators.TenseModificator == TVerbModificators::TM_PAST)
	{
		result.insert(CONST_LESSON_VERB_PAST);
	}

	if (Params.VerbStatus == TVerbParams::VS_POLITE)
	{
		result.insert(CONST_LESSON_VERB_SIMPLE_PRESENT_POLITE);
	}

	if (Params.VerbStatus == TVerbParams::VS_INFINITIVE)
	{
		result.insert(CONST_LESSON_VERB_INFINITIVE);
	}

	if (Params.VerbModificators.NegativePrefix)
	{
		result.insert(CONST_LESSON_VERB_NEGATIVE);
	}

	if (Params.VerbModificators.ImpossiblePrefix)
	{
		result.insert(CONST_LESSON_VERB_CAN);
	}
	return result;
}

std::vector<std::wstring> TVerb::Verbose() const
{
	std::vector<std::wstring> resultArr;

	for (auto& w : Itr->words)
	{
		std::wstring word;

		word = L"Глагол \"" + w.meaning + L"\"\n";

		word += Params.Verbose();

		resultArr.push_back(word);
	}

	return resultArr;
}

std::vector<std::wstring> TVerb::VerboseWithoutVerbStatus() const
{
	std::vector<std::wstring> resultArr;

	for (auto& w : Itr->words)
	{
		std::wstring word;

		word = L"Глагол \"" + w.meaning + L"\"\n";

		word += Params.VerbModificators.Verbose();

		resultArr.push_back(word);
	}

	return resultArr;
}

//========================================================

std::set<std::wstring> TSimpleWord::GetRelatedLessons() const
{
	return Itr->relatedLessons;
}

std::vector<std::wstring> TSimpleWord::Verbose() const
{
	std::vector<std::wstring> resultArr;

	for (auto& w : Itr->words)
	{
		std::wstring result;

		result = L"Слово \"" + w.meaning + L"\"\n";

		resultArr.push_back(result);
	}
	return resultArr;
}

//========================================================


TComplexVerb::TComplexVerb()
	: ComplexVerbType(CVT_NONE)
{

}

TComplexVerb::TComplexVerb(const TVerb& v)
	: ComplexVerbType(CVT_SINGLE_VERB)
	, LastVerb(v)
{
}

std::set<std::wstring> TComplexVerb::GetRelatedLessons() const
{
	std::set<std::wstring> result = LastVerb.GetRelatedLessons();

	if (InnerComplexVerb != 0)
	{
		std::set<std::wstring> complexVerbLessons = InnerComplexVerb->GetRelatedLessons();

		result.insert(complexVerbLessons.begin(), complexVerbLessons.end());

		if (ComplexVerbCase != CVC_NONE)
		{
			result.insert(THangulProcessor::ComplexVerbTestMap[ComplexVerbCase].lesson);
			
		}
	}

	return result;
}


std::vector<std::wstring> TComplexVerb::Verbose() const
{
	std::vector<std::wstring> resultArr;

	std::vector<std::wstring> innerComplexVerbResultArr;

	if (InnerComplexVerb)
	{
		
		if (ComplexVerbCase != CVC_NONE)
		{
			return THangulProcessor::ComplexVerbTestMap[ComplexVerbCase].verboseFunction(InnerComplexVerb, LastVerb, MiddleWords);
		}	
	
	}
	else
	{
		std::vector<std::wstring> verbResultArr = LastVerb.Verbose();

		resultArr = verbResultArr;
	}

	return resultArr;
}

std::vector<std::wstring> TComplexVerb::VerboseWithoutVerbStatus() const
{
	std::vector<std::wstring> resultArr;

	std::vector<std::wstring> innerComplexVerbResultArr;

	if (InnerComplexVerb)
	{

		if (ComplexVerbCase != CVC_NONE)
		{
			return THangulProcessor::ComplexVerbTestMap[ComplexVerbCase].verboseFunction(InnerComplexVerb, LastVerb, MiddleWords);
		}

	}
	else
	{
		std::vector<std::wstring> verbResultArr = LastVerb.VerboseWithoutVerbStatus();

		resultArr = verbResultArr;
	}

	return resultArr;
}

//========================================================

std::set<std::wstring> TFutureTensePhrase::GetRelatedLessons() const
{
	std::set<std::wstring> result = ComplexVerb.GetRelatedLessons();

	result.insert(CONST_LESSON_VERB_FUTURE_POLITE);

	return result;
}

std::vector<std::wstring> TFutureTensePhrase::Verbose() const
{
	std::vector<std::wstring> resultArr;

	std::vector<std::wstring> complexVerbResultArr = ComplexVerb.Verbose();

	for (auto complexVerbResult : complexVerbResultArr)
	{
		if (FutureTenseType == TFutureTensePhrase::FTT_PANMAL)
		{
			complexVerbResult += L"В будущем времени, в просторечной форме\n";
			resultArr.push_back(complexVerbResult);
		}
		else if (FutureTenseType == TFutureTensePhrase::FTT_POLITE)
		{
			complexVerbResult += L"В будущем времени, в вежливой форме\n";
			resultArr.push_back(complexVerbResult);
		}
		else if (FutureTenseType == TFutureTensePhrase::FTT_HONORIFIC)
		{
			complexVerbResult += L"В будущем времени, в почтительной форме\n";
			resultArr.push_back(complexVerbResult);
		}
		else if (FutureTenseType == TFutureTensePhrase::FTT_HONORIFIC_INTERROGATIVE)
		{
			complexVerbResult += L"В будущем времени, в вопросительной почтительной форме\n";
			resultArr.push_back(complexVerbResult);
		}
	}

	return resultArr;

}

TFutureTensePhrase::TFutureTenseType TFutureTensePhrase::RecognizeFutureTenseType(const std::wstring& word)
{
	if (word == L"거야")
	{
		return FTT_PANMAL;
	}
	else if (word == L"거예요")
	{
		return FTT_POLITE;
	}
	else if (word == L"겁니다")
	{
		return FTT_HONORIFIC;
	}
	else if (word == L"겁니까")
	{
		return FTT_HONORIFIC_INTERROGATIVE;
	}

	return FTT_NONE;

}

//========================================================


THangulDictionary::THangulDictionary()
{

	//NounTable.push_back(CreateWord(L"나", L"Я"));
	//NounTable.push_back(CreateWord(L"저", L"Я, то"));
	//NounTable.push_back(CreateWord(L"너", L"Ты"));
	//NounTable.push_back(CreateWord(L"그", L"Он, вон то"));
	//NounTable.push_back(CreateWord(L"그녀", L"Она"));
	NounTable.push_back(CreateWord(L"뭐", L"Что"));
	NounTable.push_back(CreateWord(L"뭣", L"Что"));
	NounTable.push_back(CreateWord(L"무엇", L"Что"));
	//NounTable.push_back(CreateWord(L"이것", L"Это"));
	//NounTable.push_back(CreateWord(L"이거", L"Это"));
	//NounTable.push_back(CreateWord(L"이", L"Это"));
	//NounTable.push_back(CreateWord(L"저것", L"То"));
	//NounTable.push_back(CreateWord(L"저거", L"То"));
	//NounTable.push_back(CreateWord(L"그것", L"Вон то"));
	//NounTable.push_back(CreateWord(L"그거", L"Вон то"));
	NounTable.push_back(CreateWord(L"누난", L"Старшая сестра или знакомая немного старше тебя (в именительном падеже в роли темы)"));
	

	AdverbTable.push_back(CreateWord(L"좀", L"Немножко"));
	AdverbTable.push_back(CreateWord(L"조금", L"Немножко"));
	AdverbTable.push_back(CreateWord(L"지금", L"Сейчас"));
	AdverbTable.push_back(CreateWord(L"잘", L"Хорошо"));
	AdverbTable.push_back(CreateWord(L"오늘", L"Сегодня"));
	AdverbTable.push_back(CreateWord(L"내일", L"Завтра"));
	AdverbTable.push_back(CreateWord(L"아까", L"Недавно"));
	AdverbTable.push_back(CreateWord(L"어제", L"Вчера"));

	SpecialTable.push_back(CreateWord(L"괜찮아", L"OK"));
	SpecialTable.push_back(CreateWord(L"그래", L"Да"));
	//SpecialTable.push_back(CreateWord(L"내가", L"Я"));
	//SpecialTable.push_back(CreateWord(L"네가", L"Ты"));
	//SpecialTable.push_back(CreateWord(L"니가", L"Ты"));
	SpecialTable.push_back(CreateWord(L"네", L"Твой")); //Xperimental
	SpecialTable.push_back(CreateWord(L"내", L"Мой"));
	//SpecialTable.push_back(CreateWord(L"날", L"Меня"));
	//SpecialTable.push_back(CreateWord(L"널", L"Тебя"));
	//SpecialTable.push_back(CreateWord(L"난", L"Я"));
	//SpecialTable.push_back(CreateWord(L"넌", L"Ты"));

	VerbTable.push_back(CreateWord(L"되다", L"Быть функционирующим, быть возможным"));

	LoadNounFromFile("1000_noun.txt");
	LoadVerbFromFile("1000_verb.txt");
	LoadSpecialFromFile("1000_special.txt");

	AllWords.insert(AllWords.end(), NounTable.begin(), NounTable.end());
	AllWords.insert(AllWords.end(), VerbTable.begin(), VerbTable.end());
	AllWords.insert(AllWords.end(), AdverbTable.begin(), AdverbTable.end());
	AllWords.insert(AllWords.end(), SpecialTable.begin(), SpecialTable.end());

	/*
	for (size_t i = 0; i < AllWords.size(); i++)
	{
	if (AllWords[i].base == L"")
	{
	std::cout << L"Oops!" << std::endl;
	}
	}
	*/
	std::sort(AllWords.begin(), AllWords.end(),
		[](const TWordRecord& first, const TWordRecord& second)
	{
		return first.priorityNum < second.priorityNum;
	}
	);

	RandomGenerator.seed(static_cast<unsigned int>(std::time(0)));
}

bool THangulDictionary::FindVerb(const std::vector<TSyllable>& verb, std::vector<TWordRecord>::const_iterator& result)
{
	for (result = VerbTable.begin(); result != VerbTable.end(); result++)
	{
		if (result->base == THangulProcessor::SyllableArrToWstring(verb))
		{
			return true;
		}
	}
	result = VerbTable.end();
	return false;

}


bool THangulDictionary::FindNoun(const std::vector<TSyllable>& plainNoun, std::vector<TWordRecord>::const_iterator& result)
{
	for (result = NounTable.begin(); result != NounTable.end(); result++)
	{
		if (result->base == THangulProcessor::SyllableArrToWstring(plainNoun))
		{
			return true;
		}
	}
	result = NounTable.end();
	return false;

}

bool THangulDictionary::FindAdverb(const std::vector<TSyllable>& adverb, std::vector<TWordRecord>::const_iterator& result)
{

	for (result = AdverbTable.begin(); result != AdverbTable.end(); result++)
	{
		if (result->base == THangulProcessor::SyllableArrToWstring(adverb))
		{

			return true;
		}
	}

	result = AdverbTable.end();
	return false;

}

bool THangulDictionary::FindSpecial(const std::vector<TSyllable>& special, std::vector<TWordRecord>::const_iterator& result)
{
	for (result = SpecialTable.begin(); result != SpecialTable.end(); result++)
	{
		if (result->base == THangulProcessor::SyllableArrToWstring(special))
		{
			return true;
		}
	}
	result = SpecialTable.end();
	return false;

}

void THangulDictionary::MergeWordToDictionary(std::vector<TWordRecord>& selectedTable, TWordRecord wordRecord)
{
	bool wordExists = false;
	for (auto itr = selectedTable.begin(); itr != selectedTable.end(); itr++)
	{
		if (itr->base == wordRecord.base)
		{
			itr->words.insert(wordRecord.words.begin(), wordRecord.words.end());
			wordExists = true;
		}
	}

	if (!wordExists)
	{
		selectedTable.push_back(wordRecord);
	}
}

void THangulDictionary::LoadVerbFromFile(const std::string& fileName)
{

	try
	{
#ifdef _WIN32
		std::ifstream f("C:/Workplace/Projects/hallyu/hallyu_server/" + fileName);

#else
		std::ifstream f("/home/devuser/workplace/hallyu_server/" + fileName);
#endif

		std::string line;
		std::wstring wline;

		if (f.is_open())
		{

			getline(f, line); //Skip one line

			while (getline(f, line))
			{

				wline = SE::string_to_wstring(line);
				TWordRecord wordRecord(wline);

				MergeWordToDictionary(VerbTable, wordRecord);


			}
			f.close();
		}
		else
		{
			std::cout << "file not found!" << std::endl;
		}

	}
	catch (std::exception e)
	{
		SE::WriteToLog("Error in THangulDictionary::LoadVerbFromFile");
	}


}


void THangulDictionary::LoadNounFromFile(const std::string& fileName)
{

	try
	{

#ifdef _WIN32
		std::ifstream f("C:/Workplace/Projects/hallyu/hallyu_server/" + fileName);

#else
		std::ifstream f("/home/devuser/workplace/hallyu_server/" + fileName);
#endif

		std::string line;
		std::wstring wline;

		if (f.is_open())
		{

			getline(f, line); //Skip one line

			std::cout << "File found!" << std::endl;
			while (getline(f, line))
			{

				wline = SE::string_to_wstring(line);
				TWordRecord wordRecord(wline);

				MergeWordToDictionary(NounTable, wordRecord);


			}
			f.close();
		}
		else
		{
			std::cout << "file not found!" << std::endl;
		}

	}
	catch (std::exception e)
	{
		SE::WriteToLog("Error in THangulDictionary::LoadNounFromFile");
	}


}


void THangulDictionary::LoadSpecialFromFile(const std::string& fileName)
{

	try
	{
#ifdef _WIN32
		std::ifstream f("C:/Workplace/Projects/hallyu/hallyu_server/" + fileName);

#else
		std::ifstream f("/home/devuser/workplace/hallyu_server/" + fileName);
#endif

		std::string line;
		std::wstring wline;

		if (f.is_open())
		{

			getline(f, line); //Skip one line

			std::cout << "File found!" << std::endl;
			while (getline(f, line))
			{

				wline = SE::string_to_wstring(line);
				TWordRecord wordRecord(wline);

				MergeWordToDictionary(SpecialTable, wordRecord);


			}
			f.close();
		}
		else
		{
			std::cout << "file not found!" << std::endl;
		}

	}
	catch (std::exception e)
	{
		SE::WriteToLog("Error in THangulDictionary::LoadSpecialFromFile");
	}
}



void THangulDictionary::GetRandomWord(size_t wordCount, std::wstring& translation, std::wstring& word0, std::wstring& word1, std::wstring& word2, std::wstring& word3, size_t& correctWord)
{
	boost::random::uniform_int_distribution<> random(0, std::min(wordCount, AllWords.size()) - 1);

	boost::random::uniform_int_distribution<> randomFour(0, 3);
	std::vector<int> selectedWords;

	do
	{
		int selected = random(RandomGenerator);

		if (std::find(selectedWords.begin(), selectedWords.end(), selected) == selectedWords.end())
		{
			selectedWords.push_back(selected);
		}
	} while (selectedWords.size() < 4);

	size_t selected = randomFour(RandomGenerator);

	correctWord = selected;

	word0 = AllWords[selectedWords[0]].base;
	word1 = AllWords[selectedWords[1]].base;
	word2 = AllWords[selectedWords[2]].base;
	word3 = AllWords[selectedWords[3]].base;

	translation = L"";

	for (auto itr = AllWords[selectedWords[selected]].words.begin(); itr != AllWords[selectedWords[selected]].words.end(); itr++)
	{
		translation += itr->meaning;
		auto next_itr = itr;
		next_itr++;
		if (next_itr != AllWords[selectedWords[selected]].words.end())
		{
			translation += L", ";
		}
	}


}


void THangulDictionary::GetRandomChineseNumber(int maxDigits, unsigned long long& number, std::wstring& numberStr)
{
	maxDigits = std::max<int>(1, maxDigits);
	maxDigits = std::min<int>(9, maxDigits);
	boost::random::uniform_int_distribution<> randomNumberOfDigits(1, maxDigits);

	int digits = randomNumberOfDigits(RandomGenerator);

	unsigned long long minBorder = 1;

	for (int i = 0; i < digits - 1; i++)
	{
		minBorder *= 10;
	}
	unsigned long long maxBorder = minBorder * 10;

	boost::random::uniform_int_distribution<> random(minBorder, maxBorder);

	number = random(RandomGenerator);

	numberStr = UintToChineseNumberString(number);
}



//===================================================


THangulProcessor::THangulProcessor()
{
	GenerateNounConditionMap();
	GenerateVerbConditionMap();
	GenerateComplexVerbConditionMap();
}

std::wstring THangulProcessor::SplitSyllableToChamo(wchar_t syllable)
{
	if (syllable >= 0x1100 && syllable <= 0x11FF)
	{
		//Already a chamo
		return std::wstring(1, syllable);
	}

	TSyllable result(syllable);

	return result.ToLineString();
}

std::vector<TSyllable> THangulProcessor::WstringToSyllableArr(const std::wstring& wstr)
{
	std::vector<TSyllable> result;

	for (auto& wchr : wstr)
	{
		result.push_back(TSyllable(wchr));
	}
	return result;
}

std::wstring THangulProcessor::SyllableArrToWstring(const std::vector<TSyllable>& syllableArr)
{
	std::wstring result;

	for (auto& syl : syllableArr)
	{
		result += syl.ToUnicodeSyllable();
	}
	return result;
}

std::vector<TSyllable> THangulProcessor::GetFirstVerbForm(const std::vector<TSyllable>& infinitiveVerb)
{
	if (infinitiveVerb.back() != CONST_VERB_END_SYLLABLE)
	{
		SE::WriteToLog("THangulProcessor::GetFirstVerbForm: given verb is not verb or not in infinitive form");
		//throw SE::ErrorToLog("given verb is not verb or not in infinitive form");
		return std::vector<TSyllable>();
	}
	return std::vector<TSyllable>(infinitiveVerb.begin(), infinitiveVerb.end() - 1);
}

std::vector<TSyllable> THangulProcessor::GetSecondVerbForm(const std::vector<TSyllable>& infinitiveVerb)
{
	std::vector<TSyllable> firstForm = GetFirstVerbForm(infinitiveVerb);

	wchar_t t = L'ᅩ';

	if (CONST_DO_VERB_SYLLABLE == firstForm.back())
	{
		firstForm.pop_back();
		firstForm.push_back(CONST_DO_VERB_PT_SYLLABLE);
	}
	else if (CONST_FUNCTION_VERB_SYLLABLE == firstForm.back())
	{
		firstForm.pop_back();
		firstForm.push_back(CONST_FUNCTION_VERB_PT_SYLLABLE);
	}
	else if (CONST_FORBID_VERB_SYLLABLE == firstForm.back())
	{
		firstForm.pop_back();
		firstForm.push_back(CONST_FORBID_VERB_PT_SYLLABLE);
	}
	else if (firstForm.back().middleChamo == L'ᅡ' || firstForm.back().middleChamo == L'ᅩ') //Xperimental
	{
		firstForm.push_back(L'아');
	}
	else
	{
		firstForm.push_back(L'어');
	}

	GlueEndingForVerbInSecondFormIfPossible(firstForm);

	return firstForm;
}


bool THangulProcessor::ChamoIsVowel(wchar_t chamo)
{
	return chamo >= 0x1161 && chamo <= 0x1175;
}


THangulProcessor::ChamoType THangulProcessor::GetChamoType(wchar_t chamo)
{

	if (ChamoIsVowel(chamo))
	{
		SE::WriteToLog("THangulProcessor::GetChamoType: chamo is obsolete or incorrect");
		return CT_VOWEL_LIGHT;
	}

	if (CONST_LIGHT_VOWEL_CHAMO_SET.find(chamo) != CONST_LIGHT_VOWEL_CHAMO_SET.end())
	{
		return CT_VOWEL_LIGHT;
	}
	else
		if (CONST_NEUTRAL_VOWEL_CHAMO_SET.find(chamo) != CONST_NEUTRAL_VOWEL_CHAMO_SET.end())
		{
		return CT_VOWEL_NEUTRAL;
		}
		else
		{
			return CT_VOWEL_DARK;
		}
}


void THangulProcessor::GlueEndingForVerbInSecondFormIfPossible(std::vector<TSyllable>& verbInSecondForm)
{
	TSyllable lastSyl = verbInSecondForm.back();

	TSyllable rootSyl = *(verbInSecondForm.end() - 2);

	if (lastSyl.middleChamo == L'ᅡ')
	{
		if (!rootSyl.LastChamoExists())
		{
			if (rootSyl.middleChamo == L'ᅡ')
			{
				//가다 -> 가
				RemoveLastSyllable(verbInSecondForm);
				return;
			}

			if (rootSyl.middleChamo == L'ᅩ')
			{
				//보다 -> 봐
				RemoveLastSyllable(verbInSecondForm);
				verbInSecondForm.back().middleChamo = L'ᅪ';
				return;
			}
		}
	}
	else if (lastSyl.middleChamo == L'ᅥ')
	{
		if (!rootSyl.LastChamoExists())
		{
			if (rootSyl.middleChamo == L'ᅥ')
			{
				//??? Xperimental
				RemoveLastSyllable(verbInSecondForm);
				return;
			}

			if (rootSyl.middleChamo == L'ᅮ')
			{
				//??? Xperimental
				RemoveLastSyllable(verbInSecondForm);
				verbInSecondForm.back().middleChamo = L'ᅯ';
				return;
			}

			if (rootSyl.middleChamo == L'ᅵ')
			{
				//보이다 -> 보여
				RemoveLastSyllable(verbInSecondForm);
				verbInSecondForm.back().middleChamo = L'ᅧ';
				return;
			}
		}
	}
}

void THangulProcessor::UnGlueEndingForVerbInSecondFormIfPossible(std::vector<TSyllable>& verbInSecondForm)
{
	//Like GlueEndingForVerbInSecondFormIfPossible but works in opposite way

	if (verbInSecondForm.size() == 1 && verbInSecondForm.back() == CONST_FORBID_VERB_PT_SYLLABLE)
	{
		//This is verb 말다
		return;
	}

	TSyllable lastSyl = verbInSecondForm.back();

	if (!(lastSyl.firstChamo == L'ᄋ' && !lastSyl.LastChamoExists()))
	{
		//verb was glued. Need to unglue
		if (lastSyl.middleChamo == L'ᅧ')
		{
			verbInSecondForm.back().middleChamo = L'ᅵ';
			verbInSecondForm.push_back(L'어');
			return;
		}

		if (lastSyl.middleChamo == L'ᅯ')
		{
			verbInSecondForm.back().middleChamo = L'ᅮ';
			verbInSecondForm.push_back(L'어');
			return;
		}

		if (lastSyl.middleChamo == L'ᅪ')
		{
			verbInSecondForm.back().middleChamo = L'ᅩ';
			verbInSecondForm.push_back(L'아');
			return;
		}

		if (lastSyl.middleChamo == L'ᅥ')
		{
			verbInSecondForm.push_back(L'어');
			return;
		}

		if (lastSyl.middleChamo == L'ᅡ')
		{
			verbInSecondForm.push_back(L'아');
			return;
		}
	}
}

bool THangulProcessor::RemoveInfiniteEndingIfPossible(std::vector<TSyllable>& verbInInfinite, wchar_t& outNextChamo)
{
	if (verbInInfinite.size() > 0)
	{
		TSyllable& lastSyl = verbInInfinite.back();

		if (lastSyl == CONST_VERB_END_SYLLABLE)
		{
			verbInInfinite.pop_back();
			outNextChamo = L'ᄃ';
			return true;
		}
	}

	return false;
}

bool THangulProcessor::RemoveEndingForPastVerbIfPossible(std::vector<TSyllable>& verbInFirstForm, wchar_t& outNextChamo)
{
	if (verbInFirstForm.size() > 0)
	{

		TSyllable& lastSyl = verbInFirstForm.back();

		if (lastSyl.lastChamo == L'ᆻ')
		{
			if (lastSyl == CONST_DO_VERB_PAST_SYLLABLE)
			{
				verbInFirstForm.pop_back();
				verbInFirstForm.push_back(CONST_DO_VERB_SYLLABLE);

				outNextChamo = L'ᄒ';

				return true;
			}
			else if (lastSyl == CONST_FUNCTION_VERB_PAST_SYLLABLE)
			{
				verbInFirstForm.pop_back();
				verbInFirstForm.push_back(CONST_FUNCTION_VERB_SYLLABLE);

				outNextChamo = L'ᄃ';

				return true;
			}
			if (lastSyl == L'었' || lastSyl == L'았')
			{
				verbInFirstForm.pop_back();

				outNextChamo = L'ᄋ';

				return true;
			}
			if (lastSyl.middleChamo == L'ᅡ' || lastSyl.middleChamo == L'ᅥ')
			{
				if (lastSyl.firstChamo != L'ᄋ')
				{
					lastSyl.lastChamo = CONST_EMPTY_LAST_CHAMO;

					return true;
				}
			}

			if (lastSyl.middleChamo == L'ᅪ')
			{
				lastSyl.middleChamo = L'ᅩ'; //Xperimental! Might be both ᅩ or ᅪ
				lastSyl.lastChamo = CONST_EMPTY_LAST_CHAMO;
				return true;
			}

			if (lastSyl.middleChamo == L'ᅧ')
			{
				lastSyl.middleChamo = L'ᅵ'; //Xperimental! Might be both ᅩ or ᅧ
				lastSyl.lastChamo = CONST_EMPTY_LAST_CHAMO;
				return true;
			}

			if (lastSyl.middleChamo == L'ᅯ')
			{
				lastSyl.middleChamo = L'ᅮ'; //Xperimental! Might be both ᅩ or ᅯ
				lastSyl.lastChamo = CONST_EMPTY_LAST_CHAMO;
				return true;
			}
		}

	}

	return false;
}

bool THangulProcessor::RemoveEndingForWillVerbIfPossible(std::vector<TSyllable>& verbInFirstForm, wchar_t& outNextChamo)
{
	if (verbInFirstForm.size() > 0)
	{
		TSyllable& lastSyl = verbInFirstForm.back();

		if (lastSyl == L'겠')
		{
			verbInFirstForm.pop_back();

			outNextChamo = L'ᄀ';
			return true;
		}
	}

	return false;
}

bool THangulProcessor::RemoveSiModificatorForVerbIfPossible(std::vector<TSyllable>& verbInFirstForm, wchar_t& outNextChamo)
{
	if (verbInFirstForm.size() > 0)
	{
		TSyllable& lastSyl = verbInFirstForm.back();

		if (lastSyl == L'시')
		{
			verbInFirstForm.pop_back();

			if (verbInFirstForm.size() >= 2)
			{
				if (verbInFirstForm.back() == L'으')
				{
					if ((verbInFirstForm.end() - 2)->lastChamo != CONST_EMPTY_LAST_CHAMO)
					{
						verbInFirstForm.pop_back();
					}
				}
			}
			outNextChamo = L'ᄉ';
			return true;
		}
	}
	return false;
}

bool THangulProcessor::RemoveNegativePrefixForVerbIfPossible(std::vector<TSyllable>& verbInFirstForm, wchar_t& outNextChamo)
{
	if (verbInFirstForm.size() > 0)
	{
		TSyllable& lastSyl = verbInFirstForm.front();

		if (lastSyl == L'안')
		{
			verbInFirstForm.erase(verbInFirstForm.begin());

			return true;
		}
	}
	return false;
}

bool THangulProcessor::RemoveImpossiblePrefixForVerbIfPossible(std::vector<TSyllable>& verbInFirstForm, wchar_t& outNextChamo)
{
	if (verbInFirstForm.size() > 0)
	{
		TSyllable& lastSyl = verbInFirstForm.front();

		if (lastSyl == L'못')
		{
			verbInFirstForm.erase(verbInFirstForm.begin());

			return true;
		}
	}
	return false;
}


bool THangulProcessor::ReplacePoliteEndingToPanmalIfPossible(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 0)
	{
		TSyllable& lastSyl = verb.back();

		if (lastSyl == L'요')
		{
			verb.pop_back();
			if (verb.size() >= 2)
			{
				if (verb.back() == L'세')
				{
					verb.pop_back();
					verb.push_back(L'시');
					verb.push_back(L'어');
				}
			}

			return true;
		}
	}
	return false;
}


bool THangulProcessor::RemovePoliteEndingIfPossible(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (ReplacePoliteEndingToPanmalIfPossible(verb, outNextChamo))
	{
		return ExtractVerbFirstForm(verb, outNextChamo);
	}
	return false;
}

bool THangulProcessor::RemoveHonorificEndingIfPossible(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() >= 3)
	{
		TSyllable& lastSyl = *(verb.end() - 1);
		TSyllable& prevLastSyl = *(verb.end() - 2);

		if (prevLastSyl == L'니' && lastSyl == L'다')
		{
			verb.pop_back();
			verb.pop_back();
			if (verb.back() == L'습')
			{
				verb.pop_back();

				if (verb.size() >= 1 && verb.back().LastChamoExistsAndItIsNotL())
				{
					return true;
				}
			}
			else if (verb.back().lastChamo == L'ᆸ')
			{
				verb.back().lastChamo = CONST_EMPTY_LAST_CHAMO;

				outNextChamo = L'ᄇ';

				return true;
			}
		}

	}

	return false;
}

bool THangulProcessor::RemoveHonorificQuestionEndingIfPossible(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() >= 3)
	{
		TSyllable& lastSyl = *(verb.end() - 1);
		TSyllable& prevLastSyl = *(verb.end() - 2);

		if (prevLastSyl == L'니' && lastSyl == L'까')
		{
			verb.pop_back();
			verb.pop_back();
			if (verb.back() == L'습')
			{
				verb.pop_back();

				if (verb.size() >= 1 && !verb.back().LastChamoExistsAndItIsNotL())
				{
					return true;
				}
			}
			else if (verb.back().lastChamo == L'ᆸ')
			{
				verb.back().lastChamo = CONST_EMPTY_LAST_CHAMO;

				outNextChamo = L'ᄇ';

				return true;
			}
		}

	}

	return false;
}

bool THangulProcessor::RemoveModificatorIf(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 0)
	{
		TSyllable& lastSyl = verb.back();

		if (lastSyl == L'면')
		{
			verb.pop_back();

			if (verb.size() >= 2 && verb.back() == L'으')
			{
				verb.pop_back();

				if (verb.back().LastChamoExistsAndItIsNotL())
				{
					return true;
				}
			}
			else if (!verb.back().LastChamoExistsAndItIsNotL())
			{
				outNextChamo = L'ᄆ';
				return true;
			}
		}
	}

	return false;
}


bool THangulProcessor::RemoveModificatorAnd(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 0)
	{
		TSyllable& lastSyl = verb.back();

		if (lastSyl == L'고')
		{
			verb.pop_back();
			outNextChamo = L'ᄀ';
			return true;
		}
	}

	return false;
}

bool THangulProcessor::RemoveModificatorSo(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 0)
	{
		TSyllable& lastSyl = verb.back();

		if (lastSyl == L'서')
		{
			verb.pop_back();

			if (verb.size() >= 1)
			{
				outNextChamo = L'ᄉ';
				return ExtractVerbFirstForm(verb, outNextChamo);
			}
		}
	}

	return false;
}

bool THangulProcessor::RemoveModificatorChi(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 0)
	{
		TSyllable& lastSyl = verb.back();

		if (lastSyl == L'지')
		{
			verb.pop_back();
			outNextChamo = L'ᄌ';
			return true;
		}
	}

	return false;
}


bool THangulProcessor::RemoveModificatorAlthough(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 0)
	{
		TSyllable& lastSyl = verb.back();

		if (lastSyl == L'만')
		{
			verb.pop_back();

			if (verb.size() >= 1)
			{
				outNextChamo = L'ᄆ';
				return RemoveModificatorChi(verb, outNextChamo);
			}
		}

	}
	return false;
}

bool THangulProcessor::RemoveModificatorPastParticiple(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 0)
	{
		TSyllable& lastSyl = verb.back();

		if (lastSyl == L'은')
		{
			verb.pop_back();

			if (verb.size() >= 1)
			{
				if (verb.back().LastChamoExistsAndItIsNotL())
				{
					outNextChamo = L'ᄋ';
					return true;
				}
			}
		}
		else if (lastSyl.lastChamo == L'ᆫ')
		{
			lastSyl.lastChamo = CONST_EMPTY_LAST_CHAMO;
			outNextChamo = L'ᄂ';
			return true;
		}
	}

	return false;
}

bool THangulProcessor::RemoveModificatorPresentParticiple(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 0)
	{
		TSyllable& lastSyl = verb.back();

		if (lastSyl == L'는')
		{
			verb.pop_back();

			outNextChamo = L'ᄂ';

			return true;
		}
	}

	return false;
}

bool THangulProcessor::RemoveModificatorFutureParticiple(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 0)
	{
		TSyllable& lastSyl = verb.back();

		if (lastSyl == L'을')
		{
			verb.pop_back();

			if (verb.size() >= 1)
			{
				if (verb.back().LastChamoExistsAndItIsNotL())
				{
					outNextChamo = L'ᄋ';
					return true;
				}
			}
		}
		else if (lastSyl.lastChamo == L'ᆯ')
		{
			lastSyl.lastChamo = CONST_EMPTY_LAST_CHAMO;
			outNextChamo = L'ᄅ';
			return true;
		}
	}

	return false;
}

bool THangulProcessor::RemoveModificatorToo(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 0)
	{
		TSyllable& lastSyl = verb.back();

		if (lastSyl == L'도')
		{
			verb.pop_back();

			if (verb.size() >= 1)
			{
				outNextChamo = L'ᄃ';
				return ExtractVerbFirstForm(verb, outNextChamo);
			}
		}
	}

	return false;
}

bool THangulProcessor::RemoveModificatorReason(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() >= 3)
	{
		TSyllable& lastSyl = *(verb.end() - 1);
		TSyllable& prevLastSyl = *(verb.end() - 2);

		if (prevLastSyl == L'니' && lastSyl == L'까')
		{
			verb.pop_back();
			verb.pop_back();

			if (verb.size() >= 2 && verb.back() == L'으')
			{
				verb.pop_back();

				if (verb.back().LastChamoExistsAndItIsNotL())
				{
					outNextChamo = L'ᄋ';
					return true;
				}
			}
			else if (!verb.back().LastChamoExistsAndItIsNotL())
			{
				outNextChamo = L'ᄂ';
				return true;
			}
		}
	}

	return false;
}

bool THangulProcessor::RemoveModificatorBackground(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() >= 3)
	{
		TSyllable& lastSyl = *(verb.end() - 1);

		TSyllable& prevLastSyl = *(verb.end() - 2);

		//Xperimental -- need distinguish 은 and 는
		if (prevLastSyl == L'는' && lastSyl == L'데')
		{

			verb.pop_back();
			verb.pop_back();

			outNextChamo = L'ᄂ';

			return true;
		}

		if (prevLastSyl == L'은' && lastSyl == L'데')
		{

			verb.pop_back();
			verb.pop_back();

			outNextChamo = L'ᄋ';

			return true;

		}
	}
	return false;
}

/*
bool THangulProcessor::RemoveModificatorGi(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 1)
	{
		TSyllable& lastSyl = verb.back();

		if (lastSyl == L'기')
		{
			verb.pop_back();

			outNextChamo = L'ᄀ';

			return true;

		}
	}

	return false;
}
*/


bool THangulProcessor::RemoveModificatorPanmalQuestion(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 2)
	{
		TSyllable& lastSyl = *(verb.end() - 1);

		TSyllable& prevLastSyl = *(verb.end() - 2);


		if (prevLastSyl == L'니' && lastSyl == L'야')
		{
			verb.pop_back();
			verb.pop_back();

			outNextChamo = L'ᄂ';
			return true;

		}
	}

	if (verb.size() > 1)
	{
		TSyllable& lastSyl = verb.back();

		if (lastSyl == L'냐' || lastSyl == L'니')
		{
			verb.pop_back();

			outNextChamo = L'ᄂ';
			return true;

		}
	}

	return false;
}

bool THangulProcessor::RemoveModificatorExpressed(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 2)
	{
		TSyllable& lastSyl = *(verb.end() - 1);

		TSyllable& prevLastSyl = *(verb.end() - 2);


		if (prevLastSyl == L'네' && lastSyl == L'요')
		{
			verb.pop_back();
			verb.pop_back();
			outNextChamo = L'ᄂ';
			return true;

		}
	}

	return false;
}

bool THangulProcessor::RemoveDuringAction(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 2)
	{
		TSyllable& lastSyl = *(verb.end() - 1);

		TSyllable& prevLastSyl = *(verb.end() - 2);


		if (prevLastSyl == L'다' && lastSyl == L'가')
		{
			verb.pop_back();
			verb.pop_back();
			outNextChamo = L'ᄃ';
			return true;

		}
	}

	return false;
}


bool THangulProcessor::RemoveFarPastAction(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 1)
	{
		TSyllable& lastSyl = verb.back();

		if (lastSyl == L'던')
		{
			verb.pop_back();
			outNextChamo = L'ᄃ';
			return true;

		}
	}

	return false;
}


bool THangulProcessor::RemoveMultipleAction(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 2)
	{
		TSyllable& lastSyl = *(verb.end() - 1);

		TSyllable& prevLastSyl = *(verb.end() - 2);


		if (prevLastSyl == L'든' && lastSyl == L'지')
		{
			verb.pop_back();
			verb.pop_back();
			outNextChamo = L'ᄃ';
			return true;

		}
	}

	return false;
}

bool THangulProcessor::RemoveWhenAction(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 2)
	{
		TSyllable& lastSyl = *(verb.end() - 1);

		TSyllable& prevLastSyl = *(verb.end() - 2);


		if (prevLastSyl == L'을' && lastSyl == L'때')
		{
			verb.pop_back();
			verb.pop_back();

			if (verb.back().LastChamoExists())
			{
				outNextChamo = L'ᄋ';
				return true;
			}
		}
		else if (prevLastSyl.lastChamo == L'ᆯ' && lastSyl == L'때')
		{
			prevLastSyl.lastChamo = CONST_EMPTY_LAST_CHAMO;

			verb.pop_back();
			outNextChamo = L'ᄅ';
			return true;
		}
	}

	return false;
}


bool THangulProcessor::RemoveMention(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 3)
	{
		TSyllable& lastSyl = *(verb.end() - 1);

		TSyllable& prevLastSyl = *(verb.end() - 2);

		TSyllable& prevPrevLastSyl = *(verb.end() - 3);

		TSyllable& prevPrevPrevLastSyl = *(verb.end() - 4);


		if (prevPrevPrevLastSyl.LastChamoExistsAndItIsNotL() && prevPrevLastSyl == L'으' && prevLastSyl == L'라' && lastSyl == L'고')
		{
			verb.pop_back();
			verb.pop_back();
			verb.pop_back();
			outNextChamo = L'ᄋ';
			return true;

		}
	}
	if (verb.size() > 2)
	{
		TSyllable& lastSyl = *(verb.end() - 1);

		TSyllable& prevLastSyl = *(verb.end() - 2);

		TSyllable& prevPrevLastSyl = *(verb.end() - 3);

		if (!prevPrevLastSyl.LastChamoExistsAndItIsNotL() && prevLastSyl == L'라' && lastSyl == L'고')
		{
			verb.pop_back();
			verb.pop_back();
			//outNextChamo = L'ᄅ'; This is exception in ᄅ irregular
			return true;

		}
	}

	return false;
}

bool THangulProcessor::RemoveTendAction(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 3)
	{
		TSyllable& lastSyl = *(verb.end() - 1);

		TSyllable& prevLastSyl = *(verb.end() - 2);

		TSyllable& prevPrevLastSyl = *(verb.end() - 3);

		TSyllable& prevPrevPrevLastSyl = *(verb.end() - 4);


		if (prevPrevPrevLastSyl.LastChamoExistsAndItIsNotL() && prevPrevLastSyl == L'으' && prevLastSyl == L'려' && lastSyl == L'고')
		{
			verb.pop_back();
			verb.pop_back();
			verb.pop_back();
			outNextChamo = L'ᄋ';
			return true;

		}
	}
	if (verb.size() > 2)
	{
		TSyllable& lastSyl = *(verb.end() - 1);

		TSyllable& prevLastSyl = *(verb.end() - 2);

		TSyllable& prevPrevLastSyl = *(verb.end() - 3);

		if (!prevPrevLastSyl.LastChamoExistsAndItIsNotL() && prevLastSyl == L'려' && lastSyl == L'고')
		{
			verb.pop_back();
			verb.pop_back();
			//outNextChamo = L'ᄅ'; This is exception in ᄅ irregular
			return true;

		}
	}

	return false;
}


/*
bool THangulProcessor::RemoveImperativePolite(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 3)
	{
		TSyllable& lastSyl = *(verb.end() - 1);

		TSyllable& prevLastSyl = *(verb.end() - 2);

		TSyllable& prevPrevLastSyl = *(verb.end() - 3);

		TSyllable& prevPrevPrevLastSyl = *(verb.end() - 4);


		if (prevPrevPrevLastSyl.LastChamoExistsAndItIsNotL() && prevPrevLastSyl == L'으' && prevLastSyl == L'세' && lastSyl == L'요')
		{
			verb.pop_back();
			verb.pop_back();
			outNextChamo = L'ᄋ';
			return true;

		}
	}
	if (verb.size() > 2)
	{
		TSyllable& lastSyl = *(verb.end() - 1);

		TSyllable& prevLastSyl = *(verb.end() - 2);

		TSyllable& prevPrevLastSyl = *(verb.end() - 3);

		if (!prevPrevLastSyl.LastChamoExistsAndItIsNotL() && prevLastSyl == L'세' && lastSyl == L'요')
		{
			verb.pop_back();
			verb.pop_back();
			outNextChamo = L'ᄉ';
			return true;

		}
	}

	return false;
}
*/

bool THangulProcessor::RemoveTheMoreThe(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 3)
	{
		TSyllable& lastSyl = *(verb.end() - 1);

		TSyllable& prevLastSyl = *(verb.end() - 2);

		TSyllable& prevPrevLastSyl = *(verb.end() - 3);


		if (prevPrevLastSyl == L'을' && prevLastSyl == L'수' && lastSyl == L'록')
		{
			verb.pop_back();
			verb.pop_back();
			verb.pop_back();

			if (verb.back().LastChamoExistsAndItIsNotL())
			{
				outNextChamo = L'ᄋ';
				return true;
			}
		}
		else if (prevPrevLastSyl.lastChamo == L'ᆯ' && prevLastSyl == L'수' && lastSyl == L'록')
		{
			prevPrevLastSyl.lastChamo = CONST_EMPTY_LAST_CHAMO;

			verb.pop_back();
			verb.pop_back();
			if (verb.back().LastChamoExistsAndItIsNotL())
			{
				outNextChamo = L'ᄅ';
				return true;
			}
			return true;
		}
	}

	return false;
}

bool THangulProcessor::RemoveUm(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 1)
	{
		TSyllable& lastSyl = verb.back();

		if (lastSyl == L'음')
		{
			verb.pop_back();
			outNextChamo = L'ᄋ';
			return true;

		}
	}

	return false;
}

bool THangulProcessor::RemoveCha(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 1)
	{
		TSyllable& lastSyl = verb.back();

		if (lastSyl == L'자')
		{
			verb.pop_back();
			outNextChamo = L'ᄌ';
			return true;

		}
	}

	return false;
}

bool THangulProcessor::RemovePoliteSure(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 2)
	{
		TSyllable& lastSyl = *(verb.end() - 1);

		TSyllable& prevLastSyl = *(verb.end() - 2);


		if (prevLastSyl == L'지' && lastSyl == L'요')
		{
			verb.pop_back();
			verb.pop_back();
			outNextChamo = L'ᄌ';
			return true;

		}
	}

	if (verb.size() > 1)
	{
		TSyllable& lastSyl = verb.back();

		if (lastSyl == L'죠')
		{
			verb.pop_back();
			outNextChamo = L'ᄌ';
			return true;

		}
	}

	return false;
}

bool THangulProcessor::RemoveImperativePanmal(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 1)
	{
		TSyllable& lastSyl = verb.back();

		if (lastSyl == L'라')
		{
			verb.pop_back();
			outNextChamo = L'ᄅ';
			return ExtractVerbFirstForm(verb, outNextChamo);

		}
	}

	return false;
}


bool THangulProcessor::RemoveActionAsResult(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 2)
	{
		TSyllable& lastSyl = *(verb.end() - 1);

		TSyllable& prevLastSyl = *(verb.end() - 2);


		if (prevLastSyl == L'을' && lastSyl == L'게')
		{
			verb.pop_back();
			verb.pop_back();

			if (verb.back().LastChamoExistsAndItIsNotL())
			{
				outNextChamo = L'ᄋ';
				return true;
			}
		}
		else if (prevLastSyl.lastChamo == L'ᆯ' && lastSyl == L'게')
		{
			prevLastSyl.lastChamo = CONST_EMPTY_LAST_CHAMO;

			verb.pop_back();

			outNextChamo = L'ᄅ';

			return true;
		}
	}

	return false;
}

bool THangulProcessor::RemoveForbidShortPanmal(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 2)
	{
		TSyllable& lastSyl = *(verb.end() - 1);

		TSyllable& prevLastSyl = *(verb.end() - 2);


		if (prevLastSyl == L'지' && lastSyl == L'마')
		{
			verb.pop_back();
			verb.pop_back();
			outNextChamo = L'ᄌ';
			return true;
		}
	}

	return false;
}


bool THangulProcessor::RemoveForbidShortPolite(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 3)
	{
		TSyllable& lastSyl = *(verb.end() - 1);

		TSyllable& prevLastSyl = *(verb.end() - 2);

		TSyllable& prevPrevLastSyl = *(verb.end() - 3);


		if (prevPrevLastSyl == L'지' && prevLastSyl == L'마' && lastSyl == L'요')
		{
			verb.pop_back();
			verb.pop_back();
			verb.pop_back();
			outNextChamo = L'ᄌ';
			return true;
		}
	}

	return false;
}

bool THangulProcessor::RemoveForbidShortImperative(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 4)
	{
		TSyllable& lastSyl = *(verb.end() - 1);

		TSyllable& prevLastSyl = *(verb.end() - 2);

		TSyllable& prevPrevLastSyl = *(verb.end() - 3);

		TSyllable& prevPrevPrevLastSyl = *(verb.end() - 4);

		if (prevPrevPrevLastSyl == L'지' && prevPrevLastSyl == L'마' && prevLastSyl == L'세' && lastSyl == L'요')
		{
			verb.pop_back();
			verb.pop_back();
			verb.pop_back();
			verb.pop_back();
			outNextChamo = L'ᄌ';
			return true;
		}
	}

	return false;
}

bool THangulProcessor::RemoveGerund(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 1)
	{
		TSyllable& lastSyl = verb.back();

		if (lastSyl == L'기')
		{
			verb.pop_back();
			outNextChamo = L'ᄀ';
			return true;

		}
	}

	return false;
}

bool THangulProcessor::RemoveGerundToo(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 2)
	{
		TSyllable& lastSyl = *(verb.end() - 1);

		TSyllable& prevLastSyl = *(verb.end() - 2);


		if (prevLastSyl == L'기' && lastSyl == L'도')
		{
			verb.pop_back();
			verb.pop_back();
			outNextChamo = L'ᄀ';
			return true;
		}
	}

	return false;
}

bool THangulProcessor::RemoveGerundOnly(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 2)
	{
		TSyllable& lastSyl = *(verb.end() - 1);

		TSyllable& prevLastSyl = *(verb.end() - 2);


		if (prevLastSyl == L'기' && lastSyl == L'만')
		{
			verb.pop_back();
			verb.pop_back();
			outNextChamo = L'ᄀ';
			return true;
		}
	}

	return false;
}

bool THangulProcessor::RemoveDoItForMeShortPanmal(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 1)
	{
		TSyllable& lastSyl = verb.back();

		if (lastSyl == L'줘')
		{
			verb.pop_back();

			outNextChamo = L'ᄌ';
			return ExtractVerbFirstForm(verb, outNextChamo);
		
		}
	}

	return false;
}


bool THangulProcessor::RemoveDoItForMeShortPolite(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 2)
	{
		TSyllable& lastSyl = *(verb.end() - 1);

		TSyllable& prevLastSyl = *(verb.end() - 2);


		if (prevLastSyl == L'줘' && lastSyl == L'요')
		{
			verb.pop_back();
			verb.pop_back();
			outNextChamo = L'ᄌ';
			return ExtractVerbFirstForm(verb, outNextChamo);
		}
	}

	return false;
}


bool THangulProcessor::RemoveDoItForMeShortImperative(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 3)
	{
		TSyllable& lastSyl = *(verb.end() - 1);

		TSyllable& prevLastSyl = *(verb.end() - 2);

		TSyllable& prevPrevLastSyl = *(verb.end() - 2);


		if (prevPrevLastSyl == L'주' && prevLastSyl == L'세' && lastSyl == L'요')
		{
			verb.pop_back();
			verb.pop_back();
			verb.pop_back();
			outNextChamo = L'ᄌ';
			return ExtractVerbFirstForm(verb, outNextChamo);
		}
	}

	return false;
}


bool THangulProcessor::RemoveModificatorYa(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{
	if (verb.size() > 1)
	{
		TSyllable& lastSyl = verb.back();

		if (lastSyl == L'야')
		{
			verb.pop_back();

			outNextChamo = L'ᄋ';
			return ExtractVerbFirstForm(verb, outNextChamo);

		}
	}

	return false;
}


void THangulProcessor::GenerateComplexVerbConditionMap()
{
	ComplexVerbTestMap[TComplexVerb::CVC_AND] = { std::bind(&THangulProcessor::TestComplexVerbIsAnd, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), std::bind(&THangulProcessor::VerboseComplexVerbIsAnd, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), L"" };
	ComplexVerbTestMap[TComplexVerb::CVC_NEGATION] = { std::bind(&THangulProcessor::TestComplexVerbIsNegation, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), std::bind(&THangulProcessor::VerboseComplexVerbIsNegation, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), CONST_LESSON_VERB_NEGATIVE };
	ComplexVerbTestMap[TComplexVerb::CVC_WANT] = { std::bind(&THangulProcessor::TestComplexVerbIsWant, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), std::bind(&THangulProcessor::VerboseComplexVerbIsWant, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), CONST_LESSON_VERB_I_WANT_TO };
	ComplexVerbTestMap[TComplexVerb::CVC_CONTINUOUS] = { std::bind(&THangulProcessor::TestComplexVerbIsContinuous, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), std::bind(&THangulProcessor::VerboseComplexVerbIsContinuous, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), CONST_LESSON_VERB_CONTINUOUS };
	ComplexVerbTestMap[TComplexVerb::CVC_CONTINUOUS_NEGATIVE] = { std::bind(&THangulProcessor::TestComplexVerbIsContinuousNegative, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), std::bind(&THangulProcessor::VerboseComplexVerbIsContinuousNegative, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), CONST_LESSON_VERB_CONTINUOUS };

	ComplexVerbTestMap[TComplexVerb::CVC_FORBID] = { std::bind(&THangulProcessor::TestComplexVerbIsForbid, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), std::bind(&THangulProcessor::VerboseComplexVerbIsForbid, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), CONST_LESSON_VERB_FORBID };
	ComplexVerbTestMap[TComplexVerb::CVC_GERUND_TOO] = { std::bind(&THangulProcessor::TestComplexVerbIsGerundToo, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), std::bind(&THangulProcessor::VerboseComplexVerbIsGerundToo, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), CONST_LESSON_VERB_TOO };
	ComplexVerbTestMap[TComplexVerb::CVC_GERUND_ONLY] = { std::bind(&THangulProcessor::TestComplexVerbIsGerundOnly, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), std::bind(&THangulProcessor::VerboseComplexVerbIsGerundOnly, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), CONST_LESSON_VERB_ONLY };
	ComplexVerbTestMap[TComplexVerb::CVC_DO_IT_FOR_ME] = { std::bind(&THangulProcessor::TestComplexVerbIsDoItForMe, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), std::bind(&THangulProcessor::VerboseComplexVerbIsDoItForMe, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), CONST_LESSON_VERB_DO_IT_FOR_ME };
	
	ComplexVerbTestMap[TComplexVerb::CVC_SHOULD] = { std::bind(&THangulProcessor::TestComplexVerbIsShould, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), std::bind(&THangulProcessor::VerboseComplexVerbIsShould, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), L"" };
	ComplexVerbTestMap[TComplexVerb::CVC_SHOULD_VERBAL] = { std::bind(&THangulProcessor::TestComplexVerbIsShouldVerbal, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), std::bind(&THangulProcessor::VerboseComplexVerbIsShouldVerbal, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), L"" };

	ComplexVerbTestMap[TComplexVerb::CVC_CAN_DO] = { std::bind(&THangulProcessor::TestComplexVerbIsCanDo, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), std::bind(&THangulProcessor::VerboseComplexVerbIsCanDo, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), CONST_LESSON_VERB_CAN };
	ComplexVerbTestMap[TComplexVerb::CVC_CANT_DO] = { std::bind(&THangulProcessor::TestComplexVerbIsCantDo, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), std::bind(&THangulProcessor::VerboseComplexVerbIsCantDo, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), CONST_LESSON_VERB_CAN };

	//ComplexVerbTestMap[TComplexVerb::CVC_BE_GOOD_AT] = { std::bind(&THangulProcessor::TestComplexVerbIsBeGoodAt, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), std::bind(&THangulProcessor::VerboseComplexVerbIsBeGoodAt, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), L"" };
	//ComplexVerbTestMap[TComplexVerb::CVC_BE_UNABLE_AT] = { std::bind(&THangulProcessor::TestComplexVerbIsBeUnableAt, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), std::bind(&THangulProcessor::VerboseComplexVerbIsBeUnableAt, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), L"" };
	//ComplexVerbTestMap[TComplexVerb::CVC_BE_BAD_AT] = { std::bind(&THangulProcessor::TestComplexVerbIsBeBadAt, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), std::bind(&THangulProcessor::VerboseComplexVerbIsBeBadAt, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), L"" };

}


bool THangulProcessor::TestComplexVerbIsAnd(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	if (InnerComplexVerb != 0 && InnerComplexVerb->LastVerb.Params.VerbStatus == TVerbParams::VS_MOD_AND &&
		middleWordArr.size() == 0 &&
		!(TestComplexVerbIsContinuous(InnerComplexVerb, LastVerb, middleWordArr)) &&
		!(TestComplexVerbIsWant(InnerComplexVerb, LastVerb, middleWordArr)))
	{
		return true;
	}

	return false;
}



bool THangulProcessor::TestComplexVerbIsNegation(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	if (LastVerb.Itr->base == L"않다" && InnerComplexVerb != 0 && InnerComplexVerb->LastVerb.Params.VerbStatus == TVerbParams::VS_MOD_CHI && middleWordArr.size() == 0)
	{
		return true;
	}

	return false;
}

bool THangulProcessor::TestComplexVerbIsWant(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	if (LastVerb.Itr->base == L"싶다" && InnerComplexVerb != 0 && InnerComplexVerb->LastVerb.Params.VerbStatus == TVerbParams::VS_MOD_AND && middleWordArr.size() == 0)
	{
		return true;
	}

	return false;
}

bool THangulProcessor::TestComplexVerbIsContinuous(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	if (LastVerb.Itr->base == L"있다" && InnerComplexVerb != 0 && InnerComplexVerb->LastVerb.Params.VerbStatus == TVerbParams::VS_MOD_AND && middleWordArr.size() == 0)
	{
		return true;
	}

	return false;
}

bool THangulProcessor::TestComplexVerbIsContinuousNegative(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	if (LastVerb.Itr->base == L"없다" && InnerComplexVerb != 0 && InnerComplexVerb->LastVerb.Params.VerbStatus == TVerbParams::VS_MOD_AND && middleWordArr.size() == 0)
	{
		return true;
	}

	return false;
}

bool THangulProcessor::TestComplexVerbIsForbid(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	if (LastVerb.Itr->base == L"말다" && InnerComplexVerb != 0 && InnerComplexVerb->LastVerb.Params.VerbStatus == TVerbParams::VS_MOD_CHI && middleWordArr.size() == 0)
	{
		return true;
	}

	return false;
}

bool THangulProcessor::TestComplexVerbIsGerundToo(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	if (LastVerb.Itr->base == L"하다" && InnerComplexVerb != 0 && InnerComplexVerb->LastVerb.Params.VerbStatus == TVerbParams::VS_GERUND_TOO && middleWordArr.size() == 0)
	{
		return true;
	}

	return false;
}

bool THangulProcessor::TestComplexVerbIsGerundOnly(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	if (LastVerb.Itr->base == L"하다" && InnerComplexVerb != 0 && InnerComplexVerb->LastVerb.Params.VerbStatus == TVerbParams::VS_GERUND_ONLY && middleWordArr.size() == 0)
	{
		return true;
	}

	return false;
}


bool THangulProcessor::TestComplexVerbIsDoItForMe(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	if (LastVerb.Itr->base == L"주다" 
		&& middleWordArr.size() == 0
		&& InnerComplexVerb != 0
		&& InnerComplexVerb->LastVerb.Params.VerbStatus == TVerbParams::VS_PANMAL 
		&& InnerComplexVerb->LastVerb.Params.VerbModificators.TenseModificator == TVerbModificators::TM_NONE
		&& !InnerComplexVerb->LastVerb.Params.VerbModificators.SiModificator)
	{
		return true;
	}

	return false;
}

bool THangulProcessor::TestComplexVerbIsShould(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	if (LastVerb.Itr->base == L"하다"
		&& middleWordArr.size() == 0
		&& InnerComplexVerb != 0
		&& InnerComplexVerb->LastVerb.Params.VerbStatus == TVerbParams::VS_MOD_YA
		&& InnerComplexVerb->LastVerb.Params.VerbModificators.TenseModificator == TVerbModificators::TM_NONE
		&& !InnerComplexVerb->LastVerb.Params.VerbModificators.SiModificator)
	{
		return true;
	}

	return false;
}

bool THangulProcessor::TestComplexVerbIsShouldVerbal(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	if (LastVerb.Itr->base == L"되다"
		&& middleWordArr.size() == 0
		&& InnerComplexVerb != 0
		&& InnerComplexVerb->LastVerb.Params.VerbStatus == TVerbParams::VS_MOD_YA
		&& InnerComplexVerb->LastVerb.Params.VerbModificators.TenseModificator == TVerbModificators::TM_NONE
		&& !InnerComplexVerb->LastVerb.Params.VerbModificators.SiModificator)
	{
		return true;
	}

	return false;
}

bool THangulProcessor::TestComplexVerbIsCanDo(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	if (LastVerb.Itr->base == L"있다"
		&& InnerComplexVerb != 0
		&& InnerComplexVerb->LastVerb.Params.VerbStatus == TVerbParams::VS_FUTURE_PARTICIPLE
		&& middleWordArr.size() == 1
		&& middleWordArr[0] == L"수")
	{
		return true;
	}

	return false;
}

bool THangulProcessor::TestComplexVerbIsCantDo(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	if (LastVerb.Itr->base == L"없다"
		&& InnerComplexVerb != 0
		&& InnerComplexVerb->LastVerb.Params.VerbStatus == TVerbParams::VS_FUTURE_PARTICIPLE
		&& middleWordArr.size() == 1
		&& middleWordArr[0] == L"수")
	{
		return true;
	}

	return false;
}



/*
static bool TestComplexVerbIsBeGoodAt(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
static bool TestComplexVerbIsBeUnableAt(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
static bool TestComplexVerbIsBeBadAt(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr);
*/

std::vector<std::wstring> THangulProcessor::VerboseComplexVerbIsAnd(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	std::vector<std::wstring> result;

	std::vector<std::wstring> innerComplexVerbResultArr = InnerComplexVerb->VerboseWithoutVerbStatus();

	std::vector<std::wstring> verbResultArr = LastVerb.Verbose();
	
	for (auto complexVerbResult : innerComplexVerbResultArr)
	{

		for (auto& verbResult : verbResultArr)
		{
			std::wstring resultRecord = complexVerbResult + L"Соединенный с:\n" + verbResult;
			resultRecord += LastVerb.Params.Verbose();
			result.push_back(resultRecord);
		}

	}
	return result;
}

std::vector<std::wstring> THangulProcessor::VerboseComplexVerbIsNegation(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	std::vector<std::wstring> result;

	std::vector<std::wstring> innerComplexVerbResultArr = InnerComplexVerb->VerboseWithoutVerbStatus();

	std::vector<std::wstring> verbResultArr = LastVerb.Verbose();

	for (auto complexVerbResult : innerComplexVerbResultArr)
	{

		std::wstring resultRecord = complexVerbResult + L"Поставленный в отрицательную форму\n";
		resultRecord += LastVerb.Params.Verbose();
		result.push_back(resultRecord);

	}

	return result;
}

std::vector<std::wstring> THangulProcessor::VerboseComplexVerbIsWant(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	std::vector<std::wstring> result;

	std::vector<std::wstring> innerComplexVerbResultArr = InnerComplexVerb->VerboseWithoutVerbStatus();

	std::vector<std::wstring> verbResultArr = LastVerb.Verbose();

	for (auto complexVerbResult : innerComplexVerbResultArr)
	{

		std::wstring resultRecord = complexVerbResult + L"Соединенный с глаголом \"Хотеть сделать\"\n";
		resultRecord += LastVerb.Params.Verbose();
		result.push_back(resultRecord);

	}

	return result;
}

std::vector<std::wstring> THangulProcessor::VerboseComplexVerbIsContinuous(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	std::vector<std::wstring> result;

	std::vector<std::wstring> innerComplexVerbResultArr = InnerComplexVerb->VerboseWithoutVerbStatus();

	std::vector<std::wstring> verbResultArr = LastVerb.Verbose();

	for (auto complexVerbResult : innerComplexVerbResultArr)
	{

		std::wstring resultRecord;

		resultRecord = complexVerbResult + L"В продолженном времени\n";
		resultRecord += LastVerb.Params.Verbose();
		result.push_back(resultRecord);

	}

	return result;
}

std::vector<std::wstring> THangulProcessor::VerboseComplexVerbIsContinuousNegative(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	std::vector<std::wstring> result;

	std::vector<std::wstring> innerComplexVerbResultArr = InnerComplexVerb->VerboseWithoutVerbStatus();

	std::vector<std::wstring> verbResultArr = LastVerb.Verbose();

	for (auto complexVerbResult : innerComplexVerbResultArr)
	{

		std::wstring resultRecord;


		resultRecord = complexVerbResult + L"В продолженном времени, с отрицанием\n";
		resultRecord += LastVerb.Params.Verbose();
		result.push_back(resultRecord);

	}

	return result;
}


std::vector<std::wstring> THangulProcessor::VerboseComplexVerbIsForbid(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	std::vector<std::wstring> result;

	std::vector<std::wstring> innerComplexVerbResultArr = InnerComplexVerb->VerboseWithoutVerbStatus();

	std::vector<std::wstring> verbResultArr = LastVerb.Verbose();

	for (auto complexVerbResult : innerComplexVerbResultArr)
	{

		std::wstring resultRecord = complexVerbResult + L"Соединенный с глаголом \"не делай это\"\n";
		resultRecord += LastVerb.Params.Verbose();
		result.push_back(resultRecord);

	}

	return result;
}

std::vector<std::wstring> THangulProcessor::VerboseComplexVerbIsGerundOnly(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	std::vector<std::wstring> result;

	std::vector<std::wstring> innerComplexVerbResultArr = InnerComplexVerb->VerboseWithoutVerbStatus();

	std::vector<std::wstring> verbResultArr = LastVerb.Verbose();

	for (auto complexVerbResult : innerComplexVerbResultArr)
	{

		std::wstring resultRecord = complexVerbResult + L"С модификатором \"делаю только это\"\n";
		resultRecord += LastVerb.Params.Verbose();
		result.push_back(resultRecord);

	}

	return result;
}


std::vector<std::wstring> THangulProcessor::VerboseComplexVerbIsGerundToo(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	std::vector<std::wstring> result;

	std::vector<std::wstring> innerComplexVerbResultArr = InnerComplexVerb->VerboseWithoutVerbStatus();

	std::vector<std::wstring> verbResultArr = LastVerb.Verbose();

	for (auto complexVerbResult : innerComplexVerbResultArr)
	{

		std::wstring resultRecord = complexVerbResult + L"С модификатором \"делаю это тоже\"\n";
		resultRecord += LastVerb.Params.Verbose();
		result.push_back(resultRecord);

	}

	return result;
}


std::vector<std::wstring> THangulProcessor::VerboseComplexVerbIsDoItForMe(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	std::vector<std::wstring> result;

	std::vector<std::wstring> innerComplexVerbResultArr = InnerComplexVerb->VerboseWithoutVerbStatus();

	std::vector<std::wstring> verbResultArr = LastVerb.Verbose();

	for (auto complexVerbResult : innerComplexVerbResultArr)
	{

		std::wstring resultRecord = complexVerbResult + L"С модификатором \"сделай это для меня\"\n";
		resultRecord += LastVerb.Params.Verbose();
		result.push_back(resultRecord);

	}

	return result;
}



std::vector<std::wstring> THangulProcessor::VerboseComplexVerbIsShould(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	std::vector<std::wstring> result;

	std::vector<std::wstring> innerComplexVerbResultArr = InnerComplexVerb->VerboseWithoutVerbStatus();

	std::vector<std::wstring> verbResultArr = LastVerb.Verbose();

	for (auto complexVerbResult : innerComplexVerbResultArr)
	{

		std::wstring resultRecord = complexVerbResult + L"С модификатором \"нужно сделать это\"\n";
		resultRecord += LastVerb.Params.Verbose();
		result.push_back(resultRecord);

	}

	return result;
}


std::vector<std::wstring> THangulProcessor::VerboseComplexVerbIsShouldVerbal(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	std::vector<std::wstring> result;

	std::vector<std::wstring> innerComplexVerbResultArr = InnerComplexVerb->VerboseWithoutVerbStatus();

	std::vector<std::wstring> verbResultArr = LastVerb.Verbose();

	for (auto complexVerbResult : innerComplexVerbResultArr)
	{

		std::wstring resultRecord = complexVerbResult + L"С модификатором \"нужно сделать это\" в разговорной форме\n";
		resultRecord += LastVerb.Params.Verbose();
		result.push_back(resultRecord);

	}

	return result;
}


std::vector<std::wstring> THangulProcessor::VerboseComplexVerbIsCanDo(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	std::vector<std::wstring> result;

	std::vector<std::wstring> innerComplexVerbResultArr = InnerComplexVerb->VerboseWithoutVerbStatus();

	std::vector<std::wstring> verbResultArr = LastVerb.Verbose();

	for (auto complexVerbResult : innerComplexVerbResultArr)
	{

		std::wstring resultRecord = complexVerbResult + L"С модификатором \"возможно сделать это\"\n";
		resultRecord += LastVerb.Params.Verbose();
		result.push_back(resultRecord);

	}

	return result;
}



std::vector<std::wstring> THangulProcessor::VerboseComplexVerbIsCantDo(std::shared_ptr<TComplexVerb> InnerComplexVerb, const TVerb& LastVerb, const std::vector<std::wstring>& middleWordArr)
{
	std::vector<std::wstring> result;

	std::vector<std::wstring> innerComplexVerbResultArr = InnerComplexVerb->VerboseWithoutVerbStatus();

	std::vector<std::wstring> verbResultArr = LastVerb.Verbose();

	for (auto complexVerbResult : innerComplexVerbResultArr)
	{

		std::wstring resultRecord = complexVerbResult + L"С модификатором \"невозможно сделать это\"\n";
		resultRecord += LastVerb.Params.Verbose();
		result.push_back(resultRecord);

	}

	return result;
}

void THangulProcessor::GenerateNounConditionMap()
{

	NounTestMap[TNounParams::NC_NONE] = { std::bind(&THangulProcessor::RemoveNothing, std::placeholders::_1), L"" };

	NounTestMap[TNounParams::NC_SUBJECT] = { std::bind(&THangulProcessor::RemoveSubjectCaseEndingIfPossible, std::placeholders::_1), L"В именительном падеже в роли субъекта\n", CONST_LESSON_NOUN_SUBJECT };
	NounTestMap[TNounParams::NC_TOPIC] = { std::bind(&THangulProcessor::RemoveTopicCaseEndingIfPossible, std::placeholders::_1), L"В именительном падеже в роли темы\n", CONST_LESSON_NOUN_TOPIC };
	NounTestMap[TNounParams::NC_GENITIVE] = { std::bind(&THangulProcessor::RemoveGenitiveCaseEndingIfPossible, std::placeholders::_1), L"В родительном падеже (кого? чего? Чей?)\n", CONST_LESSON_NOUN_GENITIVE };
	NounTestMap[TNounParams::NC_ACCUSATIVE] = { std::bind(&THangulProcessor::RemoveAccusativeCaseEndingIfPossible, std::placeholders::_1), L"В винительном падеже (кого? что?)\n", CONST_LESSON_NOUN_ACCUSATIVE };

	/*
	NounTestMap[TNounParams::NC_DATIVE_PLACE] = { std::bind(&THangulProcessor::RemoveDativePlaceCaseEndingIfPossible, std::placeholders::_1), L"В местном падеже (куда? где?)\n", CONST_LESSON_NOUN_DATIVE_PLACE };
	NounTestMap[TNounParams::NC_DATIVE_PERSON] = { std::bind(&THangulProcessor::RemoveDativePersonCaseEndingIfPossible, std::placeholders::_1), L"В местном падеже лица (кому? К кому? Для кого?) в письменной форме\n", CONST_LESSON_NOUN_DATIVE_PERSON };
	NounTestMap[TNounParams::NC_DATIVE_PERSON_VERBAL] = { std::bind(&THangulProcessor::RemoveDativePersonVerbalCaseEndingIfPossible, std::placeholders::_1), L"В местном падеже лица (кому? К кому? Для кого?) в разговорной форме\n", CONST_LESSON_NOUN_DATIVE_PERSON };
	NounTestMap[TNounParams::NC_DATIVE_PLACE_FROM] = { std::bind(&THangulProcessor::RemoveDativePlaceFromCaseEndingIfPossible, std::placeholders::_1), L"В местном исходящем падеже (откуда? где?)\n", CONST_LESSON_NOUN_DATIVE_PLACE };
	NounTestMap[TNounParams::NC_DATIVE_PERSON_FROM] = { std::bind(&THangulProcessor::RemoveDativePersonFromCaseEndingIfPossible, std::placeholders::_1), L"В местном исходящем падеже лица (от кого?) в письменной форме\n", CONST_LESSON_NOUN_DATIVE_PERSON };
	NounTestMap[TNounParams::NC_DATIVE_PERSON_FROM_VERBAL] = { std::bind(&THangulProcessor::RemoveDativePersonFromVerbalCaseEndingIfPossible, std::placeholders::_1), L"В местном исходящем падеже лица (от кого?) в разговорной форме\n", CONST_LESSON_NOUN_DATIVE_PERSON };
	NounTestMap[TNounParams::NC_DATIVE_PLACE_START_FROM] = { std::bind(&THangulProcessor::RemoveStartFromCaseEndingIfPossible, std::placeholders::_1), L"В местном падеже с обозначением места начала (откуда? От чего?)\n", CONST_LESSON_NOUN_FROM_TO };
	NounTestMap[TNounParams::NC_DATIVE_PLACE_END_TO] = { std::bind(&THangulProcessor::RemoveEndToCaseEndingIfPossible, std::placeholders::_1), L"В местном падеже с обозначением места конца (докуда? До чего?)\n", CONST_LESSON_NOUN_FROM_TO };
	*/
	NounTestMap[TNounParams::NC_LIKE] = { std::bind(&THangulProcessor::RemoveLikeCaseEndingIfPossible, std::placeholders::_1), L"С обозначением похожести, подобия (как кто? Как что?)\n" };
	NounTestMap[TNounParams::NC_AND] = { std::bind(&THangulProcessor::RemoveAndCaseEndingIfPossible, std::placeholders::_1), L"С союзом объединения И, в письменной форме\n", CONST_LESSON_NOUN_AND_WITH };
	NounTestMap[TNounParams::NC_AND_VERBAL] = { std::bind(&THangulProcessor::RemoveAndVerbalCaseEndingIfPossible, std::placeholders::_1), L"С союзом объединения И, в разговорной форме\n", CONST_LESSON_NOUN_AND_WITH };
	NounTestMap[TNounParams::NC_WITH] = { std::bind(&THangulProcessor::RemoveWithCaseEndingIfPossible, std::placeholders::_1), L"С союзом объединения (с кем? с чем?)\n", CONST_LESSON_NOUN_AND_WITH };


}

bool THangulProcessor::RemoveNothing(std::vector<TSyllable>& noun)
{
	if (noun.size() > 0)
	{
		return true;
	}
	return false;
}

bool THangulProcessor::RemoveSubjectCaseEndingIfPossible(std::vector<TSyllable>& noun)
{
	if (noun.size() > 1)
	{
		TSyllable& lastSyl = *(noun.end() - 1);
		TSyllable& prevLastSyl = *(noun.end() - 2);

		if ((lastSyl == L'은' && prevLastSyl.LastChamoExists()) ||
			(lastSyl == L'는' && !prevLastSyl.LastChamoExists()))
		{
			noun.pop_back();
			return true;
		}
	}

	return false;
}


bool THangulProcessor::RemoveTopicCaseEndingIfPossible(std::vector<TSyllable>& noun)
{
	if (noun.size() > 1)
	{
		TSyllable& lastSyl = *(noun.end() - 1);
		TSyllable& prevLastSyl = *(noun.end() - 2);

		if ((lastSyl == L'이' && prevLastSyl.LastChamoExists()) ||
			(lastSyl == L'가' && !prevLastSyl.LastChamoExists()))
		{
			noun.pop_back();
			return true;
		}
	}

	return false;

}

bool THangulProcessor::RemoveGenitiveCaseEndingIfPossible(std::vector<TSyllable>& noun)
{
	if (noun.size() > 1)
	{
		TSyllable& lastSyl = noun.back();

		if (lastSyl == L'의')
		{
			noun.pop_back();
			return true;
		}
	}

	return false;
}


bool THangulProcessor::RemoveAccusativeCaseEndingIfPossible(std::vector<TSyllable>& noun)
{
	if (noun.size() > 1)
	{
		TSyllable& lastSyl = *(noun.end() - 1);
		TSyllable& prevLastSyl = *(noun.end() - 2);

		//Xperimental -- need check situation when last chamo is r/l
		if ((lastSyl == L'을' && prevLastSyl.LastChamoExists()) ||
			(lastSyl == L'를' && !prevLastSyl.LastChamoExists()))
		{
			noun.pop_back();
			return true;
		}
	}

	return false;
}



bool THangulProcessor::RemoveLikeCaseEndingIfPossible(std::vector<TSyllable>& noun)
{
	if (noun.size() > 2)
	{
		TSyllable& lastSyl = *(noun.end() - 1);
		TSyllable& prevLastSyl = *(noun.end() - 2);

		if (prevLastSyl == L'처' && lastSyl == L'럼')
		{
			noun.pop_back();
			noun.pop_back();
			return true;
		}
	}

	return false;
}

bool THangulProcessor::RemoveAndCaseEndingIfPossible(std::vector<TSyllable>& noun)
{
	if (noun.size() > 1)
	{
		TSyllable& lastSyl = *(noun.end() - 1);
		TSyllable& prevLastSyl = *(noun.end() - 2);

		if ((lastSyl == L'과' && prevLastSyl.LastChamoExists()) ||
			(lastSyl == L'와' && !prevLastSyl.LastChamoExists()))
		{
			noun.pop_back();
			return true;
		}
	}

	return false;
}


bool THangulProcessor::RemoveAndVerbalCaseEndingIfPossible(std::vector<TSyllable>& noun)
{
	if (noun.size() > 2)
	{
		TSyllable& lastSyl = *(noun.end() - 1);
		TSyllable& prevLastSyl = *(noun.end() - 2);

		if (prevLastSyl == L'하' && lastSyl == L'고')
		{
			noun.pop_back();
			noun.pop_back();
			return true;
		}
	}

	return false;
}


bool THangulProcessor::RemoveWithCaseEndingIfPossible(std::vector<TSyllable>& noun)
{
	if (noun.size() > 1)
	{
		TSyllable& lastSyl = noun.back();

		if (lastSyl == L'랑')
		{
			noun.pop_back();
			return true;
		}
	}

	return false;
}

bool THangulProcessor::TryExtractNounModificator_Too(std::vector<TSyllable>& noun)
{
	if (noun.size() > 1)
	{
		TSyllable& lastSyl = noun.back();

		if (lastSyl == L'도')
		{
			noun.pop_back();
			return true;
		}
	}

	return false;
}

bool THangulProcessor::TryExtractNounModificator_Only(std::vector<TSyllable>& noun)
{
	if (noun.size() > 1)
	{
		TSyllable& lastSyl = noun.back();

		if (lastSyl == L'만')
		{
			noun.pop_back();
			return true;
		}
	}

	return false;
}

bool THangulProcessor::TryExtractNounModificator_Plural(std::vector<TSyllable>& noun)
{
	if (noun.size() > 1)
	{
		TSyllable& lastSyl = noun.back();

		if (lastSyl == L'들')
		{
			noun.pop_back();
			return true;
		}
	}

	return false;
}

/*

bool THangulProcessor::TryExtractNounModificator_Accusative(std::vector<TSyllable>& noun)
{
	if (noun.size() > 1)
	{
		TSyllable& lastSyl = *(noun.end() - 1);
		TSyllable& prevLastSyl = *(noun.end() - 2);

		if ((lastSyl == L'을' && prevLastSyl.LastChamoExists()) ||
			(lastSyl == L'를' && !prevLastSyl.LastChamoExists()))
		{
			noun.pop_back();
			return true;
		}
	}

	return false;
}
*/


bool THangulProcessor::RemoveDativePlaceCaseEndingIfPossible(std::vector<TSyllable>& noun)
{
	if (noun.size() > 1)
	{
		TSyllable& lastSyl = noun.back();

		if (lastSyl == L'에')
		{
			noun.pop_back();
			return true;
		}
	}

	return false;
}

bool THangulProcessor::RemoveDativePersonCaseEndingIfPossible(std::vector<TSyllable>& noun)
{
	if (noun.size() > 2)
	{
		TSyllable& lastSyl = *(noun.end() - 1);
		TSyllable& prevLastSyl = *(noun.end() - 2);

		if (prevLastSyl == L'에' && lastSyl == L'게')
		{
			noun.pop_back();
			noun.pop_back();
			return true;
		}
	}

	return false;
}

bool THangulProcessor::RemoveDativePersonVerbalCaseEndingIfPossible(std::vector<TSyllable>& noun)
{
	if (noun.size() > 2)
	{
		TSyllable& lastSyl = *(noun.end() - 1);
		TSyllable& prevLastSyl = *(noun.end() - 2);

		if (prevLastSyl == L'한' && lastSyl == L'테')
		{
			noun.pop_back();
			noun.pop_back();
			return true;
		}
	}

	return false;
}

bool THangulProcessor::RemoveDativePlaceFromCaseEndingIfPossible(std::vector<TSyllable>& noun)
{
	if (noun.size() > 2)
	{
		TSyllable& lastSyl = *(noun.end() - 1);
		TSyllable& prevLastSyl = *(noun.end() - 2);

		if (prevLastSyl == L'에' && lastSyl == L'서')
		{
			noun.pop_back();
			noun.pop_back();
			return true;
		}
	}

	return false;
}

bool THangulProcessor::RemoveDativePersonFromCaseEndingIfPossible(std::vector<TSyllable>& noun)
{
	if (noun.size() > 3)
	{
		TSyllable& lastSyl = *(noun.end() - 1);
		TSyllable& prevLastSyl = *(noun.end() - 2);
		TSyllable& prevPrevLastSyl = *(noun.end() - 3);

		if (prevPrevLastSyl == L'한' && prevLastSyl == L'테' && lastSyl == L'서')
		{
			noun.pop_back();
			noun.pop_back();
			noun.pop_back();
			return true;
		}
	}

	return false;
}

bool THangulProcessor::RemoveDativePersonFromVerbalCaseEndingIfPossible(std::vector<TSyllable>& noun)
{
	if (noun.size() > 3)
	{
		TSyllable& lastSyl = *(noun.end() - 1);
		TSyllable& prevLastSyl = *(noun.end() - 2);
		TSyllable& prevPrevLastSyl = *(noun.end() - 3);

		if (prevPrevLastSyl == L'에' && prevLastSyl == L'게' && lastSyl == L'서')
		{
			noun.pop_back();
			noun.pop_back();
			noun.pop_back();
			return true;
		}
	}

	return false;
}


bool THangulProcessor::RemoveStartFromCaseEndingIfPossible(std::vector<TSyllable>& noun)
{
	if (noun.size() > 2)
	{
		TSyllable& lastSyl = *(noun.end() - 1);
		TSyllable& prevLastSyl = *(noun.end() - 2);

		if (prevLastSyl == L'부' && lastSyl == L'터')
		{
			noun.pop_back();
			noun.pop_back();
			return true;
		}
	}

	return false;
}


bool THangulProcessor::RemoveEndToCaseEndingIfPossible(std::vector<TSyllable>& noun)
{
	if (noun.size() > 2)
	{
		TSyllable& lastSyl = *(noun.end() - 1);
		TSyllable& prevLastSyl = *(noun.end() - 2);

		if (prevLastSyl == L'까' && lastSyl == L'지')
		{
			noun.pop_back();
			noun.pop_back();
			return true;
		}
	}

	return false;
}


void THangulProcessor::RemoveLastSyllable(std::vector<TSyllable>& word)
{
	if (word.size() < 2)
	{
		SE::WriteToLog("THangulProcessor::RemoveLastSyllable: word is too short");
	}
	else
	{
		word.pop_back();
	}
}

bool THangulProcessor::ChamoAffectLIrregular(wchar_t chamo)
{
	return chamo == L'ᄂ' || chamo == L'ᄇ' || chamo == L'ᄉ' || chamo == L'ᄅ';
}

bool THangulProcessor::RecognizeVerbInInfinitive(std::vector<TSyllable> verb, TVerb& verbResult)
{
	return HangulDictionary.FindVerb(verb, verbResult.Itr);
}


bool THangulProcessor::RecognizeVerbInFirstForm(std::vector<TSyllable> verb, TVerb& verbResult)
{
	verb.push_back(CONST_VERB_END_SYLLABLE);

	return RecognizeVerbInInfinitive(verb, verbResult);
}

bool THangulProcessor::RecognizeVerbInFirstFormLIrregular(std::vector<TSyllable> verb, TVerb& verbResult)
{

	verb.back().lastChamo = L'ᆯ';
	verb.push_back(CONST_VERB_END_SYLLABLE);

	return RecognizeVerbInInfinitive(verb, verbResult);
}

bool THangulProcessor::RecognizeVerbInFirstFormU(std::vector<TSyllable> verb, TVerb& verbResult)
{

	if (!verb.back().LastChamoExists())
	{
		if (verb.back().middleChamo == L'ᅥ')
		{
			verb.back().middleChamo = L'ᅳ';
			verb.push_back(CONST_VERB_END_SYLLABLE);

			return RecognizeVerbInInfinitive(verb, verbResult);
		}
	}
	return false;
}


TVerbModificators THangulProcessor::RemoveBasicVerbModificators(std::vector<TSyllable>& verb, wchar_t& outNextChamo)
{

	TVerbModificators VerbModificators;
	bool pastTenseModificatorExist = RemoveEndingForPastVerbIfPossible(verb, outNextChamo);

	bool willTenseModificatorExist = RemoveEndingForWillVerbIfPossible(verb, outNextChamo);

	if (pastTenseModificatorExist)
	{
		VerbModificators.TenseModificator = TVerbModificators::TM_PAST;
	}
	else if (willTenseModificatorExist)
	{
		VerbModificators.TenseModificator = TVerbModificators::TM_WILL;
	}
	else
	{
		VerbModificators.TenseModificator = TVerbModificators::TM_NONE;
	}


	VerbModificators.SiModificator = RemoveSiModificatorForVerbIfPossible(verb, outNextChamo);

	VerbModificators.NegativePrefix = RemoveNegativePrefixForVerbIfPossible(verb, outNextChamo);

	VerbModificators.ImpossiblePrefix = RemoveImpossiblePrefixForVerbIfPossible(verb, outNextChamo);

	return VerbModificators;
}



bool THangulProcessor::ExtractVerbFirstForm(std::vector<TSyllable>& verbInSecondForm, wchar_t& outNextChamo)
{
	UnGlueEndingForVerbInSecondFormIfPossible(verbInSecondForm);

	TSyllable lastSyl = verbInSecondForm.back();

	if (lastSyl == CONST_DO_VERB_PT_SYLLABLE)
	{
		verbInSecondForm.pop_back();
		verbInSecondForm.push_back(CONST_DO_VERB_SYLLABLE);
		return true;
	}

	if (lastSyl == CONST_FUNCTION_VERB_PT_SYLLABLE)
	{
		verbInSecondForm.pop_back();
		verbInSecondForm.push_back(CONST_FUNCTION_VERB_SYLLABLE);
		return true;
	}

	if (lastSyl == CONST_FORBID_VERB_PT_SYLLABLE)
	{
		verbInSecondForm.pop_back();
		verbInSecondForm.push_back(CONST_FORBID_VERB_SYLLABLE);
		return true;
	}

	if (lastSyl == L'어' || lastSyl == L'아')
	{
		verbInSecondForm.pop_back();
		return true;
	}

	return false;
}

void THangulProcessor::GenerateVerbConditionMap()
{
	VerbTestMap[TVerbParams::VS_INFINITIVE] = { std::bind(&THangulProcessor::RemoveInfiniteEndingIfPossible, std::placeholders::_1, std::placeholders::_2), L"В инфинитиве\n", CONST_LESSON_VERB_INFINITIVE };

	VerbTestMap[TVerbParams::VS_PANMAL] = { std::bind(&THangulProcessor::ExtractVerbFirstForm, std::placeholders::_1, std::placeholders::_2), L"В просторечной форме\n", L"" };

	VerbTestMap[TVerbParams::VS_POLITE] = { std::bind(&THangulProcessor::RemovePoliteEndingIfPossible, std::placeholders::_1, std::placeholders::_2), L"В вежливой форме\n", CONST_LESSON_VERB_SIMPLE_PRESENT_POLITE };

	VerbTestMap[TVerbParams::VS_HONORIFIC] = { std::bind(&THangulProcessor::RemoveHonorificEndingIfPossible, std::placeholders::_1, std::placeholders::_2), L"В почтительной форме\n" };

	VerbTestMap[TVerbParams::VS_HONORIFIC_QUESTION] = { std::bind(&THangulProcessor::RemoveHonorificQuestionEndingIfPossible, std::placeholders::_1, std::placeholders::_2), L"В виде вопроса в почтительной форме\n" };

	VerbTestMap[TVerbParams::VS_MOD_IF] = { std::bind(&THangulProcessor::RemoveModificatorIf, std::placeholders::_1, std::placeholders::_2), L"С модификатором условия\n", CONST_LESSON_VERB_IF };

	VerbTestMap[TVerbParams::VS_MOD_AND] = { std::bind(&THangulProcessor::RemoveModificatorAnd, std::placeholders::_1, std::placeholders::_2), L"С соединительным окончанием. Допишите следующее слово, возможно вы получите более точный перевод\n" };

	VerbTestMap[TVerbParams::VS_MOD_SO] = { std::bind(&THangulProcessor::RemoveModificatorSo, std::placeholders::_1, std::placeholders::_2), L"С модификатором следствия \"и поэтому\"\n" };

	VerbTestMap[TVerbParams::VS_MOD_CHI] = { std::bind(&THangulProcessor::RemoveModificatorChi, std::placeholders::_1, std::placeholders::_2), L"С модификатором очевидности, точности. Допишите следующее слово, возможно вы получите более точный перевод\n" };

	VerbTestMap[TVerbParams::VS_MOD_ALTHOUGH] = { std::bind(&THangulProcessor::RemoveModificatorAlthough, std::placeholders::_1, std::placeholders::_2), L"С модификатором смысла \"хотя, несмотря на\"\n" };

	VerbTestMap[TVerbParams::VS_PAST_PARTICIPLE] = { std::bind(&THangulProcessor::RemoveModificatorPastParticiple, std::placeholders::_1, std::placeholders::_2), L"В форме причастия прошедшего времени. Допишите следующее слово, возможно вы получите более точный перевод\n", CONST_LESSON_VERB_PARTICIPLE };

	VerbTestMap[TVerbParams::VS_PRESENT_PARTICIPLE] = { std::bind(&THangulProcessor::RemoveModificatorPresentParticiple, std::placeholders::_1, std::placeholders::_2), L"В форме причастия настоящего времени. Допишите следующее слово, возможно вы получите более точный перевод\n", CONST_LESSON_VERB_PARTICIPLE };

	VerbTestMap[TVerbParams::VS_FUTURE_PARTICIPLE] = { std::bind(&THangulProcessor::RemoveModificatorFutureParticiple, std::placeholders::_1, std::placeholders::_2), L"В форме причастия будущего времени. Допишите следующее слово, возможно вы получите более точный перевод\n", CONST_LESSON_VERB_PARTICIPLE };

	VerbTestMap[TVerbParams::VS_TOO] = { std::bind(&THangulProcessor::RemoveModificatorToo, std::placeholders::_1, std::placeholders::_2), L"С модификатором смысла \"тоже, также\"\n", CONST_LESSON_VERB_TOO };

	VerbTestMap[TVerbParams::VS_REASON] = { std::bind(&THangulProcessor::RemoveModificatorReason, std::placeholders::_1, std::placeholders::_2), L"С модификатором причины\n" };

	VerbTestMap[TVerbParams::VS_BACKGROUND] = { std::bind(&THangulProcessor::RemoveModificatorBackground, std::placeholders::_1, std::placeholders::_2), L"С модификатором фонового действия\n" };



	//Duplicate
	//VerbTestMap[TVerbParams::VS_GI] = { std::bind(&THangulProcessor::RemoveModificatorGi, std::placeholders::_1, std::placeholders::_2), L"В субстантиве (делать -> делание).\n" };

	

	VerbTestMap[TVerbParams::VS_PANMAL_QUESTION] = { std::bind(&THangulProcessor::RemoveModificatorPanmalQuestion, std::placeholders::_1, std::placeholders::_2), L"В виде вопроса, в просторечной форме вежливости\n" };

	VerbTestMap[TVerbParams::VS_EXPRESSED] = { std::bind(&THangulProcessor::RemoveModificatorExpressed, std::placeholders::_1, std::placeholders::_2), L"В вежливой форме, с оттенком восторга, сюрприза, экспрессии\n" };

	VerbTestMap[TVerbParams::VS_DURING_ACTION] = { std::bind(&THangulProcessor::RemoveDuringAction, std::placeholders::_1, std::placeholders::_2), L"С модификатором \"делая это\", \"во время этого действия\"\n" };

	VerbTestMap[TVerbParams::VS_FAR_PAST_ACTION] = { std::bind(&THangulProcessor::RemoveFarPastAction, std::placeholders::_1, std::placeholders::_2), L"В форме причастия прошедшего времени. Обозначает действие, которое происходило когда-то в прошлом.\n" };

	VerbTestMap[TVerbParams::VS_MULTIPLE_ACTION] = { std::bind(&THangulProcessor::RemoveMultipleAction, std::placeholders::_1, std::placeholders::_2), L"С модификатором выбора из нескольких опций.\n" };

	VerbTestMap[TVerbParams::VS_WHEN_ACTION] = { std::bind(&THangulProcessor::RemoveWhenAction, std::placeholders::_1, std::placeholders::_2), L"С модификатором \"сделав это\", \"когда это действие сделано\"\n" };

	VerbTestMap[TVerbParams::VS_MENTION] = { std::bind(&THangulProcessor::RemoveMention, std::placeholders::_1, std::placeholders::_2), L"С модификатором упоминания, повторения\n" };

	VerbTestMap[TVerbParams::VS_TEND_ACTION] = { std::bind(&THangulProcessor::RemoveTendAction, std::placeholders::_1, std::placeholders::_2), L"С модификатором намерения \"чтобы сделать это\"\n" };


	//Duplicate
	//VerbTestMap[TVerbParams::VS_IMPERATIVE_POLITE] = { std::bind(&THangulProcessor::RemoveImperativePolite, std::placeholders::_1, std::placeholders::_2), L"В повелительном наклонении в вежливой форме\n" };

	//ololo

	VerbTestMap[TVerbParams::VS_THE_MORE_THE] = { std::bind(&THangulProcessor::RemoveTheMoreThe, std::placeholders::_1, std::placeholders::_2), L"С модификатором \"чем ... это, тем\"\n" };

	VerbTestMap[TVerbParams::VS_UM] = { std::bind(&THangulProcessor::RemoveUm, std::placeholders::_1, std::placeholders::_2), L"В субстантиве (делать -> делание), подразумевая действие, о свершении которого уже точно известно\n" };

	VerbTestMap[TVerbParams::VS_CHA] = { std::bind(&THangulProcessor::RemoveCha, std::placeholders::_1, std::placeholders::_2), L"В повелительном наклонении в просторечной форме\n" };

	VerbTestMap[TVerbParams::VS_POLITE_SURE] = { std::bind(&THangulProcessor::RemovePoliteSure, std::placeholders::_1, std::placeholders::_2), L"В вежливой форме с модификатором очевидности, точности\n" };

	VerbTestMap[TVerbParams::VS_IMPERATIVE_PANMAL] = { std::bind(&THangulProcessor::RemoveImperativePanmal, std::placeholders::_1, std::placeholders::_2), L"В повелительном наклонении в просторечной форме\n" };

	VerbTestMap[TVerbParams::VS_ACTION_AS_RESULT] = { std::bind(&THangulProcessor::RemoveActionAsResult, std::placeholders::_1, std::placeholders::_2), L"С модификатором действия, которое является следствием каких-то предыдущих действий - \"раз так, то ...\"\n" };

	VerbTestMap[TVerbParams::VS_FORBID_SHORT_PANMAL] = { std::bind(&THangulProcessor::RemoveForbidShortPanmal, std::placeholders::_1, std::placeholders::_2), L"Соединенный с глаголом \"не делай это\" в просторечной форме\n", CONST_LESSON_VERB_FORBID };
	VerbTestMap[TVerbParams::VS_FORBID_SHORT_POLITE] = { std::bind(&THangulProcessor::RemoveForbidShortPolite, std::placeholders::_1, std::placeholders::_2), L"Соединенный с глаголом \"не делай это\" в вежливой форме\n", CONST_LESSON_VERB_FORBID };
	VerbTestMap[TVerbParams::VS_FORBID_SHORT_IMPERATIVE] = { std::bind(&THangulProcessor::RemoveForbidShortImperative, std::placeholders::_1, std::placeholders::_2), L"Соединенный с глаголом \"не делай это\" в почтительной форме\n", CONST_LESSON_VERB_FORBID };


	VerbTestMap[TVerbParams::VS_GERUND] = { std::bind(&THangulProcessor::RemoveGerund, std::placeholders::_1, std::placeholders::_2), L"В виде субстантива, отглагольного существительного \"делание\". Допишите следующее слово, возможно вы получите более точный перевод\n" };
	VerbTestMap[TVerbParams::VS_GERUND_TOO] = { std::bind(&THangulProcessor::RemoveGerundToo, std::placeholders::_1, std::placeholders::_2), L"В виде субстантива, отглагольного существительного \"делание\" с модификатором \"тоже\", \"также\". Допишите следующее слово, возможно вы получите более точный перевод\n", CONST_LESSON_VERB_TOO };
	VerbTestMap[TVerbParams::VS_GERUND_ONLY] = { std::bind(&THangulProcessor::RemoveGerundOnly, std::placeholders::_1, std::placeholders::_2), L"В виде субстантива, отглагольного существительного \"делание\" с модификатором \"только\", \"лишь\". Допишите следующее слово, возможно вы получите более точный перевод\n", CONST_LESSON_VERB_ONLY };


	VerbTestMap[TVerbParams::VS_DO_IT_FOR_ME_SHORT_PANMAL] = { std::bind(&THangulProcessor::RemoveDoItForMeShortPanmal, std::placeholders::_1, std::placeholders::_2), L"Соединенный с глаголом \"сделай это для меня\" в просторечной форме\n" };
	VerbTestMap[TVerbParams::VS_DO_IT_FOR_ME_SHORT_POLITE] = { std::bind(&THangulProcessor::RemoveDoItForMeShortPolite, std::placeholders::_1, std::placeholders::_2), L"Соединенный с глаголом \"сделай это для меня\" в вежливой форме\n" };
	VerbTestMap[TVerbParams::VS_DO_IT_FOR_ME_SHORT_IMPERATIVE] = { std::bind(&THangulProcessor::RemoveDoItForMeShortImperative, std::placeholders::_1, std::placeholders::_2), L"Соединенный с глаголом \"сделай это для меня\" в почтительной форме\n" };

	VerbTestMap[TVerbParams::VS_MOD_YA] = { std::bind(&THangulProcessor::RemoveModificatorYa, std::placeholders::_1, std::placeholders::_2), L"С модификатором \"Только когда\". Допишите следующее слово, возможно вы получите более точный перевод\n" };

}



bool THangulProcessor::RecognizeVerb(std::vector<TSyllable> verb, std::vector<TVerb>& verbResultArr)
{
	bool r = false;

	for (auto& testCase : VerbTestMap)
	{
		std::vector<TSyllable> modVerb = verb;
		TVerb testVerb;

		wchar_t nextChamo = CONST_EMPTY_LAST_CHAMO;

		bool formExtracted = testCase.second.testFunction(modVerb, nextChamo);

		if (formExtracted)
		{
			if (modVerb.size() > 0)
			{
				std::vector<TSyllable> localModVerb = modVerb;

				testVerb.Params.VerbModificators = RemoveBasicVerbModificators(localModVerb, nextChamo);

				if (RecognizeVerbInFirstForm(localModVerb, testVerb))
				{
					testVerb.Params.VerbStatus = testCase.first;
					verbResultArr.push_back(testVerb);

					r = true;
				}

				localModVerb = modVerb;

				//Test ㄹ irregular
				if (ChamoAffectLIrregular(nextChamo) && RecognizeVerbInFirstFormLIrregular(localModVerb, testVerb))
				{
					testVerb.Params.VerbStatus = testCase.first;
					verbResultArr.push_back(testVerb);

					r = true;
				}

				localModVerb = modVerb;

				// Test ㅡ (for example 크다 쓰다 and so on)
				if (RecognizeVerbInFirstFormU(localModVerb, testVerb))
				{
					testVerb.Params.VerbStatus = testCase.first;
					verbResultArr.push_back(testVerb);

					r = true;
				}
			}
		}
	}

	return r;

}


bool THangulProcessor::RecognizePlainNoun(std::vector<TSyllable> plainNoun, std::vector<TWordRecord>::const_iterator& result)
{
	return HangulDictionary.FindNoun(plainNoun, result);
}

bool THangulProcessor::RecognizeNoun(std::vector<TSyllable> nounWord, std::vector<TNoun>& nounResultArr)
{
	bool r = false;

	for (auto& testCase : NounTestMap)
	{
		std::vector<TSyllable> modNoun = nounWord;

		TNoun testNoun;

		testNoun.Params.NounModificators.Too = TryExtractNounModificator_Too(modNoun);
		
		bool formExtracted = testCase.second.testFunction(modNoun);

		if (formExtracted)
		{

			testNoun.Params.NounCase = testCase.first;

			testNoun.Params.NounModificators.Plural = TryExtractNounModificator_Plural(modNoun);
			testNoun.Params.NounModificators.Only = TryExtractNounModificator_Only(modNoun);
			
			//testNoun.Params.NounModificators.Accusative = TryExtractNounModificator_Accusative(modNoun);
			
			if (RemoveDativePlaceCaseEndingIfPossible(modNoun))
			{
				testNoun.Params.NounModificators.NounDativeModificators = TNounModificators::NDM_DATIVE_PLACE;
			}
			if (RemoveDativePersonCaseEndingIfPossible(modNoun))
			{
				testNoun.Params.NounModificators.NounDativeModificators = TNounModificators::NDM_DATIVE_PERSON;
			}
			if (RemoveDativePersonVerbalCaseEndingIfPossible(modNoun))
			{
				testNoun.Params.NounModificators.NounDativeModificators = TNounModificators::NDM_DATIVE_PERSON_VERBAL;
			}
			if (RemoveDativePlaceFromCaseEndingIfPossible(modNoun))
			{
				testNoun.Params.NounModificators.NounDativeModificators = TNounModificators::NDM_DATIVE_PLACE_FROM;
			}
			if (RemoveDativePersonFromCaseEndingIfPossible(modNoun))
			{
				testNoun.Params.NounModificators.NounDativeModificators = TNounModificators::NDM_DATIVE_PERSON_FROM;
			}
			if (RemoveDativePersonFromVerbalCaseEndingIfPossible(modNoun))
			{
				testNoun.Params.NounModificators.NounDativeModificators = TNounModificators::NDM_DATIVE_PERSON_FROM_VERBAL;
			}
			if (RemoveStartFromCaseEndingIfPossible(modNoun))
			{
				testNoun.Params.NounModificators.NounDativeModificators = TNounModificators::NDM_DATIVE_PLACE_START_FROM;
			}
			if (RemoveEndToCaseEndingIfPossible(modNoun))
			{
				testNoun.Params.NounModificators.NounDativeModificators = TNounModificators::NDM_DATIVE_PLACE_END_TO;
			}
		
			if (RecognizePlainNoun(modNoun, testNoun.Itr))
			{

				nounResultArr.push_back(testNoun);

				r = true;
			}
		}
	}

	return r;

}


bool THangulProcessor::RecognizeAndVerboseWords(const std::vector<std::wstring>& inWordArr, std::string& outVerbose, std::set<std::wstring>& outLessons)
{
	if (inWordArr.size() == 0)
	{
		return false;
	}

	std::vector<std::wstring> meaningsArr;

	bool r2 = false;
	bool r = false;

	std::vector<TComplexVerb> verbArr;

	r = RecognizeComplexVerb(inWordArr, verbArr);

	if (r)
	{
		std::vector<std::wstring> localMeaningsArr = UniversalVerboseWord<TComplexVerb>(verbArr);

		meaningsArr.insert(meaningsArr.end(), localMeaningsArr.begin(), localMeaningsArr.end());

		std::set<std::wstring> localLessons = UniversalGetLessons<TComplexVerb>(verbArr);

		outLessons.insert(localLessons.begin(), localLessons.end());


		r2 = true;
	}

	std::vector<TFutureTensePhrase> futureTensePhraseArr;

	r = RecognizeComplexVerbAndFutureTense(inWordArr, futureTensePhraseArr);

	if (r)
	{
		std::vector<std::wstring> localMeaningsArr = UniversalVerboseWord<TFutureTensePhrase>(futureTensePhraseArr);

		meaningsArr.insert(meaningsArr.end(), localMeaningsArr.begin(), localMeaningsArr.end());

		std::set<std::wstring> localLessons = UniversalGetLessons<TFutureTensePhrase>(futureTensePhraseArr);

		outLessons.insert(localLessons.begin(), localLessons.end());


		r2 = true;
	}


	if (inWordArr.size() == 1)
	{
		std::wstring word = inWordArr[0];

		TChineseNumber n;

		r = RecognizeChineseNumber(word, n);

		if (r)
		{
			std::vector<std::wstring> localMeaningsArr = UniversalVerboseWord<TChineseNumber>({ n });

			meaningsArr.insert(meaningsArr.end(), localMeaningsArr.begin(), localMeaningsArr.end());

			std::set<std::wstring> localLessons = UniversalGetLessons<TChineseNumber>({ n });

			outLessons.insert(localLessons.begin(), localLessons.end());

			r2 = true;
		}



		TSimpleWord simpleWord;

		r = HangulDictionary.FindSpecial(WstringToSyllableArr(word), simpleWord.Itr);

		if (r)
		{
			std::vector<std::wstring> localMeaningsArr = UniversalVerboseWord<TSimpleWord>({ simpleWord });

			meaningsArr.insert(meaningsArr.end(), localMeaningsArr.begin(), localMeaningsArr.end());

			std::set<std::wstring> localLessons = UniversalGetLessons<TSimpleWord>({ simpleWord });

			outLessons.insert(localLessons.begin(), localLessons.end());

			r2 = true;
		}

		r = HangulDictionary.FindAdverb(WstringToSyllableArr(word), simpleWord.Itr);

		if (r)
		{
			std::vector<std::wstring> localMeaningsArr = UniversalVerboseWord<TSimpleWord>({ simpleWord });

			meaningsArr.insert(meaningsArr.end(), localMeaningsArr.begin(), localMeaningsArr.end());

			std::set<std::wstring> localLessons = UniversalGetLessons<TSimpleWord>({ simpleWord });

			outLessons.insert(localLessons.begin(), localLessons.end());

			r2 = true;
		}



		std::vector<TNoun> nounArr;

		r = RecognizeNoun(WstringToSyllableArr(word), nounArr);

		if (r)
		{

			std::vector<std::wstring> localMeaningsArr = UniversalVerboseWord<TNoun>(nounArr);

			meaningsArr.insert(meaningsArr.end(), localMeaningsArr.begin(), localMeaningsArr.end());

			std::set<std::wstring> localLessons = UniversalGetLessons<TNoun>(nounArr);

			outLessons.insert(localLessons.begin(), localLessons.end());

			r2 = true;
		}

	}


	outVerbose = MergeAllVerbose(meaningsArr);

	return r2;

}

bool THangulProcessor::RecognizeComplexVerb(const std::vector<std::wstring>& inWordArr, std::vector<TComplexVerb>& resultArr)
{
	if (inWordArr.size() == 1)
	{
		std::vector<TVerb> verbResultArr;


		bool success = RecognizeVerb(WstringToSyllableArr(inWordArr[0]), verbResultArr);

		if (success)
		{
			for (auto& verb : verbResultArr)
			{
				TComplexVerb complexVerb;
				complexVerb.ComplexVerbType = TComplexVerb::CVT_SINGLE_VERB;
				complexVerb.LastVerb = verb;

				resultArr.push_back(complexVerb);
			}

			return true;
		}
	}
	else if (inWordArr.size() > 1)
	{
		std::vector<std::wstring> newInWordArr(inWordArr.begin(), inWordArr.end() - 1);

		std::vector<TComplexVerb> newResultArr;

		std::vector<TVerb> verbResultArr;

		bool leftSideIsComplexVerb = RecognizeComplexVerb(newInWordArr, newResultArr);

		bool rightSideIsSimpleVerb = RecognizeVerb(WstringToSyllableArr(inWordArr[inWordArr.size() - 1]), verbResultArr);

		if (rightSideIsSimpleVerb)
		{

			if (leftSideIsComplexVerb)
			{
				bool atLeastOneVerbIsAcceptable = false;

				for (auto& innerComplexVerb : newResultArr)
				{
					for (auto& verb : verbResultArr)
					{

						for (auto& complexVerbTextMapElement : ComplexVerbTestMap)
						{
							std::shared_ptr<TComplexVerb> complexVerbPtr(new TComplexVerb(innerComplexVerb));

							if (complexVerbTextMapElement.second.testFunction(complexVerbPtr, verb, {}))
							{
								atLeastOneVerbIsAcceptable = true;

								TComplexVerb complexVerb;
								complexVerb.ComplexVerbType = TComplexVerb::CVT_CHAINED_VERBS;
								complexVerb.ComplexVerbCase = complexVerbTextMapElement.first;
								complexVerb.InnerComplexVerb = complexVerbPtr;
								complexVerb.LastVerb = verb;

								resultArr.push_back(complexVerb);
							}
						}
					}
				}

				return atLeastOneVerbIsAcceptable;
			}
			else
			{
				std::vector<std::wstring> middleWordArr;

				bool atLeastOneVerbIsAcceptable = false;

				while (newInWordArr.size() > 0)
				{
					middleWordArr.push_back(newInWordArr.back());

					newInWordArr.erase(newInWordArr.end() - 1);

					bool leftSideIsComplexVerb = RecognizeComplexVerb(newInWordArr, newResultArr);

					for (auto& innerComplexVerb : newResultArr)
					{
						for (auto& verb : verbResultArr)
						{

							for (auto& complexVerbTextMapElement : ComplexVerbTestMap)
							{
								std::shared_ptr<TComplexVerb> complexVerbPtr(new TComplexVerb(innerComplexVerb));

								if (complexVerbTextMapElement.second.testFunction(complexVerbPtr, verb, middleWordArr))
								{
									atLeastOneVerbIsAcceptable = true;

									TComplexVerb complexVerb;
									complexVerb.ComplexVerbType = TComplexVerb::CVT_CHAINED_VERBS_WITH_WORDS_IN_MIDDLE;
									complexVerb.ComplexVerbCase = complexVerbTextMapElement.first;
									complexVerb.InnerComplexVerb = complexVerbPtr;
									complexVerb.LastVerb = verb;

									complexVerb.MiddleWords = middleWordArr;

									resultArr.push_back(complexVerb);
								}
							}
						}
					}
				}

				return atLeastOneVerbIsAcceptable;
			}

		}


	}

	return false;
}


bool THangulProcessor::RecognizeComplexVerbAndFutureTense(const std::vector<std::wstring>& inWordArr, std::vector<TFutureTensePhrase>& resultArr)
{
	if (inWordArr.size() > 1)
	{
		std::vector<std::wstring> innerWordArr(inWordArr.begin(), inWordArr.end() - 1);

		std::vector<TComplexVerb> newResultArr;

		if (RecognizeComplexVerb(innerWordArr, newResultArr))
		{
			TFutureTensePhrase::TFutureTenseType futureTenseType = TFutureTensePhrase::RecognizeFutureTenseType(inWordArr[inWordArr.size() - 1]);

			if (futureTenseType != TFutureTensePhrase::FTT_NONE)
			{


				for (auto& complexVerb : newResultArr)
				{
					if (complexVerb.LastVerb.Params.VerbModificators.TenseModificator == TVerbModificators::TM_NONE &&
						complexVerb.LastVerb.Params.VerbStatus == TVerbParams::VS_FUTURE_PARTICIPLE)
					{
						TFutureTensePhrase futureTensePhrase;

						futureTensePhrase.ComplexVerb = complexVerb;

						futureTensePhrase.FutureTenseType = futureTenseType;

						resultArr.push_back(futureTensePhrase);
					}
				}

				return true;
			}

		}
	}

	return false;
}


std::string THangulProcessor::MergeAllVerbose(const std::vector<std::wstring>& verboseArr)
{
	std::wstring wOutVerbose;

	for (size_t i = 0; i < verboseArr.size(); i++)
	{
		wOutVerbose += boost::lexical_cast<std::wstring>(i + 1) + L") " + verboseArr[i] + L"\n";
	}

	return SE::wstring_to_string(wOutVerbose);
}
