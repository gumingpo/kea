/*
    Simple udp client
*/
#include <udp_client.h>
extern "C" {
    struct UDP_SERVER udpServer;
	int udp_send(char *msg, int msglen, char *server, int port)
	{
		struct sockaddr_in si_other;
		int slen=sizeof(si_other);
		int sock;

		if(msg == 0 || msg[0] == '\0' || msglen <= 0) {
            return -1;
		}
		if(server == NULL || server[0] == '\0') {
			perror("udp server is not configured");
			return -2;
		}

		if(port == 0) {
			perror("udp server port is not configured");
			return -3;
		}

		if ( (sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		{
			perror("failed to create socket");
			return -4;
		}

		memset((char *) &si_other, 0, slen);
		si_other.sin_family = AF_INET;
		si_other.sin_port = htons(port);

		if (inet_aton(server , &si_other.sin_addr) == 0)
		{
			perror("failed to convert ip address\n");
			return -5;
		}

		//send the message
		//ssize_t sendto(int socket, const void *buffer, size_t length, int flags, const struct sockaddr *dest_addr, socklen_t dest_len);
		if (sendto(sock, msg, msglen , 0 , (struct sockaddr *) &si_other, slen)==-1)
		{
			perror("failed to send message");
			return errno;
		}

		close(sock);
		return 0;
	}
}
