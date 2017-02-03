#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include "Connection.hpp"
#include "ModuleLoader.hpp"
#include "Configuration.hpp"

int main(int argc, char **argv) {
	NPPcore::Connection* connection = NPPcore::Connection::getInstance(NPPcore::Configuration::self.getValueToInt("port"));
	connection->listenAndServe(NPPcore::Configuration::self.getValueToInt("max_conn"));
	return 0;
}
