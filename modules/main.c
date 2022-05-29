
#include <linux/module.h>

#include <linux/audit.h>

#define AUDIT_MB_KERNEL     1456


void mb_audit_log(void)
{       
 audit_log(current->audit_context, GFP_KERNEL, AUDIT_MB_KERNEL,               
                 "mb ==> session=%u\n",                                    
                 audit_get_sessionid(current));
}


int __init init_main(void)
{
	int error = 0;

	printk("Hello module!\n");
	mb_audit_log();
	
	return error;
}


void __exit exit_main(void)
{

	printk("Goodbye module!\n");
}


module_init(init_main);
module_exit(exit_main);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liuqi");
MODULE_VERSION("v0.1");
MODULE_DESCRIPTION("SMZY SECURITY");
