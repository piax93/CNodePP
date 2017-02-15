#include "Connection.hpp"

namespace NPPcore {

Connection::Connection(uint16_t port) {
	struct sockaddr_in saddr;
	server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if(server_socket < 0) util::die("Socket creation error");
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = INADDR_ANY;
	if(bind(server_socket, (struct sockaddr*)&saddr, ADDR_SIZE) < 0) util::die("Socket bind error");
	struct sigaction action;
	bzero(&action, sizeof(struct sigaction));
	action.sa_handler = Connection::sigHandler;
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGTERM, &action, NULL);
	sigaction(SIGPIPE, NULL, NULL);
}

void Connection::listenAndServe(int max_connections){
	listen(server_socket, max_connections);
	struct sockaddr_in client;
	socklen_t client_len = ADDR_SIZE;
	while(true){
		Socket act_sock = accept(server_socket, (struct sockaddr*)&client, &client_len);
		if(act_sock > 0) std::thread(httpProcess, act_sock).detach();
		else std::cerr << "(accept): faulty connection" << std::endl;
	}
}

void Connection::sigHandler(int sig){
	util::die("Stopped", 0);
}

Connection::~Connection() {
	closeSocket(server_socket);
}

void httpProcess(Socket act_sock){
	FILE* socket_pointer = fdopen(act_sock, "r+");
	HTTPRequest request = HTTPRequest();
	HTTPResponse response = HTTPResponse(request, 200);
	request.load(socket_pointer);
	ModuleLoader& ml = ModuleLoader::getInstance();
	getPage_t getPage;
	if(ml.hasModule(request.getRoute())) {
		if(request.getRoute()[0] != '_'){
			getPage = (getPage_t)ml.getMethod(request.getRoute(), "getPage");
			getPage(request, response);
		} else {
			response.setCode(403);
			if(ml.hasModule("403")) {
				getPage = (getPage_t)ml.getMethod("403", "getPage");
				getPage(request, response);
			}
		}
	} else {
		response.setCode(404);
		if(ml.hasModule("404")) {
			getPage = (getPage_t)ml.getMethod("404", "getPage");
			getPage(request, response);
		}
	}
	response.send(socket_pointer);
	closeSocket(act_sock);
}

void closeSocket(Socket sock){
	close(sock);
}

} /* namespace NPPcore */
