#ifndef SIGNAL_SENDER_H_INCLUDED
#define SIGNAL_SENDER_H_INCLUDED

#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/array.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/signals2.hpp"
#include "boost/thread.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"

namespace SE
{
	//Xperimental -- need to optimize this

struct TSignalSender
	: public std::enable_shared_from_this<TSignalSender>
{
	boost::asio::ip::tcp::socket& Socket;

	int Length;

	std::string Data;

	TSignalSender(boost::asio::ip::tcp::socket& socket, int length, std::string data)
		: Socket(socket)
		, Length(length)
		, Data(data)
	{
	}

	void Send()
	{
		std::function<void(const boost::system::error_code& error, size_t)> f = std::bind(&TSignalSender::Handle, shared_from_this(), std::placeholders::_1, std::placeholders::_2);
		boost::asio::async_write(Socket, boost::asio::buffer(&Length, 4), f);
		boost::asio::async_write(Socket, boost::asio::buffer(Data.c_str(), Data.size()), f);
	}

	void Handle(const boost::system::error_code& error, size_t)
	{
		if (error)
		{

		}
	}
};

} //namespace SE

#endif
