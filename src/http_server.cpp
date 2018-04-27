#include "http_server.h"

namespace http {
namespace server {

THallyuHttpServer::THallyuHttpServer(const std::string& address, const std::string& port, const std::string& root_dir, LH::LuaHelper& iluaHelper) 
	: SE::TServerSocket(std::stoi(port))
	, luaHelper(iluaHelper)
	, io_context_(1)
	, signals_(io_context_)
	, acceptor_(io_context_)
	, connection_manager_()
	, request_handler_(root_dir)
{
	
	// -------- Signals section --------
	signals_.add(SIGINT);
	signals_.add(SIGTERM);

	do_await_stop();
	// ======== Signals section ========

	// Open the acceptor
	// #from#
	 
	do_accept();
}

void THallyuHttpServer::do_accept() {
	acceptor_.async_accept(
		[this](boost::system::error_code ec, boost::asio::ip::tcp::socket isocket){
			
			if(!acceptor_.is_open()) {
				return;
			}

			if (!ec) {
				connection_manager_.start(std::make_shared<connection> (std::move(isocket), connection_manager_, request_handler_));
			}

			do_accept();
	}
	);
}

void THallyuHttpServer::do_await_stop() {
	signals_.async_wait(
	[this](boost::system::error_code ec, int signo) {

		acceptor_.close();
		connection_manager_.stop_all();

		std::cout << "Connection stopped with error code:: " << ec << std::endl;
		std::cout << "Signo:: " << signo << std::endl;
		system("pause");
	}
	);
}

void THallyuHttpServer::run() {
	io_context_.run();
}

}
}