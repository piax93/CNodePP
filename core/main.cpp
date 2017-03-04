#include <iostream>
#include "Connection.hpp"
#include "Configuration.hpp"

int main(int argc, char **argv) {

    if(argc > 1) NPPcore::Configuration::setup(argv[1]);

    NPPcore::Connection connection;
    connection.listenAndServe();
    connection.waitAndBleed();

    std::cerr << " -- Terminated -- " << std::endl;

    return 0;
}
