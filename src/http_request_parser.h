#ifndef HTTP_REQUEST_PARSER_H
#define HTTP_REQUEST_PARSER_H
/*
#include "http_request.h"
#include <tuple>
*/

#include <tuple>
#include "http_request.h"

namespace http {
namespace server {

	class request_parser
	{
	public:
		request_parser();

		enum result_type {good, bad, indeterminate};

		//template<typename Buffer_iterator>
		//std::tuple<result_type, Buffer_iterator> parse(request& req, Buffer_iterator begin, Buffer_iterator end);
		template<typename Buffer_iterator>
		std::tuple<request_parser::result_type, Buffer_iterator> parse(request& req, Buffer_iterator begin, Buffer_iterator end)
		{
			int tmp_iter = 0;
			//std::cout << "http_request_parser before loop, iteraror " << *begin << " " << *end << std::endl;
			while (begin != end) {
				result_type result = consume(req, *begin++);
				tmp_iter++;
				if (result == good || result == bad) {
					if (result == good) { // store request content at req
						while (begin != end) {
							req.request_content.push_back(*begin++);
						}
					}
					return std::make_tuple(result, begin);
				}
			}
			return std::make_tuple(indeterminate, begin);
		}

	private:
		result_type consume(request& req, char input);

		static bool is_char(int c);
		static bool is_ctl(int c);
		static bool is_tspecial(int c);
		static bool is_digit(int c);

		enum state
		{
			method_start,
			method,
			uri,
			http_version_h,
			http_version_t_1,
			http_version_t_2,
			http_version_p,
			http_version_slash,
			http_version_major_start,
			http_version_major,
			http_version_minor_start,
			http_version_minor,
			expecting_newline_1,
			header_line_start,
			header_lws,
			header_name,
			space_before_header_value,
			header_value,
			expecting_newline_2,
			expecting_newline_3
		} state_;
	};


}
}


#endif