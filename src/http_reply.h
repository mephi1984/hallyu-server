#ifndef HTTP_REPLY_H
#define HTTP_REPLY_H

#include <vector>
#include <boost/asio.hpp>
#include "http_header.h"

namespace http {
namespace server {

	struct reply
	{
	char *http_status;
	std::vector<header> headers;

	std::string reply_content;
	};
}
}

#endif