#ifndef _HTTP_HEADER_H_
#define _HTTP_HEADER_H_

#include <string>

namespace http {
namespace server {

	struct header
	{
		std::string name;
		std::string value;
	};

}
}

#endif