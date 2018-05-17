
#include "http_server.h"

#include "http_connection.h"
#include "LuaHelper.h"
#include "SE/misc.h"

#include <clocale>

namespace http {
namespace server {
	
	connection::connection(boost::asio::ip::tcp::socket isocket, connection_manager& manager, request_handler& handler, THallyuHttpServer& server)
		: HallyuHttpServer(server)
		, http_socket(std::move(isocket))
		, http_connection_manager(manager)
		, http_request_handler(handler)
	{
		std::cout << "http_connection constructor" << std::endl;
	}

	void connection::start() {
		do_read();
	}

	void connection::do_read() {
		
		auto self(shared_from_this());
		http_socket.async_read_some(boost::asio::buffer(connection_buffer),
			[this,self](boost::system::error_code ec, std::size_t bytes_transfered) {
				if(!ec) {
					std::cout << "http_connection bytes_transfered:: " << bytes_transfered << std::endl;
					request_parser::result_type result;
/*parse*/			std::tie(result, std::ignore) = http_request_parser.parse(http_request, connection_buffer.data(), connection_buffer.data() + bytes_transfered);

					if (result == request_parser::good)
					{
						http_request_handler.handle_request(http_request, http_reply);
						do_write();
						
					}
					else if (result == request_parser::bad)
					{
						http_connection_manager.stop(shared_from_this());
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
		auto sharedThis = shared_from_this();

		const char crlf[] = { '\r','\n' };
		const char separ[] = { ':',' ' };

		std::vector<boost::asio::const_buffer> rep_buf;
		rep_buf.push_back(boost::asio::buffer(&http_reply.reply_status.c_str()[0], http_reply.reply_status.size())); // reply status
		rep_buf.push_back(boost::asio::buffer(crlf));
		rep_buf.push_back(boost::asio::buffer(&http_reply.headers[0].name.c_str()[0], http_reply.headers[0].name.size()));
		rep_buf.push_back(boost::asio::buffer(separ));
		rep_buf.push_back(boost::asio::buffer(&http_reply.headers[0].value.c_str()[0], http_reply.headers[0].value.size()));
		rep_buf.push_back(boost::asio::buffer(crlf));
		rep_buf.push_back(boost::asio::buffer(&http_reply.headers[1].name.c_str()[0], http_reply.headers[1].name.size()));
		rep_buf.push_back(boost::asio::buffer(separ));
		rep_buf.push_back(boost::asio::buffer(&http_reply.headers[1].value.c_str()[0], http_reply.headers[1].value.size()));
		rep_buf.push_back(boost::asio::buffer(crlf));
		rep_buf.push_back(boost::asio::buffer(crlf));
		rep_buf.push_back(boost::asio::buffer(&http_reply.reply_content[0], http_reply.reply_content.size()));
		std::cout << "data SIZE:: " << http_reply.reply_content.size() << std::endl;

		boost::asio::async_write(http_socket, rep_buf,
			[rep_buf, this, sharedThis](boost::system::error_code ec, std::size_t bytes_transfered)
		{
			std::cout << "async_write bytes_transfered:: " << bytes_transfered << std::endl;

			if (ec)
			{
				SE::WriteToLog("Error in inner SendPropertyTree");
			}
			http_connection_manager.stop(sharedThis); // close connection
		}
		);
	}
	
	void connection::stop() {
		http_socket.close();
	}


}
}
