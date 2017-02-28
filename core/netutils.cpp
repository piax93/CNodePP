#include "netutils.hpp"

namespace util {

Socket createServerSocket(u_int16_t port, uint16_t max_connection_queue) {
	Socket server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if(server_socket < 0) NPPcore::die("Socket creation error");
	int yes = 1;
	if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
		NPPcore::die("Your TCP socket is a spoiled child. You tried to play it cool but you failed");
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = INADDR_ANY;
	if(bind(server_socket, (struct sockaddr*)&saddr, ADDR_SIZE) < 0) NPPcore::die("Socket bind error");
	if(listen(server_socket, max_connection_queue) < 0) NPPcore::die("Socket refuses to listen");
	return server_socket;
}


void sendFile(Socket socket, const std::string& filename){
	char buffer[BUFF_LEN];
	int input_file = open(filename.c_str(), O_RDONLY);
	if(input_file < 0) throw NPPcore::NodeppError("Could not open file " + filename);
	ssize_t bytes_read;

	while (1) {
	    bytes_read = read(input_file, buffer, BUFF_LEN);
	    if(bytes_read == 0) break;
	    if(bytes_read < 0) throw NPPcore::NodeppError("Error reading file " + filename);
	    if(sendn(socket, buffer, bytes_read) != bytes_read) throw NPPcore::NodeppError("Error sending file " + filename);
	}

	close(input_file);
}


static magic_t magic_database_pointer = NULL;

std::string getMimeType(const std::string& filename) {
	if(magic_database_pointer == NULL) {
		magic_database_pointer = magic_open(MAGIC_MIME_TYPE);
		if(magic_database_pointer == NULL || magic_load(magic_database_pointer, NULL) < 0)
			return "application/octet-stream";
		std::atexit([]{ magic_close(magic_database_pointer); std::cerr << "Freed libmagic database" << std::endl; });
	}
	return std::string(magic_file(magic_database_pointer, filename.c_str()));
}


size_t getFileSize(const std::string& filename){
	struct stat st;
	if(stat(filename.c_str(), &st)) throw NPPcore::NodeppError("Cannot read file stats");
	return st.st_size;
}

bool isRegularFile(const std::string& filename){
	struct stat st;
	if(stat(filename.c_str(), &st)) throw NPPcore::NodeppError("Cannot read file stats");
	return S_ISREG(st.st_mode);
}

ssize_t sendn(Socket fd, const char* vptr, size_t n) {
	ssize_t nwritten;
	size_t nleft = n;
	const char* ptr = vptr;
	while (nleft > 0) {
		if ((nwritten = send(fd, ptr, nleft, 0)) <= 0) return -1;
		nleft -= nwritten;
		ptr += nwritten;
	}
	return n;
}


ssize_t recvn(Socket fd, char* vptr, size_t n) {
	ssize_t nread;
	size_t nleft = n;
	char* ptr = vptr;
	while (nleft > 0) {
		if ((nread = recv(fd, ptr, nleft, 0)) < 0) return -1;
		else if (nread == 0) break;
		nleft -= nread;
		ptr += nread;
	}
	return n - nleft;
}


ssize_t netgetline(Socket fd, char* vptr, size_t maxlen) {
	char* ptr = vptr;
	ssize_t rc;
	size_t n;
	char c;
	for(n = 1; n < maxlen; n++) {
		if((rc = recv(fd, &c, 1, 0)) == 1) {
			*ptr++ = c;
			if (c == '\n') break;
		} else if (rc == 0) {
			if (n == 1) return 0;
			else break;
		} else {
			return -1;
		}
	}
	*ptr = '\0';
	return n;
}


void closeSocket(Socket sock){
	if(shutdown(sock, SHUT_RDWR)) std::cerr << "Failed to close socket" << std::endl;
}

} /* namespace util */
