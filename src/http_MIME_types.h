#ifndef _HTTP_MIME_TYPES_H_
#define _HTTP_MIME_TYPES_H_

#include <string>

namespace http{
namespace server {
namespace mime_types {

	std::string extension_to_type(const std::string& extension);

}
}
}

#endif