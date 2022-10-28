#include <linux/module.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/sysrq.h>

#define MY_SYSRQ_KEY 'x'

static void my_sysrq_handler(int key) 
{
	pr_info("Hello from function %s\n", __func__);
	pr_info("Key pressed was '%c'\n", key);
}

static struct sysrq_key_op my_sysrq_key_op = {
	.handler = my_sysrq_handler,
        .help_msg = "e(X)ample",
        .action_msg = "eXample action message",
};

static int __init my_init(void)
{
        int rc = register_sysrq_key(MY_SYSRQ_KEY, &my_sysrq_key_op);

        if (rc == 0)
                pr_info("sysrq key registered\n");
        else
                pr_err("sysrq key failed to register\n");
        return rc;
}


static void __exit my_exit(void)
{
        int rc = unregister_sysrq_key(MY_SYSRQ_KEY, &my_sysrq_key_op);

        if (rc == 0)
                pr_info("sysrq key unregistered\n");
        else
                pr_err("sysrq key failed to unregister\n");
}

module_init(my_init);
module_exit(my_exit);


MODULE_AUTHOR("Jules");
MODULE_LICENSE("GPL v2");

