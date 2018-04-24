#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_

#include "boost/asio.hpp"
#include "SE/Server.h"
#include "LuaHelper.h"

namespace http {
namespace server {

class THallyuHttpServer : public SE::TServerSocket
{
public:
	THallyuHttpServer(const std::string& address, const::std::string& port, const::std::string& root_dir, LH::LuaHelper& iluaHelper);
	~THallyuHttpServer();

	THallyuHttpServer(const THallyuHttpServer&) = delete;
	THallyuHttpServer& operator=(const THallyuHttpServer&) = delete;

	LH::LuaHelper& luaHelper;

	boost::asio::ip::tcp::acceptor acceptor;
	boost::asio::io_context io_context;
	boost::asio::signal_set signals;
private:

};

}
}
#endif
