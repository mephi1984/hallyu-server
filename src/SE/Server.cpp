#include "Network.h"
#include "Server.h"
#include "boost/random.hpp"
#include "../SE/misc.h"
#include "../socket_server.h"

namespace SE
{
	
	/*
	TServerSocket::TServerSocket(int port)
		: endpoint(boost::asio::ip::tcp::v4(), port)
		, acceptor(IoService, endpoint)
	{
		SE::WriteToLog("TServerSocket::TServerSocket");

	}
	*/
	
	TServerSocket::TServerSocket(int port)
		: resolver(IoService)
		, acceptor(IoService)
	{
		std::string addr("127.0.0.1");
		endpoint = *resolver.resolve(addr, std::to_string(port)).begin();
		acceptor.open(endpoint.protocol());
		acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		acceptor.bind(endpoint);
		acceptor.listen();

		SE::WriteToLog("TServerSocket::TServerSocket acception started");
	}
	

	TServerSocket::~TServerSocket()
	{
	}

	std::string TServerSocket::getPassword()
	{
		return "";
	}


	void TServerSocket::UpdateInThread()
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

	void TServerSocket::JoinServiceThread()
	{
		ServiceThread.join();
	}
	
	



} //namspace SE
