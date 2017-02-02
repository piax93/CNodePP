#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include "Connection.hpp"
#include "ModuleLoader.hpp"
#include "Configuration.hpp"


int main(int argc, char **argv) {
	/*
	NPPcore::Connection* connection = NPPcore::Connection::getInstance(NPPcore::Configuration::self.getValueToInt("port"));
	connection->listenAndServe(NPPcore::Configuration::self.getValueToInt("max_conn"));
	*/
	NPPcore::ModuleLoader* ml = NPPcore::ModuleLoader::getInstance();
	std::this_thread::sleep_for(std::chrono::seconds(20));
	return 0;
}
