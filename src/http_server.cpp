#include "http_server.h"
#include "socket_server.h"

namespace http {
namespace server {

THallyuHttpServer::THallyuHttpServer(/*const std::string& address, */const std::string& port, const std::string& root_dir, LH::LuaHelper& iluaHelper) 
	: SE::TServerSocket(std::stoi(port))
	, luaHelper(iluaHelper)
	//, io_context_(1)
	, signals_(IoService)
	//, acceptor_(io_context_)
	, connection_manager_()
	, request_handler_(root_dir)
{
	
	// -------- Signals section --------
	signals_.add(SIGINT);
	signals_.add(SIGTERM);

	do_await_stop();
	// ======== Signals section ========

	
	do_accept();

	ServiceThread = boost::thread(std::bind(&TServerSocket::UpdateInThread, this));

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
				return;
			}

			if (!ec) {
				connection_manager_.start(std::make_shared<connection> (std::move(isocket), connection_manager_, request_handler_));
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