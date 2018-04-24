#ifndef _HTTP_CONNECTION_H_
#define _HTTP_CONNECTION_H_

#include <memory>
#include "http_connection_manager.h"

namespace http {
namespace server {

	class connection : public std::enable_shared_from_this<connection>
	{
	public:
		connection(const connection&) = delete;
		connection& operator=(const connection&) = delete;

		explicit connection(); // tmp before connection manager init

	private:
	
	};

	typedef std::shared_ptr<connection> connection_ptr;
}
}

#endif
