#include "http_connection_manager.h"

namespace http {
namespace server {

connection_manager::connection_manager() {
}

void connection_manager::start(connection_ptr c) {
	http_connections.insert(c);
	c->BeforeStart();
	c->start();
}

void connection_manager::stop(connection_ptr c) {
	http_connections.erase(c);
	c->stop();
}

void connection_manager::stop_all() {
	for (auto c : http_connections) {
		c->stop;
	}
	http_connections.clear();
}
}
}