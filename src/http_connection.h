#ifndef HTTP_CONNECTION_H
#define HTTP_CONNECTION_H
/*
#include <memory>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include "http_request_handler.h"
#include "http_request_parser.h"
#include "SE/Network.h"
//#include "http_connection_manager.h"
#include "http_server.h"
*/

#include <memory>
#include <set>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "http_request.h"
#include "http_reply.h"
#include "http_request_parser.h"
#include "http_connection_manager.h"

#include "SE/Network.h"
#include "SE/Server.h"

namespace http {
namespace server {

	class request_handler;
	class THallyuHttpServer;

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

		void before_start();

		// Request/Reply methods
		void http_recieve_RequestWordTranslation(boost::property_tree::ptree propertyTree);
		void http_send_RequestWordTranslation(std::string wordToTranslate);

		// Other methods
		void http_send_PropertyTree(boost::property_tree::ptree pTree);
		void handle_http_request(request& req);
		
	private:
		// ------ read/write ------- asynchronous
		void do_read();
		void do_write();
		// ====== read/write ======= asynchronous

		std::string Address;

		//std::vector<char> connection_buffer;
		std::array<char, 8192> connection_buffer;
	
		boost::asio::ip::tcp::socket http_socket;
		connection_manager& http_connection_manager;
		request_handler& http_request_handler;

		request_parser http_request_parser;
		request http_request;
		reply http_reply;
	};

	//typedef std::shared_ptr<connection> connection_ptr;
}
}

#endif
