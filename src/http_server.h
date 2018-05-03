#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_

#include "boost/asio.hpp"
#include "LuaHelper.h"
#include "http_connection_manager.h"
#include "http_request_handler.h"

namespace http {
namespace server {

class THallyuHttpServer : public SE::TServerSocket
{
public:
	THallyuHttpServer(/*const std::string& address, */const::std::string& port, const::std::string& root_dir, LH::LuaHelper& iluaHelper);

	THallyuHttpServer(const THallyuHttpServer&) = delete;
	THallyuHttpServer& operator=(const THallyuHttpServer&) = delete;

	LH::LuaHelper& luaHelper;

	void run(); // run io_context loop
private:

	void do_accept();
	void do_await_stop();

	//boost::asio::ip::tcp::acceptor acceptor_;
	//boost::asio::io_context io_context_;
	boost::asio::signal_set signals_;

	// handlers
	connection_manager connection_manager_;
	request_handler request_handler_;
};

}
}
#endif
