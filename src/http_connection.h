#ifndef _HTTP_CONNECTION_H_
#define _HTTP_CONNECTION_H_

#include <memory>
#include <boost/asio.hpp>
#include "http_connection_manager.h"
#include "http_request_handler.h"
#include "http_request_parser.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "LuaHelper.h"
#include "http_server.h"
#include "SE/Network.h"

namespace http {
namespace server {

	class connection_manager;

	class connection : public std::enable_shared_from_this<connection>
	{
	public:
		connection(const connection&) = delete;
		connection& operator=(const connection&) = delete;

		explicit connection(boost::asio::ip::tcp::socket isocket, connection_manager& manager, request_handler& handler, THallyuHttpServer& server);


		// Init list
		THallyuHttpServer& HallyuHttpServer;
		SE::TDataReadSignalMap DataReadSignalMap;
		int DataSize;
		std::vector<char> Data;

		// access methods
		void start();
		void stop();

		void BeforeStart();

		// Request/Reply methods
		void OnReceive_RequestWordTranslation(boost::property_tree::ptree propertyTree);
		void Send_OnRequestWordTranslation(std::string wordToTranslate);

		// Other methods
		void SendPropertyTree(boost::property_tree::ptree pTree);
		void HandleReadDataSize(const boost::system::error_code& error);
		void HandleReadData(const boost::system::error_code& error);

	private:
		// ------ read/write ------- asynchronous
		void do_read();
		void do_write();
		// ====== read/write ======= asynchronous

		std::string Address;

		std::array<char, 8192> connection_buffer;
	
		boost::asio::ip::tcp::socket http_socket;
		connection_manager& http_connection_manager;
		request_handler& http_request_handler;

		request_parser request_parser_;
		request http_request;
		reply http_reply;
	};

	typedef std::shared_ptr<connection> connection_ptr;
}
}

#endif
