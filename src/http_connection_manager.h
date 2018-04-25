#ifndef _HTTP_CONNECTION_MANAGER_H_
#define _HTTP_CONNECTION_MANAGER_H_

#include <set>
#include "http_connection.h"

namespace http {
namespace server {

class connection_manager
{
public:
	connection_manager(const connection_manager&) = delete;
	connection_manager& operator=(const connection_manager&) = delete;

	connection_manager();

	// acces methods
	void start(connection_ptr c);
	void stop(connection_ptr c);
	void stop_all();

private:
	std::set<connection_ptr> http_connections;
};

}
}

#endif
