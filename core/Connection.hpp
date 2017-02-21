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
#include <string>
#include <thread>

#define ADDR_SIZE sizeof(struct sockaddr_in)
typedef int Socket;

namespace NPPcore {

void httpProcess(Socket act_sock);
void closeSocket(Socket sock);

class Connection {
private:
	Socket server_socket;
	uint16_t port;
	uint16_t max_connection_queue;
	static void sigHandler(int sig);
public:
	Connection();
	void listenAndServe();
	virtual ~Connection();
};

} /* namespace NPPcore */

#endif /* CONNECTION_HPP_ */
