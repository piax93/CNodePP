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
	}
}

void Connection::sigHandler(int sig){
	delete instance;
	std::cerr << "Stopped" << std::endl;
	std::exit(0);
}

Connection::~Connection() {
	closeSocket(server_socket);
}

void httpProcess(Socket act_sock){
	FILE* socket_pointer = fdopen(act_sock, "r+");
	HTTPRequest request = HTTPRequest();
	request.load(socket_pointer);
	ModuleLoader* ml = ModuleLoader::getInstance();
	if(request.getRoute()[0] != '_'){
		try {
			getPage_t getPage = (getPage_t)ml->getMethod(request.getRoute(), "getPage");
			HTTPResponse response(request.getVersion(), 200);
			getPage(request, response);
			response.send(socket_pointer);
		} catch (const NodeppError& e) {
			HTTPResponse response(request.getVersion(), 404);
			// TODO: implement 404
		}
	} else {
		// TODO: Forbidden
	}
	closeSocket(act_sock);
}

void closeSocket(Socket sock){
	close(sock);
}

} /* namespace NPPcore */
