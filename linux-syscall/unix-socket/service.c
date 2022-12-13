
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <string.h>

#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SOCKET_ADDR "./socket" // Relative path is a not good idea, but for convenience.
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
	unlink(addr.sun_path);

	if(bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		perror("bind");
		return -1;
	}
	return fd;
}

void unix_dgram()
{
	int fd;
	char buf[BUFFER_SIZE];
	size_t size;
	struct sockaddr_un addr; // client addr
	socklen_t addr_len = sizeof(addr);

	fd = creat_bind_socket(SOCK_DGRAM, SOCKET_ADDR);
	if(fd < 0)
		exit(-1);

	// not exit
	while (1)
	{
		size = recvfrom(fd, buf, BUFFER_SIZE, 0, (struct  sockaddr *)&addr, &addr_len);
		if(size < 0){
			perror("recvfrom");
			exit(-1);
		}
		printf("recvfrom size: %ld: %s\n", size, buf);

		// client addr is not set.
		if(addr_len == 0)
			continue;

		size = sendto(fd,buf, size, 0, (const struct sockaddr *)&addr, sizeof(addr));
		printf("sendto: %s size: %ld\n", addr.sun_path, size);
	}
}



void unix_stream()
{
	int fd;
	char buf[BUFFER_SIZE];
	size_t size;
	

	fd = creat_bind_socket(SOCK_STREAM, SOCKET_ADDR);
	if(fd < 0)
		exit(-1);

	
	if(listen(fd, 5) < 0){
		perror("listen");
		exit(-1);
	}

	while(1){
		int conn_fd;
		struct sockaddr_un addr; // client addr
		socklen_t addr_len;

		addr_len = sizeof(addr);
		conn_fd = accept(fd, (struct sockaddr *)&addr, &addr_len);
		if(conn_fd < 0){
			perror("accept");
			exit(-1);
		}
		printf("accepted from :%s\n", addr.sun_path);

		pid_t pid = fork();
		if(pid < 0){
			perror("fork");
			exit(-1);
		}

		// parent process continue accept new connection. 
		if(pid > 0){
			close(conn_fd); // conn_fd is not used in parent process.
			// wait(); There have a small issue - child process will become zombies when they die.
			continue;
		}

		// child process read and write data.
		close(fd); // close listen fd.
		while(1){
			size = read(conn_fd, buf, sizeof(buf));
			if(size == 0) // client close connection.
				exit(0);
			if(size < 0){
				perror("read");
				exit(-1);
			}
			printf("read size: %ld: %s\n", size, buf);
			size = write(conn_fd, buf, size);
			printf("write size: %ld\n", size);
		}
		
		// close(conn_fd); // close automatic when exit.
		exit(0);
	}
}

int main()
{

	// unix_dgram();
	unix_stream();

	return 0;
}
