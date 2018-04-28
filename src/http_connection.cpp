#include "http_connection.h"

namespace http {
namespace server {
	
	connection::connection(boost::asio::ip::tcp::socket isocket, connection_manager& manager, request_handler& handler, THallyuHttpServer& server)
		: http_socket(std::move(isocket))
		, http_connection_manager(manager)
		, http_request_handler(handler)
		, HallyuHttpServer(server)
	{
	}

	void connection::BeforeStart() {
		Address = http_socket.lowest_layer().remote_endpoint().address().to_string();

		DataReadSignalMap.AddSlot("RequestWordTranslation", std::bind(&connection::OnReceive_RequestWordTranslation, this, std::placeholders::_1));
	}

	void connection::start() {
		do_read();
	}

	void connection::do_read() {

		SE::ErrorToLog("Data reader start read: " + boost::lexical_cast<std::string>(this));

#define _TMP_DEFINE_1_ // tmp, for method-assign time
#ifdef _TMP_DEFINE_1_

		//boost::asio::async_read(http_socket, boost::asio::buffer(&DataSize, 4), std::bind(&connection::HandleReadDataSize, shared_from_this(), std::placeholders::_1));
		// #from#

#else // Default request parsing
		auto self(shared_from_this());
		http_socket.async_read_some(boost::asio::buffer(connection_buffer),
			[this,self](boost::system::error_code ec, std::size_t bytes_transfered) {
				if(!ec) {
					request_parser::result_type result;
					std::tie(result, std::ignore) = request_parser_.parse(http_request, connection_buffer.data(), connection_buffer.data() + bytes_transfered);
					if (result == request_parser::good)
					{
						http_request_handler.handle_request(http_request, http_reply);
						do_write();
					}
					else if (result == request_parser::bad)
					{
						http_reply = reply::stock_reply(reply::bad_request);
						do_write();
					}
					else
					{
						do_read();
					}
				} else if (ec != boost::asio::error::operation_aborted) {
					http_connection_manager.stop(shared_from_this());
				}
		});
#endif // TMP_DEFINE_1
	}

	void connection::do_write() {
		auto self(shared_from_this());
		boost::asio::async_write(http_socket,http_reply.to_buffers(),
			[this,self](boost::system::error_code ec, std::size_t)
		{
			if(!ec) {
				boost::system::error_code ignored_ec;
				http_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec); // <== shutting down socket after handle
			}

			if (ec != boost::asio::error::operation_aborted) {
				http_connection_manager.stop(shared_from_this());
			}
		});
	}

	void connection::stop() {
		http_socket.close();
	}

	void connection::OnReceive_RequestWordTranslation(boost::property_tree::ptree propertyTree)
	{
		try
		{
			SE::WriteToLog("OnReceive_RequestWordTranslation begin");
			std::string word = propertyTree.get<std::string>("");

			std::string wordSymbols;
			for (auto i : word)
			{
				wordSymbols += boost::lexical_cast<std::string>((int)i) + " ";
			}
			SE::WriteToLog("OnReceive_RequestWordTranslation word: " + wordSymbols);
			SE::WriteToLog("OnReceive_RequestWordTranslation word: " + word);

			Send_OnRequestWordTranslation(word);

			SE::WriteToLog("OnReceive_RequestWordTranslation end");
		}
		catch (std::exception e)
		{
			SE::WriteToLog("Exception in TUser::OnReceive_RequestWordTranslation");
		}
	}

	void connection::Send_OnRequestWordTranslation(std::string wordToTranslate)
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



				LH::HangulResult result = HallyuHttpServer.luaHelper.ProcessString(wWordToTranslate);

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

		SendPropertyTree(p);
	}

	void connection::SendPropertyTree(boost::property_tree::ptree pTree)
	{
		//Xperimental -- need to optimize this
		try
		{

			std::stringstream o_stream;

			boost::property_tree::write_xml(o_stream, pTree);

			std::string data = o_stream.str();

			int len = data.size();

			boost::shared_array<char> dataToSend(new char[len + 4]);

			memcpy(&dataToSend[0], &len, 4);

			memcpy(&dataToSend[4], &data[0], len);

			auto sharedThis = shared_from_this(); // to keep connection


			boost::asio::async_write(http_socket, boost::asio::buffer(&dataToSend[0], len + 4),
				[dataToSend, sharedThis](boost::system::error_code ec, std::size_t)
			{
				if (ec)
				{
					SE::WriteToLog("Error in inner SendPropertyTree");
				}
			}
			);
		}
		catch (std::exception e)
		{
			SE::WriteToLog("Error in SendPropertyTree");
		}
	}

	void connection::HandleReadDataSize(const boost::system::error_code& error)
	{
		if (error)
		{
			SE::WriteToLog("Error in inner HandleReadDataSize: " + boost::lexical_cast<std::string>(this));
			return;
		}


		if (DataSize > 65536 || DataSize <= 0)
		{
			// len>65536 is something unbelievable. Prevent this just in case;
			SE::WriteToLog("Error in inner HandleReadDataSize, DataSize is too long or too small: " + boost::lexical_cast<std::string>(this));
			return;
		}

		Data.resize(DataSize);

		boost::asio::async_read(http_socket, boost::asio::buffer(Data), std::bind(&connection::HandleReadData, shared_from_this(), std::placeholders::_1));

	}

}
}
