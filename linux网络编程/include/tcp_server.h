#ifndef __TCP_SERVER_H
#define __TCP_SERVER_H

#include "mySocket.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/time.h>
#include <sys/types.h>
const int BUF_LEN = 10;
const int MAX_CONN = 100;
class Server{
private:
	uint16_t port;
	int srv_fd;
	struct sockaddr_in srv_addr;
public:
	Server(uint16_t p);
	void mySelect();
	void myEpoll();
};

#endif
