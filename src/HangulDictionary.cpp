#include "HangulDictionary.h"
#include "NumbersProcessor.h"

namespace LH
{

	const wchar_t CONST_EMPTY_LAST_CHAMO = 0x11A7;

	bool SymbolIsSyllable(wchar_t symbol)
	{
		return  (symbol >= 0xAC00 && symbol <= 0xD7A3);
	}

	bool SymbolIsLastChamo(wchar_t symbol)
	{
		return  (symbol >= CONST_EMPTY_LAST_CHAMO && symbol <= 0x11C2);
	}

	bool ChamoIsVowel(wchar_t chamo)
	{
		return chamo >= 0x1161 && chamo <= 0x1175;
	}

	Syllable::Syllable(wchar_t syllable)
	{
		AcceptSyllable(syllable);
	}


	void Syllable::AcceptSyllable(wchar_t syllable)
	{
		if (!SymbolIsSyllable(syllable))
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

		if (!ChamoIsVowel(middleChamo))
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


	bool Syllable::hasPatchim() const
	{
		return lastChamo != CONST_EMPTY_LAST_CHAMO;
	}

	bool Syllable::hasPatchimNotL() const
	{
		return lastChamo != CONST_EMPTY_LAST_CHAMO && lastChamo != L'ᆯ';
	}

	wchar_t Syllable::ToUnicodeSyllable() const
	{
		return ((firstChamo - 0x1100) * 0x24C)
			+ ((middleChamo - 0x1161) * 0x1C)
			+ (lastChamo - CONST_EMPTY_LAST_CHAMO)
			+ 0xAC00;
	}

	std::string Syllable::str() const
	{
		std::wstring w;
		w = w + ToUnicodeSyllable();
		return SE::wstring_to_string(w);
	}

	std::string Syllable::firstChamoStr() const
	{
		std::wstring w;
		w = w + firstChamo;
		return SE::wstring_to_string(w);
	}

	std::string Syllable::middleChamoStr() const
	{
		std::wstring w;
		w = w + middleChamo;
		return SE::wstring_to_string(w);
	}

	std::string Syllable::lastChamoStr() const
	{
		std::wstring w;
		w = w + lastChamo;
		return SE::wstring_to_string(w);
	}

	void Syllable::setFirstChamoStr(std::string c)
	{
		std::wstring w = SE::string_to_wstring(c);

		firstChamo = w[0];
	}

	void Syllable::setMiddleChamoStr(std::string c)
	{
		std::wstring w = SE::string_to_wstring(c);

		middleChamo = w[0];
	}

	void Syllable::setLastChamoStr(std::string c)
	{
		std::wstring w = SE::string_to_wstring(c);

		lastChamo = w[0];
	}

	void Syllable::removePatchim()
	{
		lastChamo = CONST_EMPTY_LAST_CHAMO;
	}

	bool Syllable::patchimIsSS()
	{
		return lastChamo == L'ᆻ';
	}

	bool Syllable::isLight() const
	{
		
		bool result = middleChamo == L'ᅡ' ||
			middleChamo == L'ᅢ' ||
			middleChamo == L'ᅣ' ||
			middleChamo == L'ᅤ' ||
			middleChamo == L'ᅩ' ||
			middleChamo == L'ᅪ' ||
			middleChamo == L'ᅫ' ||
			middleChamo == L'ᅬ' ||
			middleChamo == L'ᅭ';

		return result;
	}

	void Syllable::PrintOut() const
	{
		std::cout << str();
	}

	Syllable CreateSyllable(std::string s)
	{
		std::wstring w = SE::string_to_wstring(s);
		return Syllable(w[0]);

	}

	SyllableWord EmptySyllableWord()
	{
		return SyllableWord();
	}

	SyllableWord::SyllableWord()
	{
		
	}

	SyllableWord::SyllableWord(const std::wstring& wstr)
	{
		syllableArr = WstringToSyllableArr(wstr);
	}

	SyllableWord::SyllableWord(const std::vector<Syllable>& newSyllableArr)
	{
		syllableArr = newSyllableArr;
	}

	Syllable SyllableWord::GetSyllable(int position)
	{
		return syllableArr[position];
	}

	int SyllableWord::size()
	{
		return syllableArr.size();
	}

	Syllable SyllableWord::last()
	{
		return syllableArr[syllableArr.size()-1];
	}

	Syllable SyllableWord::first()
	{
		return syllableArr[0];
	}

	std::wstring SyllableWord::ToWString() const
	{
		return SyllableArrToWstring(syllableArr);
	}

	SyllableWord SyllableWord::removeFirst()
	{
		std::vector<Syllable> newSyllableArr(syllableArr.begin() + 1, syllableArr.end());

		return SyllableWord(newSyllableArr);
	}

	SyllableWord SyllableWord::pop()
	{
		auto newSyllableArr = syllableArr;

		newSyllableArr.pop_back();

		return SyllableWord(newSyllableArr);
		
	}

	SyllableWord SyllableWord::push(Syllable s)
	{
		auto newSyllableArr = syllableArr;

		newSyllableArr.push_back(s);

		return SyllableWord(newSyllableArr);


	}

	SyllableWord SyllableWord::clone() const
	{
		auto newSyllableArr = syllableArr;

		return SyllableWord(newSyllableArr);
	}

	void SyllableWord::PrintOut() const
	{
		std::cout << str() << std::endl;
	}

	std::string SyllableWord::str() const
	{
		std::wstring w = ToWString();
		return SE::wstring_to_string(w);
	}



	SyllableWord SyllableWord::cutEnd(std::string s) const
	{
		std::wstring w = SE::string_to_wstring(s);

		
		if ((w.size() > 0) && (w.size() <= syllableArr.size()))
		{
			auto newSyllableArr = syllableArr;

			int size = w.size();

			for (int i = 0; i < size; i++)
			{
				if (w.back() == newSyllableArr.back().ToUnicodeSyllable())
				{
					w.pop_back();
					newSyllableArr.pop_back();
				}
				else if ((i == size - 1) && SymbolIsLastChamo(w.back()) && (w.back() == newSyllableArr.back().lastChamo))
				{
					w.pop_back();
					newSyllableArr.back().removePatchim();
				}
				else
				{
					newSyllableArr.clear();
					break;
				}
			}

			return SyllableWord(newSyllableArr);

		}
		

		return SyllableWord();

	}

	SyllableWord SyllableWord::replaceEnd(std::string str, std::string newStr) const
	{
		std::wstring wstr = SE::string_to_wstring(str);
		std::wstring wNewStr = SE::string_to_wstring(newStr);

		if ((wstr.size() > 0) && (wstr.size() <= syllableArr.size()))
		{
			auto newSyllableArr = syllableArr;

			int size = wstr.size();

			for (int i = 0; i < size; i++)
			{
				if (wstr.back() == newSyllableArr.back().ToUnicodeSyllable())
				{
					wstr.pop_back();
					newSyllableArr.pop_back();
				}
				else
				{
					return SyllableWord();
				}


			}

			for (int i = 0; i < wNewStr.size(); i++)
			{
				newSyllableArr.push_back(Syllable(wNewStr[i]));
			}
			

			return SyllableWord(newSyllableArr);

		}


		return SyllableWord();
	}

	SyllableWord SyllableWord::cutEndVarious(std::string sOnPatchimExist, std::string sOnNoStrongPatchimExist, bool ignoreTypeLPatchim) const
	{

		std::wstring wOnPatchimExist = SE::string_to_wstring(sOnPatchimExist);

		std::wstring wOnNoStrongPatchimExist = SE::string_to_wstring(sOnNoStrongPatchimExist);
		
		auto newSyllableArr = syllableArr;

		if ((wOnPatchimExist.size() > 0) && (wOnPatchimExist.size() < newSyllableArr.size()))
		{

			int size = wOnPatchimExist.size();

			for (int i = 0; i < size; i++)
			{
				if (wOnPatchimExist.back() == newSyllableArr.back().ToUnicodeSyllable())
				{
					wOnPatchimExist.pop_back();
					newSyllableArr.pop_back();
				}
				else
				{
					newSyllableArr.clear();
					break;
				}
			}

			if (newSyllableArr.size() > 0)
			{

				if (ignoreTypeLPatchim)
				{
					if (!newSyllableArr.back().hasPatchimNotL())
					{
						newSyllableArr.clear();
					}
				}
				else
				{
					if (!newSyllableArr.back().hasPatchim())
					{
						newSyllableArr.clear();
					}
				}

				
				return SyllableWord(newSyllableArr);
			}
		}


		newSyllableArr = syllableArr;

		if ((wOnNoStrongPatchimExist.size() > 0) && (wOnNoStrongPatchimExist.size() <= newSyllableArr.size()))
		{

			int size = wOnNoStrongPatchimExist.size();

			for (int i = 0; i < size; i++)
			{
				if (wOnNoStrongPatchimExist.back() == newSyllableArr.back().ToUnicodeSyllable())
				{
					wOnNoStrongPatchimExist.pop_back();
					newSyllableArr.pop_back();
				}
				else if ((i == size - 1) && SymbolIsLastChamo(wOnNoStrongPatchimExist.back()) && (wOnNoStrongPatchimExist.back() == newSyllableArr.back().lastChamo))
				{
					wOnNoStrongPatchimExist.pop_back();
					newSyllableArr.back().removePatchim();
				}
				else
				{
					newSyllableArr.clear();
					break;
				}
			}

			if ((newSyllableArr.size() > 0) && (newSyllableArr.back().hasPatchim()))
			{
				newSyllableArr.clear();
			}

			return SyllableWord(newSyllableArr);
		}

		return SyllableWord();

	}

	SyllableWord SyllableWord::cutVerbEndVarious(std::string sOnPatchimExist, std::string sOnNoStrongPatchimExist) const
	{
		return cutEndVarious(sOnPatchimExist, sOnNoStrongPatchimExist, true);
	}

	SyllableWord SyllableWord::cutNounEndVarious(std::string sOnPatchimExist, std::string sOnNoPatchimExist) const
	{
		return cutEndVarious(sOnPatchimExist, sOnNoPatchimExist, false);
	}


	SyllableWord SyllableWord::cutBegin(std::string s) const
	{
		std::wstring w = SE::string_to_wstring(s);


		if ((w.size() > 0) && (w.size() <= syllableArr.size()))
		{
			auto newSyllableArr = syllableArr;

			int size = w.size();

			for (int i = 0; i < size; i++)
			{
				if (w[0] == newSyllableArr[0].ToUnicodeSyllable())
				{
					w.erase(w.begin());
					newSyllableArr.erase(newSyllableArr.begin());
				}
				else
				{
					newSyllableArr.clear();
					break;
				}
			}

			return SyllableWord(newSyllableArr);

		}

		return SyllableWord();
	}

	SyllableWord SyllableWord::glueIrregularL() const
	{
		
		if ((syllableArr.size() > 0) &&
			(!syllableArr.back().hasPatchim()))
		{
			auto newSyllableArr = syllableArr;
			newSyllableArr.back().lastChamo = L'ᆯ';
			return SyllableWord(newSyllableArr);
		}

		return SyllableWord();
	}

	bool SyllableWord::isValid() const
	{
		return syllableArr.size() > 0;
	}

	std::vector<Syllable> WstringToSyllableArr(const std::wstring& wstr)
	{
		std::vector<Syllable> result;

		for (auto& wchr : wstr)
		{
			result.push_back(Syllable(wchr));
		}
		return result;
	}

	std::wstring SyllableArrToWstring(const std::vector<Syllable>& syllableArr)
	{
		std::wstring result;

		for (auto& syl : syllableArr)
		{
			result += syl.ToUnicodeSyllable();
		}
		return result;
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

		NounTable.push_back(CreateWord(L"딸", L"Дочь"));


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

	int THangulDictionary::FindVerb(const SyllableWord& verb)
	{
		for (int i = 0; i < VerbTable.size(); i++)
		{
			if (VerbTable[i].base == verb.ToWString())
			{
				return i;
			}
		}

		return -1;
	}


	int THangulDictionary::FindNoun(const SyllableWord& noun)
	{
		for (int i = 0; i < NounTable.size(); i++)
		{
			if (NounTable[i].base == noun.ToWString())
			{
				return i;
			}
		}

		return -1;

	}

	int THangulDictionary::FindAdverb(const SyllableWord& adverb)
	{

		for (int i = 0; i < AdverbTable.size(); i++)
		{
			if (AdverbTable[i].base == adverb.ToWString())
			{
				return i;
			}
		}

		return -1;

	}

	int THangulDictionary::FindSpecial(const SyllableWord& special)
	{

		for (int i = 0; i < SpecialTable.size(); i++)
		{
			if (SpecialTable[i].base == special.ToWString())
			{
				return i;
			}
		}

		return -1;

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
			std::ifstream f("../scripts/" + fileName);

#else
			std::ifstream f("/home/ubuntu/work/hallyu_server/scripts/" + fileName); // #path switch "/home/devuser/workplace/hallyu_server/scripts/"
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
				std::cout << "file not found! " << "HD789" << std::endl;
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
			std::ifstream f("../scripts/" + fileName);

#else
			std::ifstream f("/home/ubuntu/work/hallyu_server/scripts/" + fileName); // #path switch# "/home/devuser/workplace/hallyu_server/scripts/"
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
				std::cout << "file not found! " << "HD838" << std::endl;
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
			std::ifstream f("../scripts/" + fileName);

#else
			std::ifstream f("/home/ubuntu/work/hallyu_server/scripts/" + fileName); // #path switch# "/home/devuser/workplace/hallyu_server/scripts/"
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
				std::cout << "file not found! " << "HD886" << std::endl;
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

	std::vector<TWordRecord> THangulDictionary::SearchWordByRussian(std::wstring wRussianWord)
	{

		boost::to_lower(wRussianWord);
		//std::transform(wRussianWord.begin(), wRussianWord.end(), wRussianWord.begin(), std::tolower<wchar_t>);

		std::vector<TWordRecord> result;

		for (auto& wordRecord : AllWords)
		{
			for (auto& meaningAndComment : wordRecord.words)
			{
				if (wRussianWord.size() <= meaningAndComment.meaning.size())
				{
					std::wstring substr = meaningAndComment.meaning.substr(0, wRussianWord.size());

					//std::transform(substr.begin(), substr.end(), substr.begin(), std::tolower<wchar_t>);
					boost::to_lower(substr);

					if (wRussianWord == substr)
					{
						result.push_back(wordRecord);
						break;
					}
				}
			}
		}

		return result;

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


}
