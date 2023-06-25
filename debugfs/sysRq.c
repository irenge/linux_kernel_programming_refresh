#include <linux/sysrq.h>
#include <linux/init.h>
#include <linux/module.h>

#define SYSRQ 'x'

static void example_sysrq_handler(int key)
{

	pr_info("Hello from function %s\n", __func__);
	pr_info("Key pressed was '%c'\n", key);

}

static struct sysrq_key_op defined_sysrq_op = {

	.handler = example_sysrq_handler,
	.help_msg = "x pressed help",
	.action_msg = "X pressed action message"
};

static int __init my_init(void)
{
	int rc = register_sysrq_key(SYSRQ, &defined_sysrq_op);
	if (rc == 0)
		pr_info("sysrq key registered");
	else
		pr_err("sysrq key failed to register");
	return rc;
}

static void __exit my_exit(void)
{
	int rc = unregister_sysrq_key(SYSRQ, &defined_sysrq_op);

	if (rc == 0)
		pr_info("sysrq key unregistered\n");
	else
		pr_err("sysrq key failed to unregister\n");
}



module_init(my_init);
module_exit(my_exit);


MODULE_AUTHOR("Jules");
MODULE_LICENSE("GPL v2");


