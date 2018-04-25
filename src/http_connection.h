#ifndef _HTTP_CONNECTION_H_
#define _HTTP_CONNECTION_H_

#include <memory>
#include <boost/asio.hpp>
#include "http_connection_manager.h"
#include "http_request_handler.h"

namespace http {
namespace server {

	class connection_manager;

	class connection : public std::enable_shared_from_this<connection>
	{
	public:
		connection(const connection&) = delete;
		connection& operator=(const connection&) = delete;

		explicit connection(boost::asio::ip::tcp::socket isocket, connection_manager& manager/*, request_handler& handler*/); // tmp before connection manager init

		// access methods
		void start();
		void stop();

	private:
		// ------ read/write ------- asynchronous
		void do_read();
		void do_write();
		// ====== read/write ======= asynchronous

		std::array<char, 8192> in_buffer;
	
		boost::asio::ip::tcp::socket http_socket;
		connection_manager& http_connection_manager;
	};

	typedef std::shared_ptr<connection> connection_ptr;
}
}

#endif
