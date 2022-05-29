/**
 * @file user.c
 * @author fluidog (fluidog@163.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <sys/socket.h>
 #include <string.h>
 #include <linux/netlink.h>
 #include <stdint.h>
 #include <unistd.h>
 #include <errno.h>

 #include <linux/audit.h>
 
#define NETLINK_TEST    30
#define MSG_LEN				1250

#define SELF_PORT 100
 
struct user_msg_info
{
	struct nlmsghdr hdr;

	union 
	{
		struct audit_status tatus;
		char  msg[MSG_LEN];
	};
};

static int creat_bind_socket(struct sockaddr_nl *saddr, int protocol)
{
	int skfd;

	/* 创建NETLINK socket */
	skfd = socket(AF_NETLINK, SOCK_RAW, protocol);
	if(skfd == -1)
		return skfd;
	
	
	if(bind(skfd, (struct sockaddr *)saddr, sizeof(*saddr)) != 0)
	{
		close(skfd);
		return -1;
	}

	return skfd;
}

static int send_msg(int skfd, struct sockaddr_nl *daddr, char *buff, int len)
{
	struct nlmsghdr *nlh;
	char *msg;
	size_t nlmsg_len;

	nlmsg_len = NLMSG_SPACE(sizeof(struct nlmsghdr) + len);
	nlh = (struct nlmsghdr *)malloc(nlmsg_len);
	memset(nlh, 0, sizeof(struct nlmsghdr));
	nlh->nlmsg_len = nlmsg_len;
	nlh->nlmsg_flags = 0;
	nlh->nlmsg_type = 0;
	nlh->nlmsg_seq = 0;
	nlh->nlmsg_pid = SELF_PORT;

	msg = (char *) (nlh + 1) ;
	memcpy(msg, buff, len);

	return sendto(skfd, nlh, nlh->nlmsg_len, 0, (struct sockaddr *)daddr, sizeof(struct sockaddr_nl));
}

static int recv_msg(int skfd, struct sockaddr_nl *daddr, struct user_msg_info *msg_info_buf)
{
	// socklen_t len = sizeof(struct sockaddr_nl);

	return recvfrom(skfd, msg_info_buf, sizeof(struct user_msg_info), 0, (struct sockaddr *)daddr, NULL);
}

static void resolve_msg(struct user_msg_info *msg_info)
{
	printf("msg:%s\n",msg_info->msg);
}


int test_recv_audit_muticast()
{
	int skfd;
	int ret;
	struct user_msg_info msg;
	socklen_t len;
	struct sockaddr_nl saddr, daddr;


	memset(&saddr, 0, sizeof(saddr));
	saddr.nl_family = AF_NETLINK; //AF_NETLINK
	saddr.nl_pid = SELF_PORT;
	saddr.nl_groups = 1;

	skfd = creat_bind_socket(&saddr, NETLINK_AUDIT);

	if(skfd <= 0){
		perror("creat and bind socket");
		return -1;
	}

	while (1)
	{
		memset(&msg, 0, sizeof(msg));

		ret = recv_msg(skfd, NULL, &msg);
		printf("recv msg(%d): ", ret);
		if(ret <= 0)
		{
			perror("recv massage form kernel");
			return ret;
		}
		resolve_msg(&msg);
	}
}

int test_mynetlink()
{
	int skfd;
	int ret;
	struct user_msg_info msg;
	socklen_t len;
	struct sockaddr_nl saddr, daddr;

	char *umsg = "I am massage from user space!!";

	memset(&saddr, 0, sizeof(saddr));
	saddr.nl_family = AF_NETLINK; //AF_NETLINK
	saddr.nl_pid = SELF_PORT;
	saddr.nl_groups = 0;

	memset(&daddr, 0, sizeof(daddr));
	daddr.nl_family = AF_NETLINK;
	daddr.nl_pid = 0; // to kernel 
	daddr.nl_groups = 0;

	skfd = creat_bind_socket(&saddr, NETLINK_TEST);
	if(skfd <= 0){
		perror("creat and bind socket");
		return -1;
	}

	ret = send_msg(skfd, &daddr, umsg, strlen(umsg) + 1);
	if(!ret){
		perror("send");
		return -1;
	}

	memset(&msg, 0, sizeof(msg));
	ret = recv_msg(skfd, NULL, &msg);
	if(ret <= 0)
	{
		perror("recv massage form kernel");
		return ret;
	}
	resolve_msg(&msg);
	return 0;
}

int test_send_audit()
{
	int skfd;
	int ret;
	struct user_msg_info msg = {};
	socklen_t len;
	struct sockaddr_nl saddr, daddr;
	struct nlmsghdr *nlh;

	char *umsg = "I am audit message from user sapce!!";

	memset(&saddr, 0, sizeof(saddr));
	saddr.nl_family = AF_NETLINK; //AF_NETLINK
	saddr.nl_pid = SELF_PORT;
	saddr.nl_groups = 0;

	memset(&daddr, 0, sizeof(daddr));
	daddr.nl_family = AF_NETLINK;
	daddr.nl_pid = 0; // to kernel 
	daddr.nl_groups = 0;

	skfd = creat_bind_socket(&saddr, NETLINK_AUDIT);

	if(skfd <= 0){
		perror("creat and bind socket");
		return -1;
	}

	nlh = &msg.hdr;
	nlh->nlmsg_len = NLMSG_SPACE(sizeof(struct nlmsghdr) + strlen(umsg) + 1);
	nlh->nlmsg_flags = 0;
	nlh->nlmsg_type = AUDIT_USER;
	nlh->nlmsg_seq = 0;
	nlh->nlmsg_pid = getpid();

	memcpy(&msg.msg, umsg, strlen(umsg)+1);

	return sendto(skfd, nlh, nlh->nlmsg_len, 0, (struct sockaddr *)&daddr, sizeof(struct sockaddr_nl));

	// struct audit_status *audit_status;
	// nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PLOAD));
	// memset(nlh, 0, sizeof(struct nlmsghdr));
	// nlh->nlmsg_len = NLMSG_SPACE(MAX_PLOAD);
	// nlh->nlmsg_flags = 0;
	// nlh->nlmsg_type = AUDIT_SET;
	// nlh->nlmsg_seq = 0;
	// nlh->nlmsg_pid = saddr.nl_pid; //self port

	// audit_status = (struct audit_status *) (nlh + 1) ;
	// audit_status->mask = AUDIT_STATUS_PID;
	// audit_status->pid = getpid();
	//  memcpy(NLMSG_DATA(nlh), umsg, strlen(umsg));

	//  printf("send kernel:%s\n", umsg);
}

int main(int argc, char **argv)
{
	// return test_recv_audit_muticast();
	test_send_audit();

	return test_mynetlink();

     return 0;
}




