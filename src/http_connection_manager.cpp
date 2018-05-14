

#include "http_connection_manager.h"
#include "http_connection.h"

namespace http {
namespace server {

connection_manager::connection_manager() {
}

void connection_manager::start(std::shared_ptr<connection> c) {
	http_connections.insert(c);
	c->start();
}

void connection_manager::stop(std::shared_ptr<connection> c) {
	http_connections.erase(c);

	c->stop();
}

void connection_manager::stop_all() {
	for (auto c : http_connections) {
		c->stop();
	}
	http_connections.clear();
}

int connection_manager::get_connections_count() {
	return http_connections.size();
}

}
}