

#include "http_MIME_types.h"

namespace http {
namespace server {
namespace mime_types {

	struct mapping 
	{
		const char* extension;
		const char* mime_type;
	} mappings[] = {
		{"html","text/html"},
		{"htm","text/html"},
		{"jpg","image/jpeg"},
		{"gif","image/gif"},
		{"png","image/png"}
	};

	std::string extension_to_type(const std::string& extension) {
		for (mapping m: mappings) {
			if (m.extension == extension)
				return m.mime_type;
		}
		return "text/plain";
	}


}
}
}