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
private:
	std::set<connection_ptr> connections;
};

}
}

#endif
