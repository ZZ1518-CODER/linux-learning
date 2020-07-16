#include <iostream>
#include <cstdlib>
#include "tcp_server.h"


int main(int argc, char* argv[]) {
	if(argc < 2) {
		std::cout << "please input port.." << std::endl;
	}
	int port = atoi(argv[1]);
	Server myserver = Server(port);
	myserver.myEpoll();
	return 0;
}
