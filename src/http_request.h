#ifndef _HTTP_REQUEST_H_
#define _HTTP_REQUEST_H_

#include <string>
#include <vector>
#include "http_header.h"

namespace http {
namespace server{

	struct request
	{
	std::string method;
	std::string uri;
	int http_major_ver;
	int http_minor_ver;
	std::vector<header> headers;
	};

}
}

#endif