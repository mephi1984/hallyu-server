

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
		std::cout << "handle_http_req start " << "||| req content size:: " << req.request_content.size() << " |||" << std::endl;
		if (req.request_content.size() == 0) {
			SE::WriteToLog("Request content is empty");
		}

		std::string jsonCode = std::string(&req.request_content[0], &req.request_content[0] + req.request_content.size());

		std::stringstream stream(jsonCode);

		boost::property_tree::ptree propertyTree;
		boost::property_tree::read_json(stream, propertyTree);


		BOOST_FOREACH(auto i, propertyTree)
		{

			if (i.first == "RequestWordTranslation") {
				reply_store_PropertyTree(rep, http_receive_RequestWordTranslation(i.second)); // write to reply buffer
			}
			if (i.first == "RequestCard") {
				reply_store_PropertyTree(rep, http_receive_RequestCard(i.second));
			}
			if (i.first == "RequestChineseNumberRecognize") {
				reply_store_PropertyTree(rep, http_receive_RequestChineseNumberRecognize(i.second));
			}

		}

	}
	catch (boost::property_tree::ptree_error)
	{
		SE::WriteToLog("Error in inner HandleReadData: ptree_error exception caught: " + boost::lexical_cast<std::string>(this));
	}

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

			LH::HangulResult result = luaHelper.ProcessString(wWordToTranslate);

			for (int i = 0; i < result.resultTable.size(); i++)
			{
				for (int j = 0; j < result.resultTable[i].size(); j++)
				{
					for (int k = 0; k < result.resultTable[i][j].dictStruct.words.size(); k++)
					{
						verboseResult += "\"" + SE::wstring_to_string(result.resultTable[i][j].dictStruct.words[k]) + "\" ";
					}

					verboseResult += "- ";

					verboseResult += SE::wstring_to_string(result.resultTable[i][j].verbose) + "\n";

					for (auto& lesson : result.resultTable[i][j].lessons)
					{
						lessonSet.insert(lesson);
					}

				}
			}

			for (int i = 0; i < result.complexVerbResultArr.size(); i++)
			{
				verboseResult += SE::wstring_to_string(result.complexVerbResultArr[i].verbose) + "\n";

				for (auto& lesson : result.complexVerbResultArr[i].lessons)
				{
					lessonSet.insert(lesson);
				}
			}

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


	//p.put("OnRequestWordTranslation", verboseResult); // For andoid app version 5 or lower

	p.put("OnRequestWordTranslation.Verbose", verboseResult);

	boost::property_tree::ptree lesson_tree;

	for (const std::wstring& lesson : lessonSet)
	{
		lesson_tree.add("Lesson", SE::wstring_to_string(lesson));
	}
	p.add_child("OnRequestWordTranslation.LessonList", lesson_tree);

	return p;
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
			rep.reply_content = "Error:: Empty data";
			rep.headers.resize(2);
			rep.headers[0].name = "Content-Length";
			rep.headers[0].value = "0";
			rep.headers[1].name = "Content-Type";
			rep.headers[1].value = "text/plain";
		} else
			rep.reply_content = data;
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