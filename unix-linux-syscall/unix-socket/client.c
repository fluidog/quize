
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
// #include <fcntl.h>


void creat_socket()
{
	int fd;
	struct sockaddr_un addr;

	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(fd < 0){
		perror("socket");
		exit(-1);
	}

	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, "./socket");
	unlink(addr.sun_path);

	if(bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		perror("bind");
		exit(-1);
	}

	if(listen(fd, 5) < 0){
		perror("listen");
		exit(-1);
	}

	while(1){
		int conn_fd;
		struct sockaddr_un conn_addr;
		socklen_t conn_len;

		conn_fd = accept(fd, (struct sockaddr *)&conn_addr, &conn_len);
		if(conn_fd < 0){
			perror("accept");
			exit(-1);
		}

		printf("accepted\n");
		close(conn_fd);
	}


}


int main()
{
	creat_socket();

	return 0;
}
