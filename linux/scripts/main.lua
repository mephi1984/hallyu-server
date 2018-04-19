function TryExtractNounModificator_CaseSubject(noun)

	return noun:cutNounEndVarious("은", "는")
end

function TryExtractNounModificator_CaseTopic(noun)

	return noun:cutNounEndVarious("이", "가")
end

function TryExtractNounModificator_CaseGenitive(noun)

	return noun:cutEnd("의")
end

function TryExtractNounModificator_CaseAccusative(noun)
	
	return noun:cutNounEndVarious("을", "를")
end

function TryExtractNounModificator_CaseLike(noun)
	
	return noun:cutEnd("처럼")
end

function TryExtractNounModificator_CaseAnd(noun)

	return noun:cutNounEndVarious("과", "와")
end

function TryExtractNounModificator_CaseAndVerbal(noun)

	return noun:cutNounEndVarious("이하고", "하고")
end

function TryExtractNounModificator_CaseWith(noun)
	return noun:cutEnd("랑")

end

function TryExtractNounModificator_Too(noun)
	return noun:cutEnd("도")
end

function TryExtractNounModificator_Plural(noun)
	return noun:cutEnd("들")

end

function TryExtractNounModificator_Only(noun)
	return noun:cutEnd("만")
end


function TryExtractNounModificator_DativePlace(noun)
	return noun:cutEnd("에")
end

function TryExtractNounModificator_DativePerson(noun)

	return noun:cutEnd("에게")
end

function TryExtractNounModificator_DativePersonVerbal(noun)
	
	return noun:cutEnd("한테")
end

function TryExtractNounModificator_DativePlaceFrom(noun)

	return noun:cutEnd("에서")
end

function TryExtractNounModificator_DativePersonFrom(noun)

	return noun:cutEnd("에게서")
end

function TryExtractNounModificator_DativePersonFromVerbal(noun)
	return noun:cutEnd("한테서")
end

function TryExtractNounModificator_DativePlaceStartFrom(noun)
	return noun:cutEnd("부터")
end

function TryExtractNounModificator_DativePlaceEndTo(noun)

	return noun:cutEnd("까지")
end


NounMap = {}

NounMap[1] = {}
NounMap[1]["NA_TOO"] = {}
NounMap[1]["NA_TOO"].testFunc = TryExtractNounModificator_Too
NounMap[1]["NA_TOO"].verbose = "С модификатором 'тоже', 'также'"
NounMap[1]["NA_TOO"].lesson = "lessons/korean032.html"

NounMap[2] = {}
NounMap[2]["NC_SUBJECT"] = {}
NounMap[2]["NC_SUBJECT"].testFunc = TryExtractNounModificator_CaseSubject
NounMap[2]["NC_SUBJECT"].verbose = "В именительном падеже в роли субъекта"
NounMap[2]["NC_SUBJECT"].lesson = "lessons/korean009.html"

NounMap[2]["NC_TOPIC"] = {}
NounMap[2]["NC_TOPIC"].testFunc = TryExtractNounModificator_CaseTopic
NounMap[2]["NC_TOPIC"].verbose = "В именительном падеже в роли темы"
NounMap[2]["NC_TOPIC"].lesson = "lessons/korean009.html"

NounMap[2]["NC_GENITIVE"] = {}
NounMap[2]["NC_GENITIVE"].testFunc = TryExtractNounModificator_CaseGenitive
NounMap[2]["NC_GENITIVE"].verbose = "В родительном падеже (кого? чего? Чей?)"
NounMap[2]["NC_GENITIVE"].lesson = "lessons/korean028.html"

NounMap[2]["NC_ACCUSATIVE"] = {}
NounMap[2]["NC_ACCUSATIVE"].testFunc = TryExtractNounModificator_CaseAccusative
NounMap[2]["NC_ACCUSATIVE"].verbose = "В винительном падеже (кого? что?)"
NounMap[2]["NC_ACCUSATIVE"].lesson = "lessons/korean013.html"

NounMap[2]["NC_LIKE"] = {}
NounMap[2]["NC_LIKE"].testFunc = TryExtractNounModificator_CaseLike
NounMap[2]["NC_LIKE"].verbose = "С обозначением похожести, подобия (как кто? Как что?)"
NounMap[2]["NC_LIKE"].lesson = ""

NounMap[2]["NC_AND"] = {}
NounMap[2]["NC_AND"].testFunc = TryExtractNounModificator_CaseAnd
NounMap[2]["NC_AND"].verbose = "С союзом объединения И, в письменной форме"
NounMap[2]["NC_AND"].lesson = "lessons/korean031.html"

NounMap[2]["NC_AND_VERBAL"] = {}
NounMap[2]["NC_AND_VERBAL"].testFunc = TryExtractNounModificator_CaseAndVerbal
NounMap[2]["NC_AND_VERBAL"].verbose = "С союзом объединения И, в разговорной форме"
NounMap[2]["NC_AND_VERBAL"].lesson = "lessons/korean031.html"

NounMap[2]["NC_WITH"] = {}
NounMap[2]["NC_WITH"].testFunc = TryExtractNounModificator_CaseWith
NounMap[2]["NC_WITH"].verbose = "С союзом объединения (с кем? с чем?)"
NounMap[2]["NC_WITH"].lesson = "lessons/korean031.html"


NounMap[3] = {}
NounMap[3]["NP_PLURAL"] = {}
NounMap[3]["NP_PLURAL"].testFunc = TryExtractNounModificator_Plural
NounMap[3]["NP_PLURAL"].verbose = "В множественном числе"
NounMap[3]["NP_PLURAL"].lesson = ""

NounMap[4] = {}
NounMap[4]["NM_ONLY"] = {}
NounMap[4]["NM_ONLY"].testFunc = TryExtractNounModificator_Only
NounMap[4]["NM_ONLY"].verbose = "С модификатором 'только', 'лишь'"
NounMap[4]["NM_ONLY"].lesson = "lessons/korean033.html"

NounMap[5] = {}
NounMap[5]["ND_DATIVE_PLACE"] = {}
NounMap[5]["ND_DATIVE_PLACE"].testFunc = TryExtractNounModificator_DativePlace
NounMap[5]["ND_DATIVE_PLACE"].verbose = "В местном падеже (куда? где?)"
NounMap[5]["ND_DATIVE_PLACE"].lesson = "lessons/korean017.html"

NounMap[5]["ND_DATIVE_PERSON"] = {}
NounMap[5]["ND_DATIVE_PERSON"].testFunc = TryExtractNounModificator_DativePerson
NounMap[5]["ND_DATIVE_PERSON"].verbose = "В местном падеже лица (кому? К кому? Для кого?) в письменной форме"
NounMap[5]["ND_DATIVE_PERSON"].lesson = "lessons/korean030.html"

NounMap[5]["ND_DATIVE_PERSON_VERBAL"] = {}
NounMap[5]["ND_DATIVE_PERSON_VERBAL"].testFunc = TryExtractNounModificator_DativePersonVerbal
NounMap[5]["ND_DATIVE_PERSON_VERBAL"].verbose = "В местном падеже лица (кому? К кому? Для кого?) в разговорной форме"
NounMap[5]["ND_DATIVE_PERSON_VERBAL"].lesson = "lessons/korean030.html"

NounMap[5]["ND_DATIVE_PLACE_FROM"] = {}
NounMap[5]["ND_DATIVE_PLACE_FROM"].testFunc = TryExtractNounModificator_DativePlaceFrom
NounMap[5]["ND_DATIVE_PLACE_FROM"].verbose = "В местном исходящем падеже (откуда? где?)"
NounMap[5]["ND_DATIVE_PLACE_FROM"].lesson = "lessons/korean017.html"

NounMap[5]["ND_DATIVE_PERSON_FROM"] = {}
NounMap[5]["ND_DATIVE_PERSON_FROM"].testFunc = TryExtractNounModificator_DativePersonFrom
NounMap[5]["ND_DATIVE_PERSON_FROM"].verbose = "В местном исходящем падеже лица (от кого?) в письменной форме"
NounMap[5]["ND_DATIVE_PERSON_FROM"].lesson = "lessons/korean030.html"

NounMap[5]["ND_DATIVE_PERSON_FROM_VERBAL"] = {}
NounMap[5]["ND_DATIVE_PERSON_FROM_VERBAL"].testFunc = TryExtractNounModificator_DativePersonFromVerbal
NounMap[5]["ND_DATIVE_PERSON_FROM_VERBAL"].verbose = "В местном исходящем падеже лица (от кого?) в разговорной форме"
NounMap[5]["ND_DATIVE_PERSON_FROM_VERBAL"].lesson = "lessons/korean030.html"

NounMap[5]["ND_DATIVE_PLACE_START_FROM"] = {}
NounMap[5]["ND_DATIVE_PLACE_START_FROM"].testFunc = TryExtractNounModificator_DativePlaceStartFrom
NounMap[5]["ND_DATIVE_PLACE_START_FROM"].verbose = "В местном падеже с обозначением места начала (откуда? От чего?)"
NounMap[5]["ND_DATIVE_PLACE_START_FROM"].lesson = "lessons/korean024.html"

NounMap[5]["ND_DATIVE_PLACE_END_TO"] = {}
NounMap[5]["ND_DATIVE_PLACE_END_TO"].testFunc = TryExtractNounModificator_DativePlaceEndTo
NounMap[5]["ND_DATIVE_PLACE_END_TO"].verbose = "В местном падеже с обозначением места конца (докуда? До чего?)"
NounMap[5]["ND_DATIVE_PLACE_END_TO"].lesson = "lessons/korean024.html"

function UnGlueEndingForVerbInSecondFormIfPossible(verb)


--Xperimental
--[[
	if ((verb:size() == 1) and (verb:last():str() == "마")) then
		--This is verb 말다
		return verb
	end]]
	
	if verb:isValid() then
	
		local lastSyl = verb:last()
		
		if not lastSyl:hasPatchim() then
			
			if lastSyl:middleChamoStr() == "ᅧ" then
				lastSyl:setMiddleChamoStr("ᅵ")
				verb = verb:pop()
				verb = verb:push(lastSyl)
				verb = verb:push(CreateSyllable("어"))
				return verb
			end
			

			if lastSyl:middleChamoStr() == "ᅯ" then
				lastSyl:setMiddleChamoStr("ᅮ")
				verb = verb:pop()
				verb = verb:push(lastSyl)
				verb = verb:push(CreateSyllable("어"))
				return verb
			end
			
			if lastSyl:middleChamoStr() == "ᅪ" then
				lastSyl:setMiddleChamoStr("ᅩ")
				verb = verb:pop()
				verb = verb:push(lastSyl)
				verb = verb:push(CreateSyllable("아"))
				return verb
			end
			
			if (lastSyl:middleChamoStr() == "ᅥ") and (lastSyl:str() ~= "어") then
				verb = verb:push(CreateSyllable("어"))
				return verb
			end

		end
	end
	
	return verb
end


--Xperimental
function ReplacePoliteEndingToPanmalIfPossible(verb)

	if verb:isValid() then
		local modVerb = verb:cutEnd("요")
		if modVerb:isValid() then
			if modVerb:size() >= 2 then
				if (modVerb:last():str() == "세") then
					modVerb = modVerb:pop():push(CreateSyllable("시")):push(CreateSyllable("어"))
				end
			end		
			return modVerb			
		end	
	end
	return EmptySyllableWord()
end



function TryExtractVerbModificator_Infinitive(verb)

	return verb:cutEnd("다")
end


function TryExtractVerbModificator_Panmal(verb)


	--verb:PrintOut()
	local verb = UnGlueEndingForVerbInSecondFormIfPossible(verb)
	
	--verb:PrintOut()
	
	if verb:isValid() then
		local modVerb = verb:replaceEnd("해", "하")
		if modVerb:isValid() then
			return modVerb
		end
		
		
		modVerb = verb:replaceEnd("돼", "되")
		if modVerb:isValid() then
			return modVerb
		end
		
		modVerb = verb:replaceEnd("마", "말")
		if modVerb:isValid() then
			return modVerb
		end

		modVerb = verb:cutEnd("아")
		if modVerb:isValid() then
			if modVerb:last():isLight() and not modVerb:last():patchimIsSS() then
				return modVerb
			end
		end
		
		modVerb = verb:cutEnd("어")
		if modVerb:isValid() then
			if not modVerb:last():isLight() or modVerb:last():patchimIsSS() then
				return modVerb
			end
		end
		
		
		return EmptySyllableWord()

	end
	
	return EmptySyllableWord()
end


function TryExtractVerbModificator_Polite(verb)

	return TryExtractVerbModificator_Panmal(ReplacePoliteEndingToPanmalIfPossible(verb))
end

function TryExtractVerbModificator_Honorific(verb)

	return verb:cutVerbEndVarious("습니다", "ᆸ니다")
end


function TryExtractVerbModificator_Honorific_irregularL(verb)
	
	return TryExtractVerbModificator_Honorific(verb):glueIrregularL()
end



function TryExtractVerbModificator_HonorificQuestion(verb)

	return verb:cutVerbEndVarious("습니까", "ᆸ니까")
end

function TryExtractVerbModificator_HonorificQuestion_irregularL(verb)
	return TryExtractVerbModificator_HonorificQuestion(verb):glueIrregularL()
end





function TryExtractVerbModificator_If(verb)

	return verb:cutVerbEndVarious("으면", "면")

end



function TryExtractVerbModificator_And(verb)

	return verb:cutEnd("고")
	
end


function TryExtractVerbModificator_So(verb)
	return TryExtractVerbModificator_Panmal(verb:cutEnd("서"))
end


function TryExtractVerbModificator_Chi(verb)

	return verb:cutEnd("지")
	
end

function TryExtractVerbModificator_Although(verb)

	return verb:cutEnd("지만")
end

function TryExtractVerbModificator_PastParticiple(verb)
	return verb:cutVerbEndVarious("은", "ᆫ")
end

function TryExtractVerbModificator_PastParticiple_irregularL(verb)
	return TryExtractVerbModificator_PastParticiple(verb):glueIrregularL()
end


function TryExtractVerbModificator_PresentParticiple(verb)

	return verb:cutEnd("는")
end

function TryExtractVerbModificator_PresentParticiple_irregularL(verb)
	return TryExtractVerbModificator_PresentParticiple(verb):glueIrregularL()
end


function TryExtractVerbModificator_FutureParticiple(verb)
	return verb:cutVerbEndVarious("을", "ᆯ")
end


function TryExtractVerbModificator_FutureParticiple_irregularL(verb)
	return TryExtractVerbModificator_FutureParticiple(verb):glueIrregularL()
end


function TryExtractVerbModificator_Too(verb)

	return TryExtractVerbModificator_Panmal(verb:cutEnd("도"))
end


function TryExtractVerbModificator_Reason(verb)
	return verb:cutVerbEndVarious("으니까", "니까")
end

function TryExtractVerbModificator_Reason_irregularL(verb)
	return TryExtractVerbModificator_Reason(verb):glueIrregularL()
end

function TryExtractVerbModificator_Background(verb)
	return verb:cutEnd("는데")
end

function TryExtractVerbModificator_Background_irregularL(verb)
	return TryExtractVerbModificator_Background(verb):glueIrregularL()
end

function TryExtractVerbModificator_PanmalQuestionNi(verb)
	return verb:cutEnd("니") -- is more soft than 냐
end

function TryExtractVerbModificator_PanmalQuestionNi_irregularL(verb)
	return TryExtractVerbModificator_PanmalQuestionNi(verb):glueIrregularL()
end

function TryExtractVerbModificator_PanmalQuestionNya(verb)
	return verb:cutEnd("냐") -- is more soft than 니
end

function TryExtractVerbModificator_PanmalQuestionNya_irregularL(verb)
	return TryExtractVerbModificator_PanmalQuestionNya(verb):glueIrregularL()
end

function TryExtractVerbModificator_PanmalQuestionNiya(verb)
	return verb:cutEnd("니야")
end

function TryExtractVerbModificator_PanmalQuestionNiya_irregularL(verb)
	return TryExtractVerbModificator_PanmalQuestionNiya(verb):glueIrregularL()
end

function TryExtractVerbModificator_Expressed(verb)
	return verb:cutEnd("네요")
end

function TryExtractVerbModificator_Expressed_irregularL(verb)
	return TryExtractVerbModificator_Expressed(verb):glueIrregularL()
end


function TryExtractVerbModificator_DuringAction(verb)
	return verb:cutEnd("다가") --difference between 다가 and 는데
end

function TryExtractVerbModificator_FarPastAction(verb)
	return verb:cutEnd("던")
end

function TryExtractVerbModificator_MultipleAction(verb)
	return verb:cutEnd("든지")
end

function TryExtractVerbModificator_WhenAction(verb)
	return verb:cutVerbEndVarious("을때", "ᆯ때") -- When action
--[[
때 - when 

]]	
end

function TryExtractVerbModificator_WhenAction_irregularL(verb)
	return TryExtractVerbModificator_WhenAction(verb):glueIrregularL()
end


function TryExtractVerbModificator_Mention(verb)
	return verb:cutVerbEndVarious("으라고", "라고")
	-- 다고 is action of my willing, and 라고 means forced (command)
	-- "니는 먹는다고" - [I said to him] I am eating
end

function TryExtractVerbModificator_TendAction(verb)
	return verb:cutVerbEndVarious("으려고", "려고")
end

function TryExtractVerbModificator_TheMoreThe(verb)

	return verb:cutVerbEndVarious("을수록", "ᆯ수록")
end

function TryExtractVerbModificator_TheMoreThe_irregularL(verb)
	return TryExtractVerbModificator_TheMoreThe(verb):glueIrregularL()
end



function TryExtractVerbModificator_Um(verb)
	return verb:cutEnd("음")
end

function TryExtractVerbModificator_Cha(verb)
	return verb:cutEnd("자") --Let's do it (panmal)
end

--가자 - let's do it
--갈까요 - shall we?

function TryExtractVerbModificator_PoliteSure(verb)
	return verb:cutEnd("지요")
end

function TryExtractVerbModificator_PoliteSureShort(verb)
	return verb:cutEnd("죠")
end

function TryExtractVerbModificator_ImperativePolite(verb)
	return TryExtractVerbModificator_Panmal(verb:cutEnd("세요"))
end

function TryExtractVerbModificator_ImperativePanmal(verb)
	return TryExtractVerbModificator_Panmal(verb:cutEnd("라"))
end

function TryExtractVerbModificator_ActionAsResult(verb)
	return verb:cutVerbEndVarious("을게", "ᆯ게")
end

function TryExtractVerbModificator_ActionAsResult_irregularL(verb)
	return TryExtractVerbModificator_ActionAsResult(verb):glueIrregularL()
end


function TryExtractVerbModificator_ForbidShortPanmal(verb)
	return verb:cutEnd("지마")
end

function TryExtractVerbModificator_ForbidShortPolite(verb)
	return verb:cutEnd("지마요")
end

function TryExtractVerbModificator_ForbidShortImperativePolite(verb)
	return verb:cutEnd("지마세요")
end

function TryExtractVerbModificator_Gerund(verb)
	return verb:cutEnd("기")
end

function TryExtractVerbModificator_GerundToo(verb)
	return verb:cutEnd("기도")
end

function TryExtractVerbModificator_GerundOnly(verb)
	return verb:cutEnd("기만")
end

function TryExtractVerbModificator_DoItForMeShortPanmal(verb)
	return verb:cutEnd("줘")
end

function TryExtractVerbModificator_DoItForMeShortPolite(verb)
	return verb:cutEnd("줘요")
end

function TryExtractVerbModificator_DoItForMeShortImperativePolite(verb)
	return verb:cutEnd("주세요")
end

function TryExtractVerbModificator_Ya(verb)
	return TryExtractVerbModificator_Panmal(verb:cutEnd("야"))
end



function TryExtractVerbModificator_PastTense(verb)

	return TryExtractVerbModificator_Panmal(verb:cutEnd("ᆻ"))
	

end

function TryExtractVerbModificator_DoublePastTense(verb)

	--OutputPrint("TryExtractVerbModificator_DoublePastTense ")
	--verb:PrintOut()
	
	local result = verb:cutEnd("었"):cutEnd("ᆻ")
	--result:PrintOut()
	result = TryExtractVerbModificator_Panmal(result)
	--result:PrintOut()

	return result
end



function TryExtractVerbModificator_Will(verb)
	return verb:cutEnd("겠")
end


function TryExtractVerbModificator_Si(verb)
	--OutputPrint("TryExtractVerbModificator_Si ")
	--verb:PrintOut()
	local result = verb:cutVerbEndVarious("으시", "시")
	--result:PrintOut()
	return result
end

function TryExtractVerbModificator_Si_irregularL(verb)
	--OutputPrint("TryExtractVerbModificator_Si_irregularL ")
	--verb:PrintOut()
	local result = verb:cutEnd("시"):glueIrregularL()
	--result:PrintOut()
	return result
end


function TryExtractVerbModificator_Not(verb)

	return verb:cutBegin("안")
end

function TryExtractVerbModificator_CanNot(verb)
	return verb:cutBegin("못")
end

VerbMap = {}
VerbMap[1] = {}

VerbMap[1]["VC_INFINITIVE"] = {}
VerbMap[1]["VC_INFINITIVE"].testFunc = TryExtractVerbModificator_Infinitive
VerbMap[1]["VC_INFINITIVE"].verbose = "В инфинитиве"
VerbMap[1]["VC_INFINITIVE"].lesson = "lessons/korean010.html"

VerbMap[1]["VC_PANMAL"] = {}
VerbMap[1]["VC_PANMAL"].testFunc = TryExtractVerbModificator_Panmal
VerbMap[1]["VC_PANMAL"].verbose = "В просторечной (панмаль) форме"
VerbMap[1]["VC_PANMAL"].lesson = ""

VerbMap[1]["VC_POLITE"] = {}
VerbMap[1]["VC_POLITE"].testFunc = TryExtractVerbModificator_Polite
VerbMap[1]["VC_POLITE"].verbose = "В простой вежливой форме"
VerbMap[1]["VC_POLITE"].lesson = "lessons/korean011.html"

VerbMap[1]["VC_HONORIFIC"] = {}
VerbMap[1]["VC_HONORIFIC"].testFunc = TryExtractVerbModificator_Honorific
VerbMap[1]["VC_HONORIFIC"].verbose = "В почтительной форме"
VerbMap[1]["VC_HONORIFIC"].lesson = ""

VerbMap[1]["VC_HONORIFIC_IRREGULAR_L"] = {}
VerbMap[1]["VC_HONORIFIC_IRREGULAR_L"].testFunc = TryExtractVerbModificator_Honorific_irregularL
VerbMap[1]["VC_HONORIFIC_IRREGULAR_L"].verbose = "В почтительной форме после неправильного глагола ㄹ-типа"
VerbMap[1]["VC_HONORIFIC_IRREGULAR_L"].lesson = ""

VerbMap[1]["VC_HONORIFIC_QUESTION"] = {}
VerbMap[1]["VC_HONORIFIC_QUESTION"].testFunc = TryExtractVerbModificator_HonorificQuestion
VerbMap[1]["VC_HONORIFIC_QUESTION"].verbose = "VC_HONORIFIC_QUESTION"
VerbMap[1]["VC_HONORIFIC_QUESTION"].lesson = "В почтительной форме, в виде вопроса"

VerbMap[1]["VC_HONORIFIC_QUESTION_IRREGULAR_L"] = {}
VerbMap[1]["VC_HONORIFIC_QUESTION_IRREGULAR_L"].testFunc = TryExtractVerbModificator_HonorificQuestion_irregularL
VerbMap[1]["VC_HONORIFIC_QUESTION_IRREGULAR_L"].verbose = "VC_HONORIFIC_QUESTION_IRREGULAR_L"
VerbMap[1]["VC_HONORIFIC_QUESTION_IRREGULAR_L"].lesson = "В почтительной форме, в виде вопроса, после неправильного глагола ㄹ-типа"

VerbMap[1]["VC_IF"] = {}
VerbMap[1]["VC_IF"].testFunc = TryExtractVerbModificator_If
VerbMap[1]["VC_IF"].verbose = "В форме условия (если <глагол> ...)"
VerbMap[1]["VC_IF"].lesson = "lessons/korean036.html"

VerbMap[1]["VC_AND"] = {}
VerbMap[1]["VC_AND"].testFunc = TryExtractVerbModificator_And
VerbMap[1]["VC_AND"].verbose = "С соединительным окончанием (<глагол> и ...)"
VerbMap[1]["VC_AND"].lesson = ""

VerbMap[1]["VC_SO"] = {}
VerbMap[1]["VC_SO"].testFunc = TryExtractVerbModificator_So
VerbMap[1]["VC_SO"].verbose = "С окончанием следствия (так как <глагол>, поэтому ...)"
VerbMap[1]["VC_SO"].lesson = ""

VerbMap[1]["VC_CHI"] = {}
VerbMap[1]["VC_CHI"].testFunc = TryExtractVerbModificator_Chi
VerbMap[1]["VC_CHI"].verbose = "С модификатором уверенности"
VerbMap[1]["VC_CHI"].lesson = ""

VerbMap[1]["VC_ALTHOUGH"] = {}
VerbMap[1]["VC_ALTHOUGH"].testFunc = TryExtractVerbModificator_Although
VerbMap[1]["VC_ALTHOUGH"].verbose = "С окончанием 'хотя' (хотя <глагол> ...)"
VerbMap[1]["VC_ALTHOUGH"].lesson = ""

VerbMap[1]["VC_PAST_PARTICIPLE"] = {}
VerbMap[1]["VC_PAST_PARTICIPLE"].testFunc = TryExtractVerbModificator_PastParticiple
VerbMap[1]["VC_PAST_PARTICIPLE"].verbose = "В форме причастия прошедшего времени"
VerbMap[1]["VC_PAST_PARTICIPLE"].lesson = "lessons/korean041.html"

VerbMap[1]["VC_PAST_PARTICIPLE_IRREGULAR_L"] = {}
VerbMap[1]["VC_PAST_PARTICIPLE_IRREGULAR_L"].testFunc = TryExtractVerbModificator_PastParticiple_irregularL
VerbMap[1]["VC_PAST_PARTICIPLE_IRREGULAR_L"].verbose = "В форме причастия прошедшего времени, после неправильного глагола ㄹ-типа"
VerbMap[1]["VC_PAST_PARTICIPLE_IRREGULAR_L"].lesson = "lessons/korean041.html"

VerbMap[1]["VC_PRESENT_PARTICIPLE"] = {}
VerbMap[1]["VC_PRESENT_PARTICIPLE"].testFunc = TryExtractVerbModificator_PresentParticiple
VerbMap[1]["VC_PRESENT_PARTICIPLE"].verbose = "В форме причастия настоящего времени"
VerbMap[1]["VC_PRESENT_PARTICIPLE"].lesson = "lessons/korean041.html"

VerbMap[1]["VC_PRESENT_PARTICIPLE_IRREGULAR_L"] = {}
VerbMap[1]["VC_PRESENT_PARTICIPLE_IRREGULAR_L"].testFunc = TryExtractVerbModificator_PresentParticiple_irregularL
VerbMap[1]["VC_PRESENT_PARTICIPLE_IRREGULAR_L"].verbose = "В форме причастия прошедшего времени, после неправильного глагола ㄹ-типа"
VerbMap[1]["VC_PRESENT_PARTICIPLE_IRREGULAR_L"].lesson = "lessons/korean041.html"


VerbMap[1]["VC_FUTURE_PARTICIPLE"] = {}
VerbMap[1]["VC_FUTURE_PARTICIPLE"].testFunc = TryExtractVerbModificator_FutureParticiple
VerbMap[1]["VC_FUTURE_PARTICIPLE"].verbose = "В форме причастия будущего времени"
VerbMap[1]["VC_FUTURE_PARTICIPLE"].lesson = "lessons/korean041.html"


VerbMap[1]["VC_FUTURE_PARTICIPLE_IRREGULAR_L"] = {}
VerbMap[1]["VC_FUTURE_PARTICIPLE_IRREGULAR_L"].testFunc = TryExtractVerbModificator_FutureParticiple_irregularL
VerbMap[1]["VC_FUTURE_PARTICIPLE_IRREGULAR_L"].verbose = "В форме причастия будущего времени, после неправильного глагола ㄹ-типа"
VerbMap[1]["VC_FUTURE_PARTICIPLE_IRREGULAR_L"].lesson = "lessons/korean041.html"



VerbMap[1]["VC_TOO"] = {}
VerbMap[1]["VC_TOO"].testFunc = TryExtractVerbModificator_Too
VerbMap[1]["VC_TOO"].verbose = "С окончанием 'тоже', 'также'"
VerbMap[1]["VC_TOO"].lesson = "lessons/korean032.html"

VerbMap[1]["VC_REASON"] = {}
VerbMap[1]["VC_REASON"].testFunc = TryExtractVerbModificator_Reason
VerbMap[1]["VC_REASON"].verbose = "С окончанием причины (потому что <глагол>)"
VerbMap[1]["VC_REASON"].lesson = ""

VerbMap[1]["VC_REASON_IRREGULAR_L"] = {}
VerbMap[1]["VC_REASON_IRREGULAR_L"].testFunc = TryExtractVerbModificator_Reason_irregularL
VerbMap[1]["VC_REASON_IRREGULAR_L"].verbose = "С окончанием причины (потому что <глагол>), после неправильного глагола ㄹ-типа"
VerbMap[1]["VC_REASON_IRREGULAR_L"].lesson = ""

VerbMap[1]["VC_BACKGROUND"] = {}
VerbMap[1]["VC_BACKGROUND"].testFunc = TryExtractVerbModificator_Background
VerbMap[1]["VC_BACKGROUND"].verbose = "С модификатором фона, несущественности"
VerbMap[1]["VC_BACKGROUND"].lesson = ""


VerbMap[1]["VC_BACKGROUND_IRREGULAR_L"] = {}
VerbMap[1]["VC_BACKGROUND_IRREGULAR_L"].testFunc = TryExtractVerbModificator_Background_irregularL
VerbMap[1]["VC_BACKGROUND_IRREGULAR_L"].verbose = "С модификатором фона, несущественности, после неправильного глагола ㄹ-типа"
VerbMap[1]["VC_BACKGROUND_IRREGULAR_L"].lesson = ""

VerbMap[1]["VC_PANMAL_QUESTION_NI"] = {}
VerbMap[1]["VC_PANMAL_QUESTION_NI"].testFunc = TryExtractVerbModificator_PanmalQuestionNi
VerbMap[1]["VC_PANMAL_QUESTION_NI"].verbose = "В просторечной (панмаль) форме, в виде вопроса"
VerbMap[1]["VC_PANMAL_QUESTION_NI"].lesson = ""

VerbMap[1]["VC_PANMAL_QUESTION_NI_IRREGULAR_L"] = {}
VerbMap[1]["VC_PANMAL_QUESTION_NI_IRREGULAR_L"].testFunc = TryExtractVerbModificator_PanmalQuestionNi_irregularL
VerbMap[1]["VC_PANMAL_QUESTION_NI_IRREGULAR_L"].verbose = "В просторечной (панмаль) форме, в виде вопроса, после неправильного глагола ㄹ-типа"
VerbMap[1]["VC_PANMAL_QUESTION_NI_IRREGULAR_L"].lesson = ""

VerbMap[1]["VC_PANMAL_QUESTION_NYA"] = {}
VerbMap[1]["VC_PANMAL_QUESTION_NYA"].testFunc = TryExtractVerbModificator_PanmalQuestionNya
VerbMap[1]["VC_PANMAL_QUESTION_NYA"].verbose = "В просторечной (панмаль) форме, в виде вопроса"
VerbMap[1]["VC_PANMAL_QUESTION_NYA"].lesson = ""

VerbMap[1]["VC_PANMAL_QUESTION_NYA_IRREGULAR_L"] = {}
VerbMap[1]["VC_PANMAL_QUESTION_NYA_IRREGULAR_L"].testFunc = TryExtractVerbModificator_PanmalQuestionNya_irregularL
VerbMap[1]["VC_PANMAL_QUESTION_NYA_IRREGULAR_L"].verbose = "В просторечной (панмаль) форме, в виде вопроса, после неправильного глагола ㄹ-типа"
VerbMap[1]["VC_PANMAL_QUESTION_NYA_IRREGULAR_L"].lesson = ""

VerbMap[1]["VC_PANMAL_QUESTION_NIYA"] = {}
VerbMap[1]["VC_PANMAL_QUESTION_NIYA"].testFunc = TryExtractVerbModificator_PanmalQuestionNiya
VerbMap[1]["VC_PANMAL_QUESTION_NIYA"].verbose = "В просторечной (панмаль) форме, в виде вопроса"
VerbMap[1]["VC_PANMAL_QUESTION_NIYA"].lesson = ""

VerbMap[1]["VC_PANMAL_QUESTION_NIYA_IRREGULAR_L"] = {}
VerbMap[1]["VC_PANMAL_QUESTION_NIYA_IRREGULAR_L"].testFunc = TryExtractVerbModificator_PanmalQuestionNiya_irregularL
VerbMap[1]["VC_PANMAL_QUESTION_NIYA_IRREGULAR_L"].verbose = "В просторечной (панмаль) форме, в виде вопроса, после неправильного глагола ㄹ-типа"
VerbMap[1]["VC_PANMAL_QUESTION_NIYA_IRREGULAR_L"].lesson = ""

VerbMap[1]["VC_EXPRESSED"] = {}
VerbMap[1]["VC_EXPRESSED"].testFunc = TryExtractVerbModificator_Expressed
VerbMap[1]["VC_EXPRESSED"].verbose = "В вежливой форме с модификатором экспрессии, возбуждения"
VerbMap[1]["VC_EXPRESSED"].lesson = ""

VerbMap[1]["VC_EXPRESSED_IRREGULAR_L"] = {}
VerbMap[1]["VC_EXPRESSED_IRREGULAR_L"].testFunc = TryExtractVerbModificator_Expressed_irregularL
VerbMap[1]["VC_EXPRESSED_IRREGULAR_L"].verbose = "В вежливой форме с модификатором экспрессии, возбуждения, после неправильного глагола ㄹ-типа"
VerbMap[1]["VC_EXPRESSED_IRREGULAR_L"].lesson = ""

VerbMap[1]["VC_DURING_ACTION"] = {}
VerbMap[1]["VC_DURING_ACTION"].testFunc = TryExtractVerbModificator_DuringAction
VerbMap[1]["VC_DURING_ACTION"].verbose = "С окончанием дополнительного действия (во время <глагол>, ...)"
VerbMap[1]["VC_DURING_ACTION"].lesson = ""

VerbMap[1]["VC_FAR_PAST_ACTION"] = {}
VerbMap[1]["VC_FAR_PAST_ACTION"].testFunc = TryExtractVerbModificator_FarPastAction
VerbMap[1]["VC_FAR_PAST_ACTION"].verbose = "С окончанием действия, произошедшего в далеком прошлом"
VerbMap[1]["VC_FAR_PAST_ACTION"].lesson = ""

VerbMap[1]["VC_MULTIPLE_ACTION"] = {}
VerbMap[1]["VC_MULTIPLE_ACTION"].testFunc = TryExtractVerbModificator_MultipleAction
VerbMap[1]["VC_MULTIPLE_ACTION"].verbose = "С окончанием действия, выполненного несколько раз"
VerbMap[1]["VC_MULTIPLE_ACTION"].lesson = ""

VerbMap[1]["VC_WHEN_ACTION"] = {}
VerbMap[1]["VC_WHEN_ACTION"].testFunc = TryExtractVerbModificator_WhenAction
VerbMap[1]["VC_WHEN_ACTION"].verbose = "С окончанием действия, выполненного во время другого действия (когда <глагол>, ...)"
VerbMap[1]["VC_WHEN_ACTION"].lesson = ""

VerbMap[1]["VC_WHEN_ACTION_IRREGULAR_L"] = {}
VerbMap[1]["VC_WHEN_ACTION_IRREGULAR_L"].testFunc = TryExtractVerbModificator_WhenAction_irregularL
VerbMap[1]["VC_WHEN_ACTION_IRREGULAR_L"].verbose = "С окончанием действия, выполненного во время другого действия (когда <глагол>, ...), после неправильного глагола ㄹ-типа"
VerbMap[1]["VC_WHEN_ACTION_IRREGULAR_L"].lesson = ""


VerbMap[1]["VC_MENTION"] = {}
VerbMap[1]["VC_MENTION"].testFunc = TryExtractVerbModificator_Mention
VerbMap[1]["VC_MENTION"].verbose = "С окончанием упоминания"
VerbMap[1]["VC_MENTION"].lesson = ""

VerbMap[1]["VC_TEND_ACTION"] = {}
VerbMap[1]["VC_TEND_ACTION"].testFunc = TryExtractVerbModificator_TendAction
VerbMap[1]["VC_TEND_ACTION"].verbose = "С окончанием намерения"
VerbMap[1]["VC_TEND_ACTION"].lesson = ""

VerbMap[1]["VC_THE_MORE_THE"] = {}
VerbMap[1]["VC_THE_MORE_THE"].testFunc = TryExtractVerbModificator_TheMoreThe
VerbMap[1]["VC_THE_MORE_THE"].verbose = "С окончанием 'чем больше <глагол>, тем ...'"
VerbMap[1]["VC_THE_MORE_THE"].lesson = ""

VerbMap[1]["VC_THE_MORE_THE_IRREGULAR_L"] = {}
VerbMap[1]["VC_THE_MORE_THE_IRREGULAR_L"].testFunc = TryExtractVerbModificator_TheMoreThe_irregularL
VerbMap[1]["VC_THE_MORE_THE_IRREGULAR_L"].verbose = "С окончанием 'чем больше <глагол>, тем ...', после неправильного глагола ㄹ-типа"
VerbMap[1]["VC_THE_MORE_THE_IRREGULAR_L"].lesson = ""

VerbMap[1]["VC_UM"] = {}
VerbMap[1]["VC_UM"].testFunc = TryExtractVerbModificator_Um
VerbMap[1]["VC_UM"].verbose = "С окончанием отглагольного существительного -음"
VerbMap[1]["VC_UM"].lesson = ""

VerbMap[1]["VC_CHA"] = {}
VerbMap[1]["VC_CHA"].testFunc = TryExtractVerbModificator_Cha
VerbMap[1]["VC_CHA"].verbose = "С окончанием предложения к действию (давайте <глагол>)"
VerbMap[1]["VC_CHA"].lesson = ""

VerbMap[1]["VC_POLITE_SURE"] = {}
VerbMap[1]["VC_POLITE_SURE"].testFunc = TryExtractVerbModificator_PoliteSure
VerbMap[1]["VC_POLITE_SURE"].verbose = "В вежливой форме, с модификатором уверенности"
VerbMap[1]["VC_POLITE_SURE"].lesson = ""

VerbMap[1]["VC_POLITE_SURE_SHORT"] = {}
VerbMap[1]["VC_POLITE_SURE_SHORT"].testFunc = TryExtractVerbModificator_PoliteSureShort
VerbMap[1]["VC_POLITE_SURE_SHORT"].verbose = "В вежливой форме, с модификатором уверенности, в сокращенной форме"
VerbMap[1]["VC_POLITE_SURE_SHORT"].lesson = ""



VerbMap[1]["VC_IMPERATIVE_POLITE"] = {}
VerbMap[1]["VC_IMPERATIVE_POLITE"].testFunc = TryExtractVerbModificator_ImperativePolite
VerbMap[1]["VC_IMPERATIVE_POLITE"].verbose = "В вежливой форме, в повелительном наклонении"
VerbMap[1]["VC_IMPERATIVE_POLITE"].lesson = "lessons/korean034.html"


VerbMap[1]["VC_IMPERATIVE_PANMAL"] = {}
VerbMap[1]["VC_IMPERATIVE_PANMAL"].testFunc = TryExtractVerbModificator_ImperativePanmal
VerbMap[1]["VC_IMPERATIVE_PANMAL"].verbose = "В просторечной (панмаль) форме, в повелительном наклонении"
VerbMap[1]["VC_IMPERATIVE_PANMAL"].lesson = ""

VerbMap[1]["VC_ACTION_AS_RESULT"] = {}
VerbMap[1]["VC_ACTION_AS_RESULT"].testFunc = TryExtractVerbModificator_ActionAsResult
VerbMap[1]["VC_ACTION_AS_RESULT"].verbose = "В форме действия как результата"
VerbMap[1]["VC_ACTION_AS_RESULT"].lesson = ""

VerbMap[1]["VC_ACTION_AS_RESULT_IRREGULAR_L"] = {}
VerbMap[1]["VC_ACTION_AS_RESULT_IRREGULAR_L"].testFunc = TryExtractVerbModificator_ActionAsResult_irregularL
VerbMap[1]["VC_ACTION_AS_RESULT_IRREGULAR_L"].verbose = "В форме действия как результата, после неправильного глагола ㄹ-типа"
VerbMap[1]["VC_ACTION_AS_RESULT_IRREGULAR_L"].lesson = ""


VerbMap[1]["VC_FORBID_SHORT_PANMAL"] = {}
VerbMap[1]["VC_FORBID_SHORT_PANMAL"].testFunc = TryExtractVerbModificator_ForbidShortPanmal
VerbMap[1]["VC_FORBID_SHORT_PANMAL"].verbose = "В просторечной (панмаль) форме, в виде запрета 'не делай это', в сокращенном виде"
VerbMap[1]["VC_FORBID_SHORT_PANMAL"].lesson = "lessons/korean038.html"

--Xperimental -- maybe not exist at all???
VerbMap[1]["VC_FORBID_SHORT_POLITE"] = {}
VerbMap[1]["VC_FORBID_SHORT_POLITE"].testFunc = TryExtractVerbModificator_ForbidShortPolite
VerbMap[1]["VC_FORBID_SHORT_POLITE"].verbose = "В вежливой форме, в виде запрета 'не делай это', в сокращенном виде"
VerbMap[1]["VC_FORBID_SHORT_POLITE"].lesson = "lessons/korean038.html"


VerbMap[1]["VC_FORBID_SHORT_IMPERATIVE_POLITE"] = {}
VerbMap[1]["VC_FORBID_SHORT_IMPERATIVE_POLITE"].testFunc = TryExtractVerbModificator_ForbidShortImperativePolite
VerbMap[1]["VC_FORBID_SHORT_IMPERATIVE_POLITE"].verbose = "В вежливой форме, в виде запрета 'не делай это', в сокращенном виде"
VerbMap[1]["VC_FORBID_SHORT_IMPERATIVE_POLITE"].lesson = "lessons/korean038.html"

VerbMap[1]["VC_GERUND"] = {}
VerbMap[1]["VC_GERUND"].testFunc = TryExtractVerbModificator_Gerund
VerbMap[1]["VC_GERUND"].verbose = "В форме герундия (глагол -> делание)"
VerbMap[1]["VC_GERUND"].lesson = ""


VerbMap[1]["VC_GERUND_TOO"] = {}
VerbMap[1]["VC_GERUND_TOO"].testFunc = TryExtractVerbModificator_GerundToo
VerbMap[1]["VC_GERUND_TOO"].verbose = "В форме герундия с модификатором 'тоже', 'также'"
VerbMap[1]["VC_GERUND_TOO"].lesson = "lessons/korean032.html"


VerbMap[1]["VC_GERUND_ONLY"] = {}
VerbMap[1]["VC_GERUND_ONLY"].testFunc = TryExtractVerbModificator_GerundOnly
VerbMap[1]["VC_GERUND_ONLY"].verbose = "В форме герундия с модификатором 'только', 'лишь'"
VerbMap[1]["VC_GERUND_ONLY"].lesson = "lessons/korean033.html"


VerbMap[1]["VC_DO_IT_FOR_ME_SHORT_PANMAL"] = {}
VerbMap[1]["VC_DO_IT_FOR_ME_SHORT_PANMAL"].testFunc = TryExtractVerbModificator_DoItForMeShortPanmal
VerbMap[1]["VC_DO_IT_FOR_ME_SHORT_PANMAL"].verbose = "В просторечной (панмаль) форме, с модификатором 'сделай это для меня'"
VerbMap[1]["VC_DO_IT_FOR_ME_SHORT_PANMAL"].lesson = "lessons/korean035.html"

--Xperimental, maybe not exist at all?
VerbMap[1]["VC_DO_IT_FOR_ME_SHORT_POLITE"] = {}
VerbMap[1]["VC_DO_IT_FOR_ME_SHORT_POLITE"].testFunc = TryExtractVerbModificator_DoItForMeShortPolite
VerbMap[1]["VC_DO_IT_FOR_ME_SHORT_POLITE"].verbose = "В вежливой форме, с модификатором 'сделай это для меня'"
VerbMap[1]["VC_DO_IT_FOR_ME_SHORT_POLITE"].lesson = "lessons/korean035.html"

VerbMap[1]["VC_DO_IT_FOR_ME_SHORT_IMPERATIVE_POLITE"] = {}
VerbMap[1]["VC_DO_IT_FOR_ME_SHORT_IMPERATIVE_POLITE"].testFunc = TryExtractVerbModificator_DoItForMeShortImperativePolite
VerbMap[1]["VC_DO_IT_FOR_ME_SHORT_IMPERATIVE_POLITE"].verbose = "В вежливой форме, с модификатором 'сделай это для меня'"
VerbMap[1]["VC_DO_IT_FOR_ME_SHORT_IMPERATIVE_POLITE"].lesson = "lessons/korean035.html"

--Xperimental
VerbMap[1]["VC_YA"] = {}
VerbMap[1]["VC_YA"].testFunc = TryExtractVerbModificator_Ya
VerbMap[1]["VC_YA"].verbose = "С окончанием -야"
VerbMap[1]["VC_YA"].lesson = ""


VerbMap[2] = {}

VerbMap[2]["VB_WILL"] = {}
VerbMap[2]["VB_WILL"].testFunc = TryExtractVerbModificator_Will
VerbMap[2]["VB_WILL"].verbose = "В форме намерения"
VerbMap[2]["VB_WILL"].lesson = ""

VerbMap[3] = {}

VerbMap[3]["VB_PAST_TENSE"] = {}
VerbMap[3]["VB_PAST_TENSE"].testFunc = TryExtractVerbModificator_PastTense
VerbMap[3]["VB_PAST_TENSE"].verbose = "В прошедшем времени"
VerbMap[3]["VB_PAST_TENSE"].lesson = "lessons/korean015.html"

VerbMap[3]["VB_DOUBLE_PAST_TENSE"] = {}
VerbMap[3]["VB_DOUBLE_PAST_TENSE"].testFunc = TryExtractVerbModificator_DoublePastTense
VerbMap[3]["VB_DOUBLE_PAST_TENSE"].verbose = "С удвоенным модификатором прошедшего времени"
VerbMap[3]["VB_DOUBLE_PAST_TENSE"].lesson = "lessons/korean015.html"



VerbMap[4] = {}
VerbMap[4]["VM_SI"] = {}
VerbMap[4]["VM_SI"].testFunc = TryExtractVerbModificator_Si
VerbMap[4]["VM_SI"].verbose = "С дополнительной формой вежливости"
VerbMap[4]["VM_SI"].lesson = ""

VerbMap[4]["VM_SI_IRREGULAR_L"] = {}
VerbMap[4]["VM_SI_IRREGULAR_L"].testFunc = TryExtractVerbModificator_Si_irregularL
VerbMap[4]["VM_SI_IRREGULAR_L"].verbose = "С дополнительной формой вежливости при неправильном глаголе L-типа"
VerbMap[4]["VM_SI_IRREGULAR_L"].lesson = ""




VerbMap[5] = {}
VerbMap[5]["VP_NOT"] = {}
VerbMap[5]["VP_NOT"].testFunc = TryExtractVerbModificator_Not
VerbMap[5]["VP_NOT"].verbose = "С отрицанием"
VerbMap[5]["VP_NOT"].lesson = "lessons/korean018.html"

VerbMap[5]["VP_CAN_NOT"] = {}
VerbMap[5]["VP_CAN_NOT"].testFunc = TryExtractVerbModificator_CanNot
VerbMap[5]["VP_CAN_NOT"].verbose = "С отрицанием в смысле не мочь сделать"
VerbMap[5]["VP_CAN_NOT"].lesson = "lessons/korean043.html"


function RecognizeNoun(noun)
	
	local lessons = {}
	
	local result = {}

	local branches = {}
	
	branches[1] = {}
	branches[1].noun = noun
	branches[1].verbose = ""
	branches[1].lessons = {}
	branches[1].modificators = {}
	
	for i = 1, #NounMap do
	
		--OutputPrint("NounMap cycle " .. i .. " begin")
	
		--local oldBranchesCount = #branches
		
		--OutputPrint("Calculated count of branches")
		
		local oldBranches = {}
		
		for j = 1, #branches do
			oldBranches[j] = {}
			oldBranches[j].noun = branches[j].noun:clone()
			
			oldBranches[j].verbose = branches[j].verbose
			
			oldBranches[j].lessons = {}
			for k = 1, #branches[j].lessons do
				oldBranches[j].lessons[k] = branches[j].lessons[k]
			end
			
			oldBranches[j].modificators = {}
			for k = 1, #branches[j].modificators do
				oldBranches[j].modificators[k] = branches[j].modificators[k]
			end
		end
			
		for j = 1, #oldBranches do
		
			--OutputPrint("Branches cycle " .. j .. " begin with noun " .. oldBranches[j].noun:str())
		
			for k,v in pairs(NounMap[i]) do
			
				--OutputPrint("Test new rule " .. k .. " for branch [" .. j .. "] with noun " .. oldBranches[j].noun:str() .. " begin" )
							
				local modNoun = v.testFunc(oldBranches[j].noun:clone())
				
				if (modNoun:isValid()) then
					--OutputPrint("Rule is applied! Modnoun : " .. modNoun:str())
					--modNoun:PrintOut()
					
					local c = #branches + 1
					branches[c] = {}
					branches[c].noun = modNoun:clone()
					branches[c].verbose = oldBranches[j].verbose
					branches[c].lessons = {}
					for k = 1, #oldBranches[j].lessons do
						branches[c].lessons[k] = oldBranches[j].lessons[k]
					end
					
					branches[c].modificators = {}
					for k = 1, #oldBranches[j].modificators do
						branches[c].modificators[k] = oldBranches[j].modificators[k]
					end
					
					branches[c].verbose = branches[c].verbose .. v.verbose .. "\n"
				
					if (v.lesson ~= "") then
						table.insert(branches[c].lessons, v.lesson)
					end
					
					table.insert(branches[c].modificators, k)
					
				end
				
				--OutputPrint("Test new rule " .. k .. " for branch [" .. j .. "] with noun " .. oldBranches[j].noun:str() .. " end" )
			
			
			end
			
			--OutputPrint("Branches cycle " .. j .. " end with noun " .. oldBranches[j].noun:str())
		end
		
		--OutputPrint("NounMap cycle " .. i .. " end")
	end
	
	for i = 1, #branches do
		local dictStruct = luaHelper:FindNoun(branches[i].noun)
			
		if (dictStruct ~= nil) then
			local wordStruct = {}
			wordStruct.wordType = "NOUN"
			wordStruct.dictStruct = dictStruct
			wordStruct.verbose = branches[i].verbose
			wordStruct.lessons = branches[i].lessons
			wordStruct.modificators = branches[i].modificators

			table.insert(result, wordStruct)
		end
	end

	return result
end


function RecognizeVerb(verb)
	
	local lessons = {}
	
	local result = {}

	local branches = {}
	
	branches[1] = {}
	branches[1].verb = verb
	branches[1].verbose = ""
	branches[1].lessons = {}
	branches[1].modificators = {}
	
	for i = 1, #VerbMap do
	
		--OutputPrint("VerbMap cycle " .. i .. " begin")
	
		--local oldBranchesCount = #branches
		
		--OutputPrint("Calculated count of branches")
		
		local oldBranches = {}
		
		for j = 1, #branches do
			oldBranches[j] = {}
			oldBranches[j].verb = branches[j].verb:clone()
			oldBranches[j].verbose = branches[j].verbose
			oldBranches[j].lessons = {}
			for k = 1, #branches[j].lessons do
				oldBranches[j].lessons[k] = branches[j].lessons[k]
			end
			
			oldBranches[j].modificators = {}
			for k = 1, #branches[j].modificators do
				oldBranches[j].modificators[k] = branches[j].modificators[k]
			end
		end
			
		for j = 1, #oldBranches do
		
			--OutputPrint("Branches cycle " .. j .. " begin with verb " .. oldBranches[j].verb:str())
		
			for k,v in pairs(VerbMap[i]) do
			
				--OutputPrint("Test new rule " .. k .. " for branch [" .. j .. "] with verb " .. oldBranches[j].verb:str() .. " begin" )
			
				local modVerb = v.testFunc(oldBranches[j].verb:clone())
				
				if (modVerb:isValid()) then
					--OutputPrint("Rule is applied! Modverb : " .. modVerb:str())
					--modVerb:PrintOut()
					
					local c = #branches + 1
					branches[c] = {}
					branches[c].verb = modVerb:clone()
					branches[c].verbose = oldBranches[j].verbose
					branches[c].lessons = {}
					for k = 1, #oldBranches[j].lessons do
						branches[c].lessons[k] = oldBranches[j].lessons[k]
					end
					
					branches[c].modificators = {}
					for k = 1, #oldBranches[j].modificators do
						branches[c].modificators[k] = oldBranches[j].modificators[k]
					end
					
					branches[c].verbose = branches[c].verbose .. v.verbose .. "\n"
				
					if (v.lesson ~= "") then
						table.insert(branches[c].lessons, v.lesson)
					end
					
					table.insert(branches[c].modificators, k)
					
				end
				
				--OutputPrint("Test new rule " .. k .. " for branch [" .. j .. "] with verb " .. oldBranches[j].verb:str() .. " end" )
			
			
			end
			
			--OutputPrint("Branches cycle " .. j .. " end with verb " .. oldBranches[j].verb:str())
		end
		
		--OutputPrint("VerbMap cycle " .. i .. " end")
	end
	
	for i = 1, #branches do
		local dictStruct = luaHelper:FindVerb(branches[i].verb:push(CreateSyllable("다")))
			
		if (dictStruct ~= nil) then
			local wordStruct = {}
			wordStruct.wordType = "VERB"
			wordStruct.dictStruct = dictStruct
			wordStruct.verbose = branches[i].verbose
			wordStruct.lessons = branches[i].lessons
			wordStruct.modificators = branches[i].modificators

			
			table.insert(result, wordStruct)
		end
	end

	return result
end

function RecognizeChineseNumber(word)

	local result = {}
	local dictStruct = luaHelper:FindChineseNumber(word)
			
	if (dictStruct ~= nil) then
		local wordStruct = {}
		wordStruct.wordType = "CHINESE_NUMBER"
		wordStruct.dictStruct = dictStruct
		wordStruct.verbose = { "Число, записанное китайскими числительными" }
		wordStruct.lessons = { "lessons/korean014.html" }
		wordStruct.modificators = {}

			
		table.insert(result, wordStruct)
	end

	return result
end

function RecognizeWord(word)

	local wordArr = {}
	
	local nounArr = RecognizeNoun(word)
	
	for i=1, #nounArr do
		table.insert(wordArr, nounArr[i])
	end
	
	local verbArr = RecognizeVerb(word)
	
	for i=1, #verbArr do
		table.insert(wordArr, verbArr[i])
	end
	
	local chineseNumberArr = RecognizeChineseNumber(word)
	
	for i=1, #chineseNumberArr do
		table.insert(wordArr, chineseNumberArr[i])
	end
	
	local adverbDictStruct = luaHelper:FindAdverb(word)
	
	if adverbDictStruct ~= nil then
		local wordStruct = {}
		wordStruct.wordType = "SPECIAL"
		wordStruct.dictStruct = adverbDictStruct
		wordStruct.verbose = ""
		wordStruct.lessons = {}
		wordStruct.modificators = {}
		table.insert(wordArr, wordStruct)
	end
	
	local specialDictStruct = luaHelper:FindSpecial(word)
	
	if specialDictStruct ~= nil then
		local wordStruct = {}
		wordStruct.wordType = "SPECIAL"
		wordStruct.dictStruct = specialDictStruct
		wordStruct.verbose = ""
		wordStruct.lessons = {}
		wordStruct.modificators = {}
		table.insert(wordArr, wordStruct)
	end
	
	return wordArr
	
end


------- Complex verbs!  ----

function SecondaryVerbModificatorEqualsTo(wordStruct, modificator)
	if wordStruct.wordType == "VERB" then
		for i=1, #wordStruct.modificators do
			if wordStruct.modificators[i] == modificator then
				return true
			end
		end	
	end
	return false
end


function RecognizeSecondaryVerb_Chi(wordStruct)
	return SecondaryVerbModificatorEqualsTo(wordStruct, "VC_CHI")
end


function RecognizeSecondaryVerb_And(wordStruct)
	return SecondaryVerbModificatorEqualsTo(wordStruct, "VC_AND")
end

function RecognizeSecondaryVerb_GerundToo(wordStruct)
	return SecondaryVerbModificatorEqualsTo(wordStruct, "VC_GERUND_TOO")
end


function RecognizeSecondaryVerb_GerundOnly(wordStruct)
	return SecondaryVerbModificatorEqualsTo(wordStruct, "VC_GERUND_ONLY")
end

function RecognizeSecondaryVerb_Panmal(wordStruct)
	return SecondaryVerbModificatorEqualsTo(wordStruct, "VC_PANMAL")
end

function RecognizeSecondaryVerb_Ya(wordStruct)
	return SecondaryVerbModificatorEqualsTo(wordStruct, "VC_YA")
end



function MainVerbBaseEqualsTo(wordStruct, base)
	if wordStruct.wordType == "VERB" then
		return wordStruct.dictStruct.base == base
	end
	return false
end

function RecognizeMainVerb_AnyVerb(wordStruct)
	return wordStruct.wordType == "VERB"
end

function RecognizeMainVerb_No(wordStruct)
	return MainVerbBaseEqualsTo(wordStruct, "않다")
end

function RecognizeMainVerb_Want(wordStruct)
	return MainVerbBaseEqualsTo(wordStruct, "싶다")
end

function RecognizeMainVerb_Exist(wordStruct)
	return MainVerbBaseEqualsTo(wordStruct, "있다")
end

function RecognizeMainVerb_NotExist(wordStruct)
	return MainVerbBaseEqualsTo(wordStruct, "없다")
end

function RecognizeMainVerb_DontDo(wordStruct)
	return MainVerbBaseEqualsTo(wordStruct, "말다")
end

function RecognizeMainVerb_Do(wordStruct)
	return MainVerbBaseEqualsTo(wordStruct, "하다")
end

function RecognizeMainVerb_Give(wordStruct)
	return MainVerbBaseEqualsTo(wordStruct, "주다")
end


function RecognizeMainVerb_Possible(wordStruct)
	return MainVerbBaseEqualsTo(wordStruct, "되다")
end


ComplexVerbMap = {}

ComplexVerbMap["COMPLEX_VERB_AND"] = {}
ComplexVerbMap["COMPLEX_VERB_AND"].mainVerbFunc = RecognizeMainVerb_AnyVerb
ComplexVerbMap["COMPLEX_VERB_AND"].secondaryVerbFunc = RecognizeSecondaryVerb_And
ComplexVerbMap["COMPLEX_VERB_AND"].verbose = "Составной глагол из двух глаголов, соединенных модификатором 'И' (<глагол> и <глагол>)"
ComplexVerbMap["COMPLEX_VERB_AND"].lesson = ""

ComplexVerbMap["COMPLEX_VERB_NO"] = {}
ComplexVerbMap["COMPLEX_VERB_NO"].mainVerbFunc = RecognizeMainVerb_No
ComplexVerbMap["COMPLEX_VERB_NO"].secondaryVerbFunc = RecognizeSecondaryVerb_Chi
ComplexVerbMap["COMPLEX_VERB_NO"].verbose = "Составной глагол, с смыслом отрицания (не <глагол>)"
ComplexVerbMap["COMPLEX_VERB_NO"].lesson = "lessons/korean018.html"

ComplexVerbMap["COMPLEX_VERB_WANT"] = {}
ComplexVerbMap["COMPLEX_VERB_WANT"].mainVerbFunc = RecognizeMainVerb_Want
ComplexVerbMap["COMPLEX_VERB_WANT"].secondaryVerbFunc = RecognizeSecondaryVerb_And
ComplexVerbMap["COMPLEX_VERB_WANT"].verbose = "Составной глагол, с смыслом желания (хотеть <глагол>)"
ComplexVerbMap["COMPLEX_VERB_WANT"].lesson = "lessons/korean020.html"

ComplexVerbMap["COMPLEX_CONTINUOUS"] = {}
ComplexVerbMap["COMPLEX_CONTINUOUS"].mainVerbFunc = RecognizeMainVerb_Exist
ComplexVerbMap["COMPLEX_CONTINUOUS"].secondaryVerbFunc = RecognizeSecondaryVerb_And
ComplexVerbMap["COMPLEX_CONTINUOUS"].verbose = "Составной глагол, с смыслом настоящего продолженного времени"
ComplexVerbMap["COMPLEX_CONTINUOUS"].lesson = "lessons/korean029.html"

ComplexVerbMap["COMPLEX_CONTINUOUS_NEGATIVE"] = {}
ComplexVerbMap["COMPLEX_CONTINUOUS_NEGATIVE"].mainVerbFunc = RecognizeMainVerb_NotExist
ComplexVerbMap["COMPLEX_CONTINUOUS_NEGATIVE"].secondaryVerbFunc = RecognizeSecondaryVerb_And
ComplexVerbMap["COMPLEX_CONTINUOUS_NEGATIVE"].verbose = "Составной глагол, с смыслом настоящего продолженного времени, с отрицанием"
ComplexVerbMap["COMPLEX_CONTINUOUS_NEGATIVE"].lesson = "lessons/korean029.html"

ComplexVerbMap["COMPLEX_FORBID"] = {}
ComplexVerbMap["COMPLEX_FORBID"].mainVerbFunc = RecognizeMainVerb_DontDo
ComplexVerbMap["COMPLEX_FORBID"].secondaryVerbFunc = RecognizeSecondaryVerb_Chi
ComplexVerbMap["COMPLEX_FORBID"].verbose = "Составной глагол, с смыслом отрицания (не делай <глагол>)"
ComplexVerbMap["COMPLEX_FORBID"].lesson = "lessons/korean029.html"

ComplexVerbMap["COMPLEX_GERUND_TOO"] = {}
ComplexVerbMap["COMPLEX_GERUND_TOO"].mainVerbFunc = RecognizeMainVerb_Do
ComplexVerbMap["COMPLEX_GERUND_TOO"].secondaryVerbFunc = RecognizeSecondaryVerb_GerundToo
ComplexVerbMap["COMPLEX_GERUND_TOO"].verbose = "Составной глагол, c смыслом 'тоже', 'также' (<глагол> тоже)"
ComplexVerbMap["COMPLEX_GERUND_TOO"].lesson = "lessons/korean032.html"

ComplexVerbMap["COMPLEX_GERUND_ONLY"] = {}
ComplexVerbMap["COMPLEX_GERUND_ONLY"].mainVerbFunc = RecognizeMainVerb_Do
ComplexVerbMap["COMPLEX_GERUND_ONLY"].secondaryVerbFunc = RecognizeSecondaryVerb_GerundOnly
ComplexVerbMap["COMPLEX_GERUND_ONLY"].verbose = "Составной глагол, c смыслом 'только', 'лишь' (только <глагол>)"
ComplexVerbMap["COMPLEX_GERUND_ONLY"].lesson = "lessons/korean033.html"

ComplexVerbMap["COMPLEX_DO_IT_FOR_ME"] = {}
ComplexVerbMap["COMPLEX_DO_IT_FOR_ME"].mainVerbFunc = RecognizeMainVerb_Give
ComplexVerbMap["COMPLEX_DO_IT_FOR_ME"].secondaryVerbFunc = RecognizeSecondaryVerb_Panmal
ComplexVerbMap["COMPLEX_DO_IT_FOR_ME"].verbose = "Составной глагол, c смыслом 'сделай это для меня'"
ComplexVerbMap["COMPLEX_DO_IT_FOR_ME"].lesson = ""

ComplexVerbMap["COMPLEX_SHOULD"] = {}
ComplexVerbMap["COMPLEX_SHOULD"].mainVerbFunc = RecognizeMainVerb_Do
ComplexVerbMap["COMPLEX_SHOULD"].secondaryVerbFunc = RecognizeSecondaryVerb_Ya
ComplexVerbMap["COMPLEX_SHOULD"].verbose = "Составной глагол, c смыслом 'должен', в письменной форме"
ComplexVerbMap["COMPLEX_SHOULD"].lesson = ""


ComplexVerbMap["COMPLEX_SHOULD_VERBAL"] = {}
ComplexVerbMap["COMPLEX_SHOULD_VERBAL"].mainVerbFunc = RecognizeMainVerb_Possible
ComplexVerbMap["COMPLEX_SHOULD_VERBAL"].secondaryVerbFunc = RecognizeSecondaryVerb_Ya
ComplexVerbMap["COMPLEX_SHOULD_VERBAL"].verbose = "Составной глагол, c смыслом 'должен', в устной форме"
ComplexVerbMap["COMPLEX_SHOULD_VERBAL"].lesson = ""

function RecognizeChainedVerb(phraseTable)
	
	local result = {}
	
	if #phraseTable == 2 then
		local firstWordArr = phraseTable[1]
		local lastWordArr = phraseTable[2]
		
		
		for i = 1, #lastWordArr do
			for j = 1, #firstWordArr do
			
	
				for key, value in pairs(ComplexVerbMap) do
					if value.mainVerbFunc(lastWordArr[i]) and value.secondaryVerbFunc(firstWordArr[j]) then

						local wordStruct = {}
						wordStruct.wordType = "COMPLEX_VERB"
						wordStruct.mainWordStruct = lastWordArr[i]
						wordStruct.secondaryWordStruct = firstWordArr[j]
						wordStruct.verbose = value.verbose
						wordStruct.lessons = { }
						if value.lesson ~= "" then
							table.insert(wordStruct.lessons, value.lesson)
						end
		
						wordStruct.complexVerbType = key
						
						
						table.insert(result, wordStruct)
					
					end
				end
			
			end
		end			
	end
	
	return result
	
end

----------- General code ---------

function RecognizeRequestPhraseNew(requestPhrase)

	local result = {}
	
	local phraseTable = {}
		
	for i = 1, #requestPhrase do
		table.insert(phraseTable, RecognizeWord(requestPhrase[i]))
	end
	
	result["complexVerbArr"] = RecognizeChainedVerb(phraseTable)

	result["phraseTable"] = phraseTable
	
	return result
end




