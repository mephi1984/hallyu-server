#include "http_connection.h"

namespace http {
namespace server {
	
	connection::connection(boost::asio::ip::tcp::socket isocket, connection_manager& manager, request_handler& handler)
		: http_socket(std::move(isocket))
		, http_connection_manager(manager)
		, http_request_handler(handler)
	{
	}

	void connection::start() {
		do_read();
	}

	void connection::do_read() {
		auto self(shared_from_this());
		http_socket.async_read_some(boost::asio::buffer(connection_buffer),
			[this,self](boost::system::error_code ec, std::size_t bytes_transfered) {
				if(!ec) {
					request_parser::result_type result;
					std::tie(result, std::ignore) = request_parser_.parse(http_request, connection_buffer.data(), connection_buffer.data() + bytes_transfered);
					if (result == request_parser::good)
					{
						http_request_handler.handle_request(http_request, http_reply);
						do_write();
					}
					else if (result == request_parser::bad)
					{
						http_reply = reply::stock_reply(reply::bad_request);
						do_write();
					}
					else
					{
						do_read();
					}
				} else if (ec != boost::asio::error::operation_aborted) {
					http_connection_manager.stop(shared_from_this());
				}
		});
	}

	void connection::do_write() {
		auto self(shared_from_this());
		boost::asio::async_write(http_socket,http_reply.to_buffers(),
			[this,self](boost::system::error_code ec, std::size_t)
		{
			if(!ec) {
				boost::system::error_code ignored_ec;
				http_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec); // <== shutting down socket after handle
			}

			if (ec != boost::asio::error::operation_aborted) {
				http_connection_manager.stop(shared_from_this());
			}
		});
	}

	void connection::stop() {
		http_socket.close();
	}
}
}
