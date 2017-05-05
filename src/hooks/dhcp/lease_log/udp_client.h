#ifndef _UDP_H
#define _UDP_H
#include <stdio.h> //printf
#include <string.h> //memset
#include <stdlib.h> //exit(0);
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>

extern "C" {

	#define BUFLEN 512  //Max length of buffer
	struct UDP_SERVER {
		char server[16] = "";
		uint16_t port;
	};

	extern struct UDP_SERVER udpServer;
    int udp_send(char *message, int msglen, char *server, int port);
}

#endif //_UDP_H
