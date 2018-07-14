#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	if (argc != 5) {
		printf("Usage: UDP_CLIENT [multicast_ip] [port] [ip] [msg_type] (0 for \"Hello!\", 1 for \"Quit\")\n");
		return -2;
	}
	int socket_fd, ret, on = 1;
	size_t len;
	struct sockaddr_in target;
	socklen_t target_size;

	char buf[16] = {0};

	socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (socket_fd == -1) {
		perror("socket");
		return -1;
	}
	
	memset(&target, 0, sizeof(target));
	target.sin_family = AF_INET;
	target.sin_port = htons(atoi(argv[2]));
	target.sin_addr.s_addr = inet_addr(argv[1]);
	target_size = sizeof(target);
	len = sizeof(buf);
	if (atoi(argv[4])) {
		sprintf(buf, "Quit");
	} else {
		sprintf(buf, "Hello!");
	}

	struct in_addr inaddr;
	inaddr.s_addr = inet_addr(argv[3]);

	if (setsockopt(socket_fd, IPPROTO_IP, IP_MULTICAST_LOOP, &on, sizeof(on)) < 0) {
		perror("setsockopt-mcast_loop");
		return -4;
	}
	if (setsockopt(socket_fd, IPPROTO_IP, IP_MULTICAST_IF, &inaddr, sizeof(inaddr)) < 0) {
		perror("setsockopt-mcast_if");
		return -4;
	}
	on = 255;
	if (setsockopt(socket_fd, IPPROTO_IP, IP_MULTICAST_TTL, &on, sizeof(on)) < 0) {
		perror("setsockopt-mcast_ttl");
		return -4;
	}

	ret = sendto(socket_fd, buf, len, 0, (struct sockaddr*)&target, target_size);
	if (ret == -1) {
		perror("send");
		return -3;
	}

	close(socket_fd);
	return 0;
}


