#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include "mySocket.h"

int Socket(int domain, int type, int protocol) {
	int ret = socket(domain, type, protocol);
	if(ret == -1) {
		err_exit("socket");
	}
	return ret;
}

int Bind(int sockfd, struct sockaddr_in *addr, size_t addrlen) {
	int ret = bind(sockfd, (const struct sockaddr*)addr, addrlen);
	if(ret == -1) {
		err_exit("bind");
	}
	return ret;
}

int Listen(int sockfd, int backlog) {
	int ret = listen(sockfd, backlog);
	if(ret == -1) {
		err_exit("listen");
	}
	return ret;
}

int Accept(int sockfd, struct sockaddr_in *addr, size_t *addrlen) {
	int ret = accept(sockfd, (struct sockaddr*)addr, (socklen_t*)addrlen);
	if(ret == -1) {
		err_exit("accept");
	}
	return ret;
}

int Connect(int sockfd, struct sockaddr_in *addr, size_t addrlen) {
	int ret = connect(sockfd, (const struct sockaddr*)addr, addrlen);
	if(ret == -1) {
		err_exit("connect");
	}
	return ret;
}

int Read(int fd, void *buf, size_t count) {
	int ret = read(fd, buf, count);
	if(ret == -1) {
		err_exit("read");
	}
	return ret;
}

int Write(int fd, const void *buf, size_t count) {
	int ret = write(fd, buf, count);
	if(ret == -1) {
		err_exit("write");
	}
	return ret;
}

int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) {
	int ret = select(nfds, readfds, writefds, exceptfds, timeout);
	if(ret == -1) {
		err_exit("select");
	}
	return ret;
}

