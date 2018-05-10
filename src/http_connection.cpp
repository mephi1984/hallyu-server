
#include "http_server.h"

#include "http_connection.h"
#include "LuaHelper.h"
#include "SE/misc.h"

#include <clocale>

namespace http {
namespace server {
	
	connection::connection(boost::asio::ip::tcp::socket isocket, connection_manager& manager, request_handler& handler, THallyuHttpServer& server)
		: HallyuHttpServer(server)
		, http_socket(std::move(isocket))
		, http_connection_manager(manager)
		, http_request_handler(handler)
	{
		std::cout << "http_connection constructor" << std::endl;
	}

	void connection::before_start() {
		Address = http_socket.lowest_layer().remote_endpoint().address().to_string();
		std::cout << "http_connection before_start " << Address <<  std::endl;
		DataReadSignalMap.AddSlot("RequestWordTranslation", std::bind(&connection::http_recieve_RequestWordTranslation, this, std::placeholders::_1));
	}

	void connection::start() {
		do_read();
	}

	void connection::do_read() {
		
		auto self(shared_from_this());
		http_socket.async_read_some(boost::asio::buffer(connection_buffer),
			[this,self](boost::system::error_code ec, std::size_t bytes_transfered) {
				if(!ec) {
					std::cout << "http_connection bytes_transfered:: " << bytes_transfered << std::endl;
					request_parser::result_type result;
/*parse*/			std::tie(result, std::ignore) = http_request_parser.parse(http_request, connection_buffer.data(), connection_buffer.data() + bytes_transfered);

					//std::cout << "http_connection do_read result:: " << result << std::endl;

					if (result == request_parser::good)
					{
						std::cout << "http_connection request handle " << std::endl;
						// Signals emitting
						handle_http_request(http_request);
						
						// http_request_handler.handle_request(http_request, http_reply); // read headers and content to reply
						//do_write(); // write content to socket (close socket if error_code)
					}
					else if (result == request_parser::bad)
					{
						//http_reply = reply::stock_reply(reply::bad_request);
						//do_write();
						http_connection_manager.stop(shared_from_this());
					}
					else
					{
						do_read();
					}
				} else if (ec != boost::asio::error::operation_aborted) {
					http_connection_manager.stop(shared_from_this());
				}
		});
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

	void connection::http_recieve_RequestWordTranslation(boost::property_tree::ptree propertyTree)
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

			http_send_RequestWordTranslation(word);

			SE::WriteToLog("http_recieve_RequestWordTranslation end");
		}
		catch (std::exception e)
		{
			SE::WriteToLog("Exception in connection::http_recieve_RequestWordTranslation");
		}
	}

	void connection::http_send_RequestWordTranslation(std::string wordToTranslate)
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

		http_send_PropertyTree(p);
	}

	void connection::http_send_PropertyTree(boost::property_tree::ptree pTree)
	{
		//Xperimental -- need to optimize this
		try
		{
			std::cout << "sending ptree " << std::endl;
			std::stringstream o_stream;

			boost::property_tree::write_json(o_stream, pTree);

			std::string data = o_stream.str();
			int dLen = data.size();


			const char http_rep_data[] = "HTTP/1.0 400 Bad Request\r\n";
			const char crlf[] = {'\r','\n'};
			//const char httpContent[] = "hello";
			int len = 0;

			//boost::shared_array<char> dataToSend(new char[len]); // [len +4]

			//memcpy(&dataToSend[0], &len, 4);

			//memcpy(&dataToSend[4], &data[0], len);
			
			//memcpy(&dataToSend[0], &http_rep_data[0], len);

			/*
			http_headers = "HTTP/1.0 200 OK\n";
			http_headers += "Content-Length: ";
			http_headers += std::to_string(dLen);
			http_headers += "\n";
			http_headers += "Content-Type: text/plain";
			http_headers+= "\r\n"
			*/
			//std::vector<const char> stringDataToSend;
			//stringDataToSend.push_back('h');
			//stringDataToSend.push_back('e');

			//stringDataToSend.push_back("HTTP/1.0 200 OK\n");
			//stringDataToSend.push_back("Content-Length");
			//stringDataToSend.push_back(": ");
			//stringDataToSend.push_back(std::to_string(dLen));
			//stringDataToSend.push_back("\r\n");
			//stringDataToSend.push_back(data);
			/*
			std::vector<boost::asio::const_buffer> rep_buf;
			rep_buf.push_back(boost::asio::buffer("HTTP/1.0 200 OK\n"));
			rep_buf.push_back(boost::asio::buffer("Content-Length"));
			rep_buf.push_back(boost::asio::buffer(": "));
			rep_buf.push_back(boost::asio::buffer(std::to_string(dLen)));
			rep_buf.push_back(boost::asio::buffer("\r\n"));
			rep_buf.push_back(boost::asio::buffer("\r\n"));
			rep_buf.push_back(boost::asio::buffer(data));
			*/

			auto sharedThis = shared_from_this(); // to keep connection
			
			char* c_data = new char[data.length() + 1];
			strcpy(c_data, data.c_str());

			std::vector<boost::asio::const_buffer> rep_buf;
			rep_buf.push_back(boost::asio::buffer("HTTP/1.1 200 OK\n"));
			rep_buf.push_back(boost::asio::buffer("Content-Type: application/json\r\n"));
			rep_buf.push_back(boost::asio::buffer(&c_data[0], sizeof(c_data)));

			boost::asio::async_write(http_socket, rep_buf /*boost::asio::buffer(&dataToSend[0], len /*len + 4)*/,
				[rep_buf, this, sharedThis](boost::system::error_code ec, std::size_t bytes_transfered)
			{
				std::cout << "async_write bytes_transferes:: " << bytes_transfered << std::endl;
				std::ofstream tstTrans;
				tstTrans.open("transTest.txt", std::ios::app);
				for (size_t itr=0;itr<rep_buf.size();itr++) {
					tstTrans << boost::asio::buffer_cast<const char*>(rep_buf[itr]);
				}
				tstTrans.close();

				if (ec)
				{
					SE::WriteToLog("Error in inner SendPropertyTree");
				}
				http_connection_manager.stop(sharedThis); // close connection
			}
			);
		}
		catch (std::exception e)
		{
			SE::WriteToLog("Error in SendPropertyTree");
		}
	}

	/*
	std::vector<boost::asio::const_buffer> connection::to_bffers(std::string data, int dLen) {
		
		std::vector<std::string> stringDataToSend;
		stringDataToSend.push_back("HTTP/1.0 200 OK\n");
		stringDataToSend.push_back("Content-Length");
		stringDataToSend.push_back(": ");
		stringDataToSend.push_back(std::to_string(dLen));
		stringDataToSend.push_back("\r\n");
		stringDataToSend.push_back(data);

		std::vector<boost::asio::const_buffer> rep_buf;
		for (size_t sdbi = 0; sdbi < stringDataToSend.size(); sdbi++) {
			rep_buf.push_back(boost::asio::buffer(stringDataToSend[sdbi]));
		}
		return rep_buf;
	}
	*/
	void connection::handle_http_request(request& req) {
		try 
		{
			//Xperimental - Might be optimized a lot:
			std::cout << "handle_http_req start " << "||| req content size:: " << req.request_content.size() << " |||" << std::endl;
			if (req.request_content.size() == 0) {
				SE::WriteToLog("Request content is empty");
				http_connection_manager.stop(shared_from_this()); // close connection
				return;
			}

			std::string jsonCode = std::string(&req.request_content[0], &req.request_content[0] + req.request_content.size());

			std::stringstream stream(jsonCode);

			boost::property_tree::ptree propertyTree;
			boost::property_tree::read_json(stream, propertyTree);

			bool nextIsBinary = false;
			boost::property_tree::ptree binaryTree;

			BOOST_FOREACH(auto i, propertyTree)
			{
				if (i.first == "_binary")
				{
					binaryTree = i.second;
					nextIsBinary = true;

				}
				else
					if (DataReadSignalMap.SignalExists(i.first))
					{
						DataReadSignalMap.EmitSignal(i.first, i.second); // All methods need to close connection after reply is sended
						//http_recieve_RequestWordTranslation(i.second);
					} else {
						std::cout << "No signal exists" << std::endl;
						http_connection_manager.stop(shared_from_this()); // close connection
					}
			}

		}
		catch (boost::property_tree::ptree_error)
		{
			SE::WriteToLog("Error in inner HandleReadData: ptree_error exception caught: " + boost::lexical_cast<std::string>(this));
		}

	}

}
}
