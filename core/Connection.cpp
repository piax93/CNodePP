#include "Connection.hpp"

namespace NPPcore {

Connection* Connection::instance = NULL;

Connection* Connection::getInstance(uint16_t port) {
	if(instance == NULL) instance = new Connection(port);
	return instance;
}

Connection::Connection(uint16_t port) {
	struct sockaddr_in saddr;
	server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = INADDR_ANY;
	bind(server_socket, (struct sockaddr*)&saddr, ADDR_SIZE);
	struct sigaction action;
	bzero(&action, sizeof(struct sigaction));
	action.sa_handler = Connection::sigHandler;
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGTERM, &action, NULL);
}

void Connection::listenAndServe(int max_connections){
	listen(server_socket, max_connections);
	struct sockaddr_in client;
	socklen_t client_len = ADDR_SIZE;
	while(true){
		Socket act_sock = accept(server_socket, (struct sockaddr*)&client, &client_len);
		if(act_sock > 0 && fork() == 0){
			httpProcess(act_sock);
			closeSocket(act_sock);
			break;
		}
		closeSocket(act_sock);
	}
}

void Connection::sigHandler(int sig){
	delete instance;
	exit(0);
}

Connection::~Connection() {
	closeSocket(server_socket);
}

void httpProcess(Socket act_sock){
	HTTPRequest request;
	request.load(fdopen(act_sock, "r"));
	std::cout << request;
	exit(0);
}

void closeSocket(Socket sock){
	close(sock);
}

} /* namespace NPPcore */
