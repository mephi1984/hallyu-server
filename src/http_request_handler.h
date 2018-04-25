#ifndef _HTTP_REQUEST_HADLER_H_
#define _HTTP_REQUEST_HANDLER_H_

#include <string>
#include "http_request.h"
#include "http_reply.h"

namespace http {
namespace server {

class request_handler
{
public:
	request_handler(const request_handler&) = delete;
	request_handler& operator=(const request_handler&) = delete;

	explicit request_handler(const std::string doc_root);

	void handle_request(const request& req, reply& rep); // core request handle method (produce a reply)
private:
	std::string root_dir;

	static bool url_decode(const std::string& in, std::string& out);
};

}
}

#endif