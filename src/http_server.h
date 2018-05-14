#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <string>
#include "SE/Server.h"
#include "LuaHelper.h"

#include "http_connection.h"

#include "http_connection_manager.h"
#include "http_request_handler.h"

namespace http {
namespace server {

class THallyuHttpServer
{
public:
	THallyuHttpServer(int port, LH::LuaHelper& iluaHelper);

	THallyuHttpServer(const THallyuHttpServer&) = delete;
	THallyuHttpServer& operator=(const THallyuHttpServer&) = delete;

	LH::LuaHelper& luaHelper;

	void run(); // run io_context loop

	void UpdateInThread(); 
	void JoinServiceThread(); 
	boost::asio::io_service IoService; 
protected:
	boost::asio::ip::tcp::resolver resolver; 

	boost::asio::ip::tcp::endpoint endpoint; 

	boost::asio::ip::tcp::acceptor acceptor; 

	boost::thread ServiceThread; 
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
