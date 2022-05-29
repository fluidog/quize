/**
 * @file kernel.c
 * @author fluidog (fluidog@163.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */
 
#include <linux/audit.h>

#include <linux/types.h>
#include <net/sock.h>
#include <linux/netlink.h>

#include <linux/module.h>

#define NETLINK_TEST     30

struct sock *nlsk = NULL;
extern struct net init_net;

void audit_test(void);

int send_usrmsg( unsigned int port, char *pbuf, size_t len)
{
	struct sk_buff *nl_skb;
	struct nlmsghdr *nlh;

	/* 创建sk_buff 空间 */
	nl_skb = nlmsg_new(len, GFP_ATOMIC);
	if(!nl_skb)
	{
		printk("netlink alloc failure\n");
		return -1;
	}

	/* 设置netlink消息头部 */
	nlh = nlmsg_put(nl_skb, 0, 0, NETLINK_TEST, len, 0);
	if(nlh == NULL)
	{
		printk("nlmsg_put failaure \n");
		nlmsg_free(nl_skb);
		return -1;
	}

	/* 拷贝数据发送 */
	memcpy(nlmsg_data(nlh), pbuf, len);

	return  netlink_unicast(nlsk, nl_skb, port, MSG_DONTWAIT);  
}

static void netlink_rcv_msg(struct sk_buff *skb)
{
	struct nlmsghdr *nlh = NULL;
	char *umsg = NULL;
	char *kmsg = "I am message form kernel!!!";

	if(skb->len >= nlmsg_total_size(0))
	{
		nlh = nlmsg_hdr(skb);
		umsg = NLMSG_DATA(nlh);
		if(umsg)
		{
			printk("recv from user(%d): %s\n", nlh->nlmsg_pid, umsg);
			send_usrmsg(nlh->nlmsg_pid, kmsg, strlen(kmsg));
		}
	}
}

struct netlink_kernel_cfg cfg = { 
	.input  = netlink_rcv_msg, /* set recv callback */
};

int test_netlink_init(void)
{
	/* create netlink socket */
	struct net *net;

	net = get_net_ns_by_pid(pid_vnr(task_pid(current)));
	// net = &init_net;

	nlsk = (struct sock *)netlink_kernel_create(net, NETLINK_TEST, &cfg);
	if(nlsk == NULL)
	{   
		printk("netlink_kernel_create error !\n");
		return -1; 
	}
	printk("test_netlink_init\n");

	// audit_test();

	return 0;
}

void test_netlink_exit(void)
{
	if (nlsk){
		netlink_kernel_release(nlsk); /* release ..*/
		nlsk = NULL;
	}   
	printk("test_netlink_exit!\n");
}

void audit_test(void)
{
	//struct audit_buffer *ab;
	//char comm[sizeof(current->comm)];

	audit_log(audit_context(), GFP_ATOMIC, AUDIT_AVC, "avc:  denied { read write 0x23} for pid=145 comm=fakeexec scontext=fakescontext tcontext=fakescontext tclass=faketclass permissive=1");
}

module_init(test_netlink_init);
module_exit(test_netlink_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liuqi");
MODULE_VERSION("v0.1");
MODULE_DESCRIPTION("MY TEST MODULE");
