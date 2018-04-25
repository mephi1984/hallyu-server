#include "http_connection.h"

namespace http {
namespace server {
	
	connection::connection(boost::asio::ip::tcp::socket isocket, connection_manager& manager/*, request_handler& handler*/)
		: http_socket(std::move(isocket))
		, http_connection_manager(manager)
		/*, http_request_handler(handler)*/
	{
		
	}

}
}
