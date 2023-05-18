
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SERVICE_SOCKET_ADDR "./socket"
#define BUFFER_SIZE 1024

int creat_bind_socket(int type, char *socket_addr)
{
	int fd;
	struct sockaddr_un addr;

	fd = socket(AF_UNIX, type, 0);
	if(fd < 0){
		perror("socket");
		return -1;
	}

	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, socket_addr);

	if(bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		perror("bind");
		return -1;
	}
	printf("bind socket addr:%s\n", addr.sun_path);
	return fd;
}

void unix_dgram()
{
	int fd;
	struct sockaddr_un addr; // service addr
	size_t size;
	char buf[BUFFER_SIZE];

	fd  = creat_bind_socket(SOCK_DGRAM, tmpnam(NULL)); // tmp local addr
	if(fd < 0)
		exit(-1);
	
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, SERVICE_SOCKET_ADDR);
	
	size = sendto(fd, "hello", 6, 0, (const struct sockaddr *)&addr, sizeof(addr));
	printf("sendto size: %ld\n", size);

	size = recvfrom(fd, buf, BUFFER_SIZE, 0, NULL, NULL);
	printf("recvfrom size:%ld: %s\n", size, buf);

	// close(fd);  // close automatic when exit
}

void unix_stream()
{
	int fd;
	char buf[BUFFER_SIZE];
	size_t size;
	struct sockaddr_un addr; // service addr
	socklen_t addr_len = sizeof(addr);

	fd  = creat_bind_socket(SOCK_STREAM, tmpnam(NULL)); // tmp local addr
	if(fd < 0)
		exit(-1);
	
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, SERVICE_SOCKET_ADDR);

	connect(fd, (struct sockaddr *)&addr, sizeof(addr));

	size = write(fd, "hello", 6);
	printf("write size: %ld\n", size);

	size = read(fd, buf, BUFFER_SIZE);
	printf("read size: %ld: %s\n", size, buf);
}

int main()
{

	// unix_dgram();
	unix_stream();

	return 0;
}
