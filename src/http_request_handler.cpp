

#include "http_request_handler.h"

#include "SE/misc.h"

namespace http {
namespace server {

request_handler::request_handler(LH::LuaHelper& iluaHelper)
	: luaHelper(iluaHelper)
{
}

void request_handler::handle_request(const request& req, reply& rep){
	try
	{
		//Xperimental - Might be optimized a lot:

		// :::::::::::::::::::::::::::::::::::::::

		std::string uri_context;
		url_decode(req.uri, uri_context);

		// :::::::::::::::::::::::::::::::::::::::

		if (req.request_content.size() == 0 || uri_context == "/") {
			reply_store_ErrorPt(rep, "Empty request uri!");
			return;
		}

		std::string jsonCode = std::string(&req.request_content[0], &req.request_content[0] + req.request_content.size());

		std::stringstream stream(jsonCode);

		boost::property_tree::ptree propertyTree;
		boost::property_tree::read_json(stream, propertyTree);


		BOOST_FOREACH(auto i, propertyTree)
		{
			if (i.first == "RequestWordTranslation") {
				if (i.second.get<std::string>("","wrong") != "wrong" && uri_context == "/translate") {
						reply_store_PropertyTree(rep, http_receive_RequestWordTranslation(i.second));
				}
				else if (uri_context != "/translate") {
					reply_store_ErrorPt(rep, "Wrong request uri(need: /translate)!");
				} else {
					reply_store_ErrorPt(rep, "Wrong request value!");
				}
			}
			else if (i.first == "RequestCard") {
				if (i.second.get<size_t>("", 0) >= 100 && uri_context == "/requestCard" ) {
				reply_store_PropertyTree(rep, http_receive_RequestCard(i.second));
				}
				else if (uri_context != "/requestCard" && i.second.get<size_t>("", 0) >= 100) {
					reply_store_ErrorPt(rep, "Wrong request uri(need: /requestCard)!");
				} else {
					reply_store_ErrorPt(rep, "Wrong request value!");
				}
			}
			else if (i.first == "RequestChineseNumberRecognize") {
				if (i.second.get<int>("", -1) != -1 && uri_context == "/number") {
				reply_store_PropertyTree(rep, http_receive_RequestChineseNumberRecognize(i.second));
				}
				else if (uri_context != "/number") {
					reply_store_ErrorPt(rep, "Wrong request uri(need: /number)!");
				} else {
					reply_store_ErrorPt(rep, "Wrong request value!");
				}
			}
			else {
				reply_store_ErrorPt(rep, "Wrong request!");
			}
		}

	}
	catch (boost::property_tree::ptree_error)
	{
		SE::WriteToLog("Error in inner HandleReadData: ptree_error exception caught: " + boost::lexical_cast<std::string>(this));
		reply_store_ErrorPt(rep, "Wrong request!");
		return;
	}

}

void request_handler::reply_store_ErrorPt(reply& rep, std::string err_s) {

	// :::::::::::::::::::::::
	boost::property_tree::ptree content;
	content.put("result.error", err_s);

	std::stringstream o_stream;

	boost::property_tree::write_json(o_stream, content);

	std::string data = o_stream.str();

	// :::::::::::::::::::::::
	rep.reply_status = "HTTP/1.0 400 Bad Request";
	rep.reply_content = data;
	rep.headers.resize(2);
	rep.headers[0].name = "Content-Length";
	rep.headers[0].value = std::to_string(rep.reply_content.size());
	rep.headers[1].name = "Content-Type";
	rep.headers[1].value = "application/json";
}

bool request_handler::url_decode(const std::string& in, std::string& out){
	out.clear();
	out.reserve(in.size());
	for (std::size_t i = 0; i < in.size(); ++i)
	{
		if (in[i] == '%')
		{
			if (i + 3 <= in.size())
			{
				int value = 0;
				std::istringstream is(in.substr(i + 1, 2));
				if (is >> std::hex >> value)
				{
					out += static_cast<char>(value);
					i += 2;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else if (in[i] == '+')
		{
			out += ' ';
		}
		else
		{
			out += in[i];
		}
	}
	return true;
}


boost::property_tree::ptree request_handler::http_receive_RequestWordTranslation(boost::property_tree::ptree propertyTree)
{
	try
	{
		SE::WriteToLog("http_recieve_RequestWordTranslation begin");
		std::string word = propertyTree.get<std::string>("");

		std::string wordSymbols;
		for (auto i : word)
		{
			wordSymbols += boost::lexical_cast<std::string>((int)i) + " ";
		}
		SE::WriteToLog("http_recieve_RequestWordTranslation word: " + wordSymbols);
		SE::WriteToLog("http_recieve_RequestWordTranslation word: " + word);

		

		SE::WriteToLog("http_recieve_RequestWordTranslation end");
		return http_send_RequestWordTranslation(word);
	}
	catch (std::exception e)
	{
		SE::WriteToLog("Exception in connection::http_recieve_RequestWordTranslation");
	}
}

boost::property_tree::ptree request_handler::http_send_RequestWordTranslation(std::string wordToTranslate)
{
	boost::property_tree::ptree p;

	std::vector<std::vector<std::string>> p_words; //		||:::::::::::::::::::||
	std::vector<std::vector<std::string>> p_meanings; //	||::New data struct::||
	std::vector<std::string> p_complex_words;//				||:::::::::::::::::::||
	std::vector<std::string> p_complex_meanings;//			||:::::::::::::::::::||
	LH::HangulResult result;

	//boost::trim(wordToTranslate);

	std::wstring wWordToTranslate = SE::string_to_wstring(wordToTranslate);

	boost::trim(wWordToTranslate);

	std::string verboseResult;

	std::set<std::wstring> lessonSet;

	if (wWordToTranslate.size() > 0)
	{

		//std::vector<std::wstring> wWordToTranslateArr;

		//boost::split(wWordToTranslateArr, wWordToTranslate, boost::is_any_of(L" "), boost::token_compress_on);

		bool hangulWord = true;

		for (wchar_t& wc : wWordToTranslate)
		{
			if (!((wc >= 44032 && wc <= 55203) || (wc == L' ')))
			{
				hangulWord = false;
			}
		}

		if (hangulWord)
		{
			//HallyuSocketServer.HangulProcessor.RecognizeAndVerboseWords(wWordToTranslateArr, verboseResult, lessonSet);

			result = luaHelper.ProcessString(wWordToTranslate);

			p_words.resize(result.resultTable.size()); // ::::::::::::::::::::::::::::::
			p_meanings.resize(result.resultTable.size()); // :::::::::::::::::::::::::::

			for (int i = 0; i < result.resultTable.size(); i++)
			{
				p_meanings[i].resize(result.resultTable[i].size()); // :::::::::::::::::::::::::::::

				for (int j = 0; j < result.resultTable[i].size(); j++)
				{
					p_words[i].resize(result.resultTable[i][j].dictStruct.words.size()); // ::::::::::::

					for (int k = 0; k < result.resultTable[i][j].dictStruct.words.size(); k++)
					{
						verboseResult += "\"" + SE::wstring_to_string(result.resultTable[i][j].dictStruct.words[k]) + "\" ";

						p_words[i][k] = SE::wstring_to_string(result.resultTable[i][j].dictStruct.words[k]); // :::::::::::
					}
					p_meanings[i][j] = SE::wstring_to_string(result.resultTable[i][j].verbose);

					verboseResult += "- ";
					verboseResult += SE::wstring_to_string(result.resultTable[i][j].verbose) + "\n";

					for (auto& lesson : result.resultTable[i][j].lessons)
					{
						lessonSet.insert(lesson);
					}

				}
			}

			p_complex_meanings.resize(result.complexVerbResultArr.size());
			for (int i = 0; i < result.complexVerbResultArr.size(); i++)
			{
				verboseResult += SE::wstring_to_string(result.complexVerbResultArr[i].verbose) + "\n";
				
				p_complex_meanings[i] = SE::wstring_to_string(result.complexVerbResultArr[i].verbose);

				for (auto& lesson : result.complexVerbResultArr[i].lessons)
				{
					lessonSet.insert(lesson);
				}
			}

			/*// ==================== TRANSLATE LOG 
			std::ofstream trs;
			trs.open("trans_stream.txt", std::ios::app);
			trs << "===::WORDS::===" << std::endl;
			for (int i = 0; i<p_words.size(); i++) {
				trs << "i[" << i << "] " << std::endl;
				for (int j = 0; j<p_words[i].size(); j++) {
					trs << "-" << p_words[i][j] << std::endl;
				}
			}
			trs << "===::MEANINGS::===" << std::endl;
			for (int i = 0; i<p_meanings.size(); i++) {
				trs << "i[" << i << "]" << std::endl;
				for (int j = 0; j<p_meanings[i].size(); j++) {
					trs << "-" << p_meanings[i][j] << std::endl;
				}
			}

			trs.close();
			// ==================== TRANSLATE LOG */


		}
		else
		{
			verboseResult = "Request contains non-hangul characters";
		}
	}
	else
	{
		verboseResult = "Request is empty";
	}


	SE::WriteToLog("Verbose result size : " + boost::lexical_cast<std::string>(verboseResult.size()));

//#define OLD_VER_PROC
#ifdef OLD_VER_PROC
	//p.put("OnRequestWordTranslation", verboseResult); // For andoid app version 5 or lower

	p.put("OnRequestWordTranslation.Verbose", verboseResult);

	boost::property_tree::ptree lesson_tree;

	for (const std::wstring& lesson : lessonSet)
	{
		lesson_tree.add("Lesson", SE::wstring_to_string(lesson));
	}
	p.add_child("OnRequestWordTranslation.LessonList", lesson_tree);

	return p;
#else
	boost::property_tree::ptree result_data; // priority	 #1
	boost::property_tree::ptree data_element; //			 #2
	boost::property_tree::ptree compound_array; //			 #reserver#
	boost::property_tree::ptree compound_data; //			 #reserved#
	// ====================================
	boost::property_tree::ptree result_element; //			 #3
	boost::property_tree::ptree element_part; //			 #4
	// ====================================
	boost::property_tree::ptree array_data; //				 #5
	boost::property_tree::ptree array_element; //			 #6

	for (int i = 0; i < p_words.size(); i++)
	{
		// :::::::::::::::::::::::::::::::::::::::::::::
		for (int j = 0; j < p_words[i].size(); j++) {
			array_element.put("", p_words[i][j]);
			array_data.push_back(std::make_pair("", array_element));
			array_element.clear();
		}
		element_part.add_child("Words", array_data);
		array_data.clear();

		for (int j = 0; j < p_meanings[i].size(); j++) {
			array_element.put("", p_meanings[i][j]);
			array_data.push_back(std::make_pair("", array_element));
			array_element.clear();
		}
		element_part.add_child("Meanings", array_data);
		array_data.clear();
		// :::::::::::::::::::::::::::::::::::::::::::::
		result_element.push_back(std::make_pair("", element_part));
	}
	data_element.add_child("ResultTable", result_element);

	// ======= clear =======
	array_data.clear();
	array_element.clear();
	element_part.clear();
	result_element.clear();
	// ======= clear =======

	/*..complex verb section..*///////////////////////////////////
	for (int i = 0; i < result.complexVerbResultArr.size(); i++) {
		// ================== main word
		// :::::::::::::::::: dictStruct
		element_part.put("base", SE::wstring_to_string(result.complexVerbResultArr[i].mainWordStruct.dictStruct.base));
		for (int j = 0; j < result.complexVerbResultArr[i].mainWordStruct.dictStruct.words.size(); j++) {
			array_element.put("", SE::wstring_to_string(result.complexVerbResultArr[i].mainWordStruct.dictStruct.words[j]));
			array_data.push_back(std::make_pair("", array_element));
			array_element.clear();
		}
		element_part.add_child("words", array_data);
		result_element.add_child("dictStruct", element_part);
		array_data.clear();
		element_part.clear();
		// :::::::::::::::::: dictStruct
		result_element.put("verbose", SE::wstring_to_string(result.complexVerbResultArr[i].mainWordStruct.verbose));
		for (int j = 0; j < result.complexVerbResultArr[i].mainWordStruct.lessons.size(); j++) {
			array_element.put("", SE::wstring_to_string(result.complexVerbResultArr[i].mainWordStruct.lessons[j]));
			array_data.push_back(std::make_pair("", array_element));
			array_element.clear();
		}
		result_element.add_child("lessons", array_data);
		array_data.clear();

		for (int j = 0; j < result.complexVerbResultArr[i].mainWordStruct.modificators.size();j++) {
			array_element.put("", SE::wstring_to_string(result.complexVerbResultArr[i].mainWordStruct.modificators[j]));
			array_data.push_back(std::make_pair("", array_element));
			array_element.clear();
		}
		result_element.add_child("modificators", array_data);
		compound_data.add_child("mainWordStruct", result_element);
		result_element.clear();
		array_data.clear();
		// ================== main word
		// ----------------------------
		// ================== second word
		element_part.put("base", SE::wstring_to_string(result.complexVerbResultArr[i].secondaryWordStruct.dictStruct.base));
		for (int j = 0; j < result.complexVerbResultArr[i].secondaryWordStruct.dictStruct.words.size(); j++) {
			array_element.put("", SE::wstring_to_string(result.complexVerbResultArr[i].secondaryWordStruct.dictStruct.words[j]));
			array_data.push_back(std::make_pair("", array_element));
			array_element.clear();
		}
		element_part.add_child("words", array_data);
		result_element.add_child("dictStruct", element_part);
		array_data.clear();
		element_part.clear();
		// :::::::::::::::::: dictStruct
		result_element.put("verbose", SE::wstring_to_string(result.complexVerbResultArr[i].secondaryWordStruct.verbose));
		for (int j = 0; j < result.complexVerbResultArr[i].secondaryWordStruct.lessons.size(); j++) {
			array_element.put("", SE::wstring_to_string(result.complexVerbResultArr[i].secondaryWordStruct.lessons[j]));
			array_data.push_back(std::make_pair("", array_element));
			array_element.clear();
		}
		result_element.add_child("lessons", array_data);
		array_data.clear();

		for (int j = 0; j < result.complexVerbResultArr[i].secondaryWordStruct.modificators.size(); j++) {
			array_element.put("", SE::wstring_to_string(result.complexVerbResultArr[i].secondaryWordStruct.modificators[j]));
			array_data.push_back(std::make_pair("", array_element));
			array_element.clear();
		}
		result_element.add_child("modificators", array_data);
		compound_data.add_child("secondaryWordStruct", result_element);
		result_element.clear();
		array_data.clear();
		// ================== second word
		compound_array.push_back(std::make_pair("", compound_data));
		compound_data.clear();
	}
	data_element.add_child("ComplexVerbResultArr", compound_array);
	compound_array.clear();
	/*..complex verb section..*///////////////////////////////////

	// ======= clear =======
	array_data.clear();
	array_element.clear();
	element_part.clear();
	result_element.clear();
	// ======= clear =======

	for (const std::wstring& lesson : lessonSet)
	{
		array_element.put("", SE::wstring_to_string(lesson));
		array_data.push_back(std::make_pair("", array_element));
		array_element.clear();
	}
	data_element.add_child("LessonList", array_data);

	/*..last data process..*/
	result_data.add_child("OnRequestWordTranslation", data_element);

	return result_data;
	
#endif
}

boost::property_tree::ptree request_handler::http_receive_RequestCard(boost::property_tree::ptree propertyTree)
{
	try
	{
		SE::WriteToLog("OnReceive_RequestCard begin");
		size_t card = propertyTree.get<size_t>(""); 
		
		

		SE::WriteToLog("OnReceive_RequestCard: " + boost::lexical_cast<std::string>(card));
		
		return http_send_OnRequestCard(card);
	}
	catch (std::exception e)
	{
		SE::WriteToLog("Exception in TUser::OnReceive_RequestCard");
	}
}

boost::property_tree::ptree request_handler::http_send_OnRequestCard(size_t wordCount)
{
	
	//Xperimental
	boost::property_tree::ptree p;
	
	std::wstring translation;
	std::wstring word0;
	std::wstring word1;
	std::wstring word2;
	std::wstring word3;
	
	size_t correctAnswer;
	
	luaHelper.HangulDictionary.GetRandomWord(wordCount, translation, word0, word1, word2, word3, correctAnswer);
	
	p.put("OnRequestCard.Translation", SE::wstring_to_string(translation));
	p.put("OnRequestCard.Word0", SE::wstring_to_string(word0));
	p.put("OnRequestCard.Word1", SE::wstring_to_string(word1));
	p.put("OnRequestCard.Word2", SE::wstring_to_string(word2));
	p.put("OnRequestCard.Word3", SE::wstring_to_string(word3));
	p.put("OnRequestCard.CorrectWord", correctAnswer);
	
	return p;
}

boost::property_tree::ptree request_handler::http_receive_RequestChineseNumberRecognize(boost::property_tree::ptree propertyTree)
{
	try
	{
		SE::WriteToLog("OnReceive_RequestChineseNumberRecognize begin");
		int number = propertyTree.get<int>(""); 

		SE::WriteToLog("OnReceive_RequestChineseNumberRecognize: " + boost::lexical_cast<std::string>(number));
		return http_send_OnRequestChineseNumberRecognize(number);
	}
	catch (std::exception e)
	{
		SE::WriteToLog("Exception in TUser::OnReceive_RequestChineseNumberRecognize");
	}
}

boost::property_tree::ptree request_handler::http_send_OnRequestChineseNumberRecognize(int maxDigits)
{

	std::wstring numberStr;
	unsigned long long number;

	luaHelper.HangulDictionary.GetRandomChineseNumber(maxDigits, number, numberStr);

	boost::property_tree::ptree p;

	p.put("OnRequestChineseNumberRecognize.number", number);
	p.put("OnRequestChineseNumberRecognize.numberStr", SE::wstring_to_string(numberStr));


	return p;
}

void request_handler::reply_store_PropertyTree(reply& rep, boost::property_tree::ptree pTree)
{
	//Xperimental -- need to optimize this
	try
	{
		std::stringstream o_stream;

		boost::property_tree::write_json(o_stream, pTree);

		std::string data = o_stream.str();
		int dataLength = data.size();
		if (dataLength == 0) {
			reply_store_ErrorPt(rep, "(something goes wrong)Empty reply!");
		} else
			rep.reply_content = data;
			rep.reply_status = "HTTP/1.1 200 OK";
			rep.headers.resize(2);
			rep.headers[0].name = "Content-Length";
			rep.headers[0].value = std::to_string(rep.reply_content.size());
			rep.headers[1].name = "Content-Type";
			rep.headers[1].value = "application/json";
	}
	catch (std::exception e)
	{
		SE::WriteToLog("Error in SendPropertyTree");
	}
}

}
}