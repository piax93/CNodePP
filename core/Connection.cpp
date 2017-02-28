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
	nworkers = conf.getValueToInt("workers");
	nworkers = nworkers > 0 ? (nworkers < MAX_WORKERS ? nworkers : MAX_WORKERS) : 1;
	server_socket = util::createServerSocket(conf.getValueToInt("port"), conf.getValueToInt("max_conn"));
	working = true;
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

void Connection::httpProcess(Socket act_sock, struct sockaddr_in clientaddress){
	HTTPRequest request(clientaddress);
	HTTPResponse response(request, 200);
	ModuleLoader& ml = ModuleLoader::getInstance();
	try {
		request.load(act_sock);
		if(request.isStatic()) {
			response.setStaticFile(request.getRoute());
		} else if(ml.hasModule(request.getRoute())) {
			if(request.getRoute()[0] != '_'){
				ml.getPage(request.getRoute(), request, response);
			} else {
				response.setCode(403);
				ml.getPage("403", request, response);
			}
		} else throw NodeppNotFound("Module not found " + request.getRoute());
	} catch (NodeppUnsupported& e) {
		std::cerr << e.what() << std::endl;
		response.setCode(501);
	} catch (NodeppNotFound& e) {
		std::cerr << e.what() << std::endl;
		response.setCode(404);
		ml.getPage("404", request, response);
	} catch(std::exception& e){
		std::cerr << e.what() << std::endl;
		response.setCode(500);
		ml.getPage("500", request, response);
	}
	response.send(act_sock);
	util::closeSocket(act_sock);
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
			std::cerr << '\r';
			working = false;
			return;
		}
	}
}

Connection::~Connection() {
	util::closeSocket(server_socket);
	for(unsigned i = 0; i < workers.size(); i++) workers[i].join();
	std::cerr << "Connection closed" << std::endl;
}

} /* namespace NPPcore */
