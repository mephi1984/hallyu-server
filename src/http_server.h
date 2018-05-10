#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
/*
//#include <boost/asio.hpp>
#include "LuaHelper.h"
#include "http_connection_manager.h"
#include "http_request_handler.h"
#include "SE/Server.h"
*/

#include <string>
#include "SE/Server.h"
#include "LuaHelper.h"

#include "http_connection.h" // !!!

#include "http_connection_manager.h"
#include "http_request_handler.h"

namespace http {
namespace server {

class THallyuHttpServer
{
public:
	THallyuHttpServer(/*const std::string& address, */int port/*, const::std::string& root_dir*/, LH::LuaHelper& iluaHelper);

	THallyuHttpServer(const THallyuHttpServer&) = delete;
	THallyuHttpServer& operator=(const THallyuHttpServer&) = delete;

	LH::LuaHelper& luaHelper;

	void run(); // run io_context loop

	void UpdateInThread(); // new
	void JoinServiceThread(); // new
	boost::asio::io_service IoService; // new
protected:
	boost::asio::ip::tcp::resolver resolver; // new

	boost::asio::ip::tcp::endpoint endpoint; // new

	boost::asio::ip::tcp::acceptor acceptor; // new

	boost::thread ServiceThread; // new
private:

	void do_accept();
	void do_await_stop();

	boost::asio::signal_set signals_;

	// handlers
	connection_manager connection_manager_;
	request_handler request_handler_;
};

}
}
#endif
