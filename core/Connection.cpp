#include "Connection.hpp"

namespace NPPcore {

Connection::Connection() {
	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGINT);
	sigaddset(&sigset, SIGTERM);
	sigaddset(&sigset, SIGPIPE);
	sigprocmask(SIG_BLOCK, &sigset, NULL);
	Configuration& conf = Configuration::getInstance();
	port = conf.getValueToInt("port");
	max_connection_queue = conf.getValueToInt("max_conn");
	nworkers = conf.getValueToInt("workers");
	nworkers = nworkers > 0 ? (nworkers < MAX_WORKERS ? nworkers : MAX_WORKERS) : 1;
	working = true;
	struct sockaddr_in saddr;
	server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if(server_socket < 0) util::die("Socket creation error");
	int yes = 1;
	if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) util::die("Your TCP socket is a spoiled child. You tried to play it cool but you failed");
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = INADDR_ANY;
	if(bind(server_socket, (struct sockaddr*)&saddr, ADDR_SIZE) < 0) util::die("Socket bind error");
	if(listen(server_socket, max_connection_queue) < 0) util::die("Socket refuses to listen");
}

void Connection::listenAndServe(bool dofork){
	if(dofork) {
		for(int i = 0; i < nworkers; i++) workers.push_back(std::thread(&Connection::listenAndServe, this, false));
	} else {
		struct sockaddr_in client;
		socklen_t client_len = ADDR_SIZE;
		while(true){
			acceptme.lock();
			Socket act_sock = accept(server_socket, (struct sockaddr*)&client, &client_len);
			acceptme.unlock();
			if(!working) return;
			if(act_sock > 0) httpProcess(act_sock, client);
			else std::cerr << "(accept): faulty connection" << std::endl;
		}
	}
}

void Connection::waitAndBleed(){
	sigset_t sigset;
	siginfo_t siginfo;
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGINT);
	sigaddset(&sigset, SIGTERM);
	sigaddset(&sigset, SIGPIPE);
	while(true){
		sigwaitinfo(&sigset, &siginfo);
		if(siginfo.si_signo == SIGINT || siginfo.si_signo == SIGTERM) {
			std::cout << '\r';
			working = false;
			return;
		}
	}
}

Connection::~Connection() {
	closeSocket(server_socket);
	for(unsigned i = 0; i < workers.size(); i++) workers[i].join();
	std::cerr << "Connection closed" << std::endl;
}

void httpProcess(Socket act_sock, struct sockaddr_in clientaddress){
	FILE* socket_pointer = fdopen(act_sock, "r+");
	HTTPRequest request(clientaddress);
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
	} catch (NodeppUnsupported& e) {
		response.setCode(501);
	} catch(std::exception& e){
		std::cerr << e.what() << std::endl;
		response.setCode(500);
		ml.getPage("500", request, response);
	}
	response.send(socket_pointer);
	closeSocket(act_sock);
}

int closeSocket(Socket sock){
	return shutdown(sock, SHUT_RDWR);
}

} /* namespace NPPcore */
