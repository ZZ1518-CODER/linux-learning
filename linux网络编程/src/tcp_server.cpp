#include "mySocket.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "tcp_server.h"
#include <sys/epoll.h>

Server::Server(uint16_t p):port(p) {
	srv_fd = Socket(AF_INET, SOCK_STREAM, 0);
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(port);
	srv_addr.sin_addr.s_addr = INADDR_ANY;
	Bind(srv_fd, &srv_addr, sizeof(srv_addr));
   	Listen(srv_fd, MAX_CONN);
}

void Server::mySelect() {
	fd_set reads, tmp;
	FD_ZERO(&reads);
	FD_SET(srv_fd, &reads);
	struct sockaddr_in conn_addr;
	size_t conn_len = sizeof(conn_len);
	int max_fd = srv_fd;
	while(1) {
		tmp = reads;
		Select(max_fd + 1, &tmp, nullptr, nullptr, nullptr);
		if(FD_ISSET(srv_fd, &tmp)) {
			int conn_fd = Accept(srv_fd, &conn_addr, &conn_len);
			std::cout << "new connection "
					<< "ip = " << inet_ntoa(conn_addr.sin_addr)
					<< "port = " << ntohs(conn_addr.sin_port)
					<< std::endl;
			FD_SET(conn_fd, &reads);
			max_fd = max_fd > conn_fd ? max_fd : conn_fd;
		}
		for(int i = srv_fd + 1; i <= max_fd; i++) {
			if(FD_ISSET(i, &tmp)) {
				char buf[BUF_LEN] = {0};
				int len = Read(i, buf, BUF_LEN);
				if(len == 0) {
					std::cout << "one connection closed.." << std::endl;
					close(i);
					FD_CLR(i, &reads);
				}
				else {
					fputs(buf, stdout);
				 	for(int j = 0; j < len; j++) {
						if(buf[j] >= 'a' && buf[j] <= 'z') {
							buf[j] -= 32;
						}
						else if(buf[j] >= 'A' && buf[j] <= 'Z') {
							buf[j] += 32;
						}
					}
					Write(i, buf + 1, len);
				}
			}
		}
	}
	close(srv_fd);
}

void Server::myEpoll() {
	int epfd = epoll_create(1024);
	if(epfd == -1) {
		err_exit("epoll_create");
	}
	struct epoll_event ev;
	ev.data.fd = srv_fd;
	ev.events = EPOLLIN;
	int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, srv_fd, &ev);
	if(ret == -1) {
		err_exit("epoll ctl");
	}
	struct epoll_event all[1024];
	struct sockaddr_in conn_addr;
	size_t conn_len = sizeof(conn_addr);
	while(1) {
		int epnum = epoll_wait(epfd, all, sizeof(all) / sizeof(all[0]), -1);
		if(epnum == -1) {
			err_exit("epoll_wait");
		}
		for(int i = 0; i < epnum; i++) {
			if(all[i].data.fd == srv_fd) {
				int conn_fd = Accept(srv_fd, &conn_addr, &conn_len);
				std::cout << "new connection "
					<< "ip = " << inet_ntoa(conn_addr.sin_addr)
					<< "port = " << ntohs(conn_addr.sin_port)
					<< std::endl;
				struct epoll_event conn_ev;
				conn_ev.data.fd = conn_fd;
				conn_ev.events = EPOLLIN;
				ret = epoll_ctl(epfd, EPOLL_CTL_ADD, conn_fd, &conn_ev);
				if(ret == -1) {
					err_exit("epoll conn ctl");
				}
			}
			else {
				int conn_fd = all[i].data.fd;
				char buf[BUF_LEN] = {0};
				int len = Read(conn_fd, buf, BUF_LEN);
				if(len == 0) {
					std::cout << "one connection closed.." << std::endl;
					
					ret = epoll_ctl(epfd, EPOLL_CTL_DEL, conn_fd, &all[i]);
					if(ret == -1) {
						err_exit("epoll_ctr");
					}
					close(conn_fd);
				}
				else {
					fputs(buf, stdout);
					for(int j = 0; j < len; j++) {
						if(buf[j] >= 'a' && buf[j] <= 'z') {
							buf[j] -= 32;
						}
						else if(buf[j] >= 'A' && buf[j] <= 'Z') {
							buf[j] += 32;
						}
					}
					Write(conn_fd, buf, len);
				}
			}

		}
	}


}
