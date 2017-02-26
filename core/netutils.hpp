#ifndef NETUTILS_HPP_
#define NETUTILS_HPP_

#include "NodeppError.hpp"
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

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
 * @return Sent bytes
 */
size_t sendn(Socket fd, const char* vptr, size_t n);

/**
 * Receive n bytes
 * @param fd Socket file descriptor
 * @param vptr Pointer to buffer
 * @param n Buffer size
 * @return Bytes read
 */
size_t recvn(Socket fd, const char* vptr, size_t n);

/**
 * Send file over TCP socket
 * @param socket Socket file descriptor
 * @param filename Path to file
 */
void sendFile(Socket socket, const std::string& filename);

/**
 * Safely close socket file descriptor
 */
void closeSocket(Socket sock);

}  /* namespace util */



#endif /* NETUTILS_HPP_ */
