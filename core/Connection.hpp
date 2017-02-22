#ifndef CONNECTION_HPP_
#define CONNECTION_HPP_

#include "HTTPResponse.hpp"
#include "ModuleLoader.hpp"
#include "HTTPRequest.hpp"
#include "utilities.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <thread>
#include <mutex>

#define ADDR_SIZE sizeof(struct sockaddr_in)
#define MAX_WORKERS 128
typedef int Socket;

namespace NPPcore {

void httpProcess(Socket act_sock, struct sockaddr_in clientaddress);
int closeSocket(Socket sock);

class Connection {
private:
	Socket server_socket;
	uint16_t nworkers;
	uint16_t port;
	uint16_t max_connection_queue;
	std::vector<std::thread> workers;
	std::mutex acceptme;
	bool working;
public:
	Connection();
	void listenAndServe(bool dofork = true);
	void waitAndBleed();
	virtual ~Connection();
};

} /* namespace NPPcore */

#endif /* CONNECTION_HPP_ */
