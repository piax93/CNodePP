#ifndef CONNECTION_HPP_
#define CONNECTION_HPP_

#include "HTTPResponse.hpp"
#include "ModuleLoader.hpp"
#include "HTTPRequest.hpp"
#include "strutils.hpp"
#include "netutils.hpp"
#include <sys/socket.h>
#include <signal.h>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>

#define MAX_WORKERS 128

namespace NPPcore {

class Connection {
private:
    Socket server_socket;
    uint16_t nworkers;
    std::vector<std::thread> workers;
    std::mutex acceptme;
    bool working;
    static void httpProcess(Socket act_sock, struct sockaddr_in clientaddress);
public:
    Connection();
    void listenAndServe(bool dofork = true);
    void waitAndBleed();
    virtual ~Connection();
};

} /* namespace NPPcore */

#endif /* CONNECTION_HPP_ */
