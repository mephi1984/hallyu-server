#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <string>
#include <vector>
#include "http_header.h"

namespace http {
namespace server {

	struct request
	{
	std::string method;
	std::string uri;
	int http_version_major;
	int http_version_minor;
	std::vector<header> headers;
	std::vector<char> request_content;
	};

}
}

#endif