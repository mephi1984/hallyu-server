/*
#include "http_server.h"
#include "socket_server.h"
*/

#include "http_server.h"

namespace http {
namespace server {

THallyuHttpServer::THallyuHttpServer(/*const std::string& address, */int port/*, const std::string& root_dir*/, LH::LuaHelper& iluaHelper) 
	: SE::TServerSocket(port)
	, luaHelper(iluaHelper)
	//, io_context_(1)
	, signals_(IoService)
	//, acceptor_(io_context_)
	, connection_manager_()
	, request_handler_(/*root_dir*/)
{
	std::cout << "http_Server_constructor start" << std::endl;
	// -------- Signals section --------
	signals_.add(SIGINT);
	signals_.add(SIGTERM);
	std::cout << "http_Server_constructor signals" << std::endl;
	do_await_stop();
	// ======== Signals section ========

	std::cout << "http_Server_constructor await to stop" << std::endl;
	do_accept();
	std::cout << "http_Server_constructor in_acception" << std::endl;
	ServiceThread = boost::thread(std::bind(&TServerSocket::UpdateInThread, this));
	std::cout << "http_Server_constructor threaded" << std::endl;
	// Open the acceptor
	// -----------------
	/*
	boost::asio::ip::tcp::resolver resolver(io_context_);
	boost::asio::ip::tcp::endpoint endpoint =
		*resolver.resolve(address, port).begin();
	acceptor_.open(endpoint.protocol());
	acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	acceptor_.bind(endpoint);
	acceptor_.listen();
	 
	do_accept();
	*/
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
				
				std::cout << "acception" << std::endl;

				/*
				std::shared_ptr<TUser> user = std::shared_ptr<TUser>(new TUser(IoService, std::move(socket), *this));

				user->BeforeStartRead();

				user->StartRead();
				*/
			}
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
	//io_context_.run();
	IoService.run();
}

}
}