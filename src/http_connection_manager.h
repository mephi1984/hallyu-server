#ifndef HTTP_CONNECTION_MANAGER_H
#define HTTP_CONNECTION_MANAGER_H
/*
#include <set>
#include <memory>
*/

#include <set>
#include <memory>
#include <array>


namespace http {
namespace server {

	class connection;
	//typedef std::shared_ptr<connection> connection_ptr;

class connection_manager
{
public:
	connection_manager(const connection_manager&) = delete;
	connection_manager& operator=(const connection_manager&) = delete;

	connection_manager();

	// access methods
	void start(std::shared_ptr<connection> c);
	void stop(std::shared_ptr<connection> c);
	void stop_all();
	
	int get_connections_count();

private:
	std::set<std::shared_ptr<connection>> http_connections;
};

}
}

#endif
