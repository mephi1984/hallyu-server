/*
#include <sstream>
#include <fstream>
#include "http_request_handler.h"
#include "http_MIME_types.h"
#include "http_reply.h"
#include "http_request.h"
*/

#include "http_request_handler.h"

namespace http {
namespace server {

request_handler::request_handler(/*std::string doc_root*/)
	: root_dir(/*doc_root*/"default")
{
}

void request_handler::handle_request(const request& req, reply& rep){
	
	/*// ====== tmp section (sending file to client by url) ======	

	std::string request_path;
	if(!url_decode(req.uri,request_path)) {  // <=== Decoding url address
		rep = reply::stock_reply(reply::bad_request);
		return;
	}
	if (request_path.empty() || request_path[0] != '/' // <=== Request path must bee absolute
		|| request_path.find("..") != std::string::npos)
	{
		rep = reply::stock_reply(reply::bad_request);
		return;
	}
	if (request_path[request_path.size() - 1] == '/') // <=== If url ends "/" then add "index.html"
	{
		request_path += "index.html";
	}

	// Determine file extension
	std::size_t last_slash_pos = request_path.find_last_of("/");
	std::size_t last_dot_pos = request_path.find_last_of(".");
	std::string extension;
	if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
	{
		extension = request_path.substr(last_dot_pos + 1);
	}
	// Open the file to send back
	std::string full_path = root_dir + request_path;
	std::ifstream i_f_stream(full_path.c_str(), std::ios::in | std::ios::binary);
	if (!i_f_stream)
	{
		rep = reply::stock_reply(reply::not_found);
		return;
	}

	// ------ tmp section (...) ------ 

	// Fill out the reply
	rep.status = reply::ok;
	char buf[512];
	while(i_f_stream.read(buf, sizeof(buf)).gcount() > 0) {
		rep.content.append(buf, i_f_stream.gcount());
	}
	rep.headers.resize(2);
	rep.headers[0].name = "Content-Length";
	rep.headers[0].value = rep.content.size();
	rep.headers[1].name = "Content-Type";
	rep.headers[1].value = mime_types::extension_to_type(extension);
	*/
}

bool request_handler::url_decode(const std::string& in, std::string& out){
	out.clear();
	out.reserve(in.size());
	for (std::size_t i = 0; i < in.size(); ++i)
	{
		if (in[i] == '%')
		{
			if (i + 3 <= in.size())
			{
				int value = 0;
				std::istringstream is(in.substr(i + 1, 2));
				if (is >> std::hex >> value)
				{
					out += static_cast<char>(value);
					i += 2;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else if (in[i] == '+')
		{
			out += ' ';
		}
		else
		{
			out += in[i];
		}
	}
	return true;
}

}
}