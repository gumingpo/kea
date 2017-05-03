#ifndef _UDP_H
#define _UDP_H
#include <stdio.h> //printf
#include <string.h> //memset
#include <stdlib.h> //exit(0);
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>

#define SERVER "127.0.0.1"
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to send data
extern "C" {
    int udp_send(char *message, int msglen);
}

#endif //_UDP_H
