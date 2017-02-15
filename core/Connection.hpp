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
#include <thread>

#define ADDR_SIZE sizeof(struct sockaddr_in)
typedef int Socket;

namespace NPPcore {

typedef void (*getPage_t)(HTTPRequest&, HTTPResponse&);

void httpProcess(Socket act_sock);
void closeSocket(Socket sock);

class Connection {
private:
	Socket server_socket;
	static void sigHandler(int sig);
	Connection(uint16_t port);
public:
	static Connection& getInstance(uint16_t port){
		static Connection instance(port);
		return instance;
	}
	void listenAndServe(int max_connections);
	virtual ~Connection();
};

} /* namespace NPPcore */

#endif /* CONNECTION_HPP_ */
