#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <string>
#include <map>
#include <vector>
#include "boost/shared_array.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/foreach.hpp"

#include "boost/asio.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/signals2.hpp"
#include "boost/variant.hpp"

#include "SignalSender.h"
#include "Network.h"

namespace SE
{
class TServerSocket
{
public:
	//std::vector<std::shared_ptr<TConnectedUser>> UserArr;

	boost::asio::ip::tcp::acceptor acceptor2;

	boost::asio::io_service IoService;

protected:

	std::string getPassword();

	boost::asio::ip::tcp::endpoint endpoint;

	boost::asio::ip::tcp::acceptor acceptor;


	boost::thread ServiceThread;

	


public:

	void UpdateInThread();

	TServerSocket(int port);
	virtual ~TServerSocket();

	void JoinServiceThread();
	
};




} //namespace SE


#endif