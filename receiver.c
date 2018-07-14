#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>

int main(int argc, char **argv)
{
	if (argc != 4) {
		printf("Usage: RECEIVER [ip] [port] [multicast_ip]\n");
		return -1;
	}

	int sock_fd, ret;
	size_t len;

	char buf[16] = {0};
	len = sizeof(buf);
	sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	int on = 1;
	
	struct sockaddr_in target, server;
	struct ip_mreq mreq;
	socklen_t target_size = sizeof(target), server_size = sizeof(server);
	memset(&server, 0, server_size);
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr.s_addr = inet_addr(argv[3]);
	memset(&mreq, 0, sizeof(mreq));
	mreq.imr_multiaddr.s_addr = inet_addr(argv[3]);
	mreq.imr_interface.s_addr = inet_addr(argv[1]);

	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
		perror("setsockopt-reuseaddr");
		return -4;
	}
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on)) < 0) {
		perror("setsockopt-reuseport");
		return -4;
	}
	if (setsockopt(sock_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
		perror("setsockopt-add_mbrship");
		return -4;
	}

	if (bind(sock_fd, (struct sockaddr *) &server, server_size) == -1) {
		perror("bind");
		return -3;
	}

	while (1)	{
		memset(buf, 0, len);
		memset(&target, 0, target_size);
		recvfrom(sock_fd, buf, len, 0, (struct sockaddr *) &target, &target_size);
		printf("UDP Connection from %s:%d\n", inet_ntoa(target.sin_addr), ntohs(target.sin_port));
		if (strcmp(buf, "Quit") != 0) {
			printf("%s\n", buf);
		} else {
			printf("%s\n", buf);
			break;
		}

	}
	close(sock_fd);

	return 0;
}



