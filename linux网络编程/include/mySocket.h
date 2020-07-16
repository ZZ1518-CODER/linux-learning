#ifndef __MY_SOCKET_H__
#define __MY_SOCKET_H__
#include <stdio.h>
#include <stdlib.h>
inline void err_exit(const char* s) {
	perror(s);
	exit(-1);
}

int Socket(int domain, int type, int protocol);

int Bind(int sockfd, struct sockaddr_in *addr, size_t addrlen); 

int Listen(int sockfd, int backlog);

int Accept(int sockfd, struct sockaddr_in *addr, size_t *addrlen);

int Connect(int sockfd, struct sockaddr_in *addr, size_t addrlen);

int Read(int fd, void *buf, size_t count);

int Write(int fd, const void *buf, size_t count);

int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

#endif
