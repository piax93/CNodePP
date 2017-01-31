#include <iostream>
#include <stdlib.h>
#include "utilities.hpp"
#include "Connection.hpp"
#include "Configuration.hpp"


int main(int argc, char **argv) {
	NPPcore::Configuration conf = NPPcore::Configuration("settings.conf");
	NPPcore::Connection* connection = NPPcore::Connection::getInstance(conf.getValueToInt("port"));
	connection->listenAndServe(conf.getValueToInt("max_conn"));
	return 0;
}
