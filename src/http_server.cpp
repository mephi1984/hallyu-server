#include "http_server.h"

namespace http {
namespace server {

THallyuHttpServer::THallyuHttpServer(const std::string& address, const std::string& port, const std::string& root_dir, LH::LuaHelper& iluaHelper) 
	: SE::TServerSocket(std::stoi(port))
	, luaHelper(iluaHelper)
	, io_context(1)
	, signals(io_context)
	, acceptor(io_context)
{
	/*.....*/
}

}
}