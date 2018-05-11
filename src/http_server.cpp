/*
#include "http_server.h"
#include "socket_server.h"
*/

#include "http_server.h"

namespace http {
namespace server {

THallyuHttpServer::THallyuHttpServer(/*const std::string& address, */int port/*, const std::string& root_dir*/, LH::LuaHelper& iluaHelper) 
	: resolver(IoService)
	, luaHelper(iluaHelper)
	, acceptor(IoService)
	, signals_(IoService)
	, connection_manager_()
	, request_handler_(/*root_dir*/)
{

	std::string addr("127.0.0.1");
	endpoint = *resolver.resolve(addr, std::to_string(port)).begin();
	acceptor.open(endpoint.protocol());
	acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	acceptor.bind(endpoint);
	acceptor.listen();

	// -------- Signals section --------
	signals_.add(SIGINT);
	signals_.add(SIGTERM);
	do_await_stop();
	// ======== Signals section ========
	do_accept();
	std::cout << "==::Acception::==" << std::endl;
	
	ServiceThread = boost::thread(std::bind(&THallyuHttpServer::UpdateInThread, this));
	
}

void THallyuHttpServer::do_accept() {
	acceptor.async_accept(
		[this](boost::system::error_code ec, boost::asio::ip::tcp::socket isocket){
			
			if(!acceptor.is_open()) {
				std::cout << "http_Server_acception open_error" << std::endl;
				return;
			}

			if (!ec) {
				connection_manager_.start(std::make_shared<connection>(std::move(isocket), connection_manager_, request_handler_, *this));
				std::cout << "--:connected:--" << std::endl;
			}
			std::cout << " connections COUNT:: " << connection_manager_.get_connections_count() << std::endl;
			do_accept();
	}
	);
}

void THallyuHttpServer::do_await_stop() {
	signals_.async_wait(
	[this](boost::system::error_code ec, int signo) {

		acceptor.close();
		connection_manager_.stop_all();

		std::cout << "Connection stopped with error code:: " << ec << std::endl;
		std::cout << "Signo:: " << signo << std::endl;
		system("pause");
	}
	);
}

void THallyuHttpServer::run() {
	IoService.run();
}

void THallyuHttpServer::JoinServiceThread()
{
	ServiceThread.join();
}

void THallyuHttpServer::UpdateInThread()
{
	try
	{
		IoService.run();
		IoService.reset();
	}
	catch (std::exception e)
	{
		SE::WriteToLog("Error in TServerSocket::UpdateInThread");
	}
}

}
}