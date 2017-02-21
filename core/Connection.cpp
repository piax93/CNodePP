#include "Connection.hpp"

namespace NPPcore {

Connection::Connection() {
	Configuration& conf = Configuration::getInstance();
	port = conf.getValueToInt("port");
	max_connection_queue = conf.getValueToInt("max_conn");
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

void Connection::listenAndServe(){
	listen(server_socket, max_connection_queue);
	struct sockaddr_in client;
	socklen_t client_len = ADDR_SIZE;
	while(true){
		Socket act_sock = accept(server_socket, (struct sockaddr*)&client, &client_len);
		if(act_sock > 0) std::thread(httpProcess, act_sock).detach();
		else std::cerr << "(accept): faulty connection" << std::endl;
	}
}

void Connection::sigHandler(int sig){
	util::die("\rStopped", 0);
}

Connection::~Connection() {
	closeSocket(server_socket);
}

void httpProcess(Socket act_sock){
	FILE* socket_pointer = fdopen(act_sock, "r+");
	HTTPRequest request;
	request.load(socket_pointer);
	HTTPResponse response(request, 200);
	ModuleLoader& ml = ModuleLoader::getInstance();
	try {
		if(ml.hasModule(request.getRoute())) {
			if(request.getRoute()[0] != '_'){
				ml.getPage(request.getRoute(), request, response);
			} else {
				response.setCode(403);
				ml.getPage("403", request, response);
			}
		} else {
			response.setCode(404);
			ml.getPage("404", request, response);
		}
	} catch(std::exception& e){
		std::cerr << e.what() << std::endl;
		response.setCode(500);
		ml.getPage("500", request, response);
	}
	response.send(socket_pointer);
	closeSocket(act_sock);
}

void closeSocket(Socket sock){
	shutdown(sock, SHUT_RDWR);
}

} /* namespace NPPcore */
