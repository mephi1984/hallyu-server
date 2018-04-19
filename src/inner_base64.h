#ifndef INNER_BASE64_H_INCLUDED
#define INNER_BASE64_H_INCLUDED


#include <string>

namespace B
{
	std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);

	std::string base64_decode(std::string const& encoded_string);
}


#endif //INNER_BASE64_H_INCLUDED
