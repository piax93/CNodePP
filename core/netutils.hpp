#ifndef NETUTILS_HPP_
#define NETUTILS_HPP_

#include <netinet/in.h>
#include <string>

#define ADDR_SIZE sizeof(struct sockaddr_in)
#define BUFF_LEN 4096
typedef int Socket;

namespace util {

/**
 * Create a IPV4 passive socket ready to accept incoming connections
 * @param port Listening port
 * @param max_connection_queue Maximum length of the TCP connection queue before further requests are refused
 * @return socket file descriptor
 */
Socket createServerSocket(u_int16_t port, uint16_t max_connection_queue);

/**
 * Send n bytes
 * @param fd Socket file descriptor
 * @param vptr Pointer to buffer
 * @param n Buffer size
 * @param write If true uses the write function instead of send
 * @return Sent bytes, -1 on error
 */
ssize_t sendn(Socket fd, const char* vptr, size_t n, bool usewrite = false);

/**
 * Receive n bytes
 * @param fd Socket file descriptor
 * @param vptr Pointer to buffer
 * @param n Buffer size
 * @return Bytes read, -1 on error
 */
ssize_t recvn(Socket fd, char* vptr, size_t n);

/**
 * Read a line from filedescriptor
 * @param fd Socket file descriptor
 * @param vptr Pointer to allocated buffer
 * @param maxlen Buffer size
 * @return Bytes read, -1 on error
 */
ssize_t netgetline(Socket fd, char* vptr, size_t maxlen);

/**
 * Send file over TCP socket
 * @param socket Socket file descriptor
 * @param filename Path to file
 */
void sendFile(Socket socket, const std::string& filename);

/**
 * Get MIME type of a file
 * @param filename Filepath
 */
std::string getMimeType(const std::string& filename);

/**
 * Get the size of a file in bytes
 * @param filename Filepath
 */
size_t getFileSize(const std::string& filename);

/**
 * Check if path points to regular file
 */
bool isRegularFile(const std::string& filename);

/**
 * Safely close socket file descriptor
 */
void closeSocket(Socket sock);

}  /* namespace util */



#endif /* NETUTILS_HPP_ */
