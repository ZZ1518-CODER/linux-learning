#include <iostream>
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
inline void err_exit(const char* s) {
	perror(s);
	exit(-1);
}

int main(int argc, char* argv[]) {
	if(argc < 3) {
		std::cout << "please input ip port" << std::endl;
		exit(-1);
	}
	int clt_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(clt_fd == -1) {
		err_exit("socket:");
	}
	struct sockaddr_in clt_addr;
	clt_addr.sin_family = AF_INET;
	clt_addr.sin_port = htons(atoi(argv[2]));
	clt_addr.sin_addr.s_addr = inet_addr(argv[1]);
	int conn = connect(clt_fd, (const struct sockaddr*)&clt_addr, sizeof(clt_addr));
	if(conn == -1) {
		err_exit("connect");
	}
	else {
		std::cout << "connect success!" << std::endl;
	}
	char send_buf[1024];
	char recv_buf[1024];
	while(fgets(send_buf, sizeof(send_buf), stdin) != nullptr) {
		int ret = write(clt_fd, send_buf, strlen(send_buf));
		if(ret == -1) {
			err_exit("write:");
		}
		ret = read(clt_fd, recv_buf, sizeof(recv_buf));
		if(ret == -1) {
			err_exit("read");
		}
		else if(ret == 0) {
			std::cout << "connection closed" << std::endl;
			break;
		}
		fputs(recv_buf, stdout);
		memset(send_buf, 0, sizeof(send_buf));
		memset(recv_buf, 0, sizeof(recv_buf));
	}
	close(clt_fd);
	return 0;
}
