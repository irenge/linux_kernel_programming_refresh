#include<linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>


static int ex_probe(struct platform_device *pdev) 
{
	pr_info("Inside %s\n", __func__);
	return 0;
}
static int ex_remove(struct platform_device *pdev)
{
	pr_info("Inside %s\n", __func__);
	return 0;
}

static const struct platform_device_id ex_id_table[] = {
	{"example_platform_1", 0},
	{"example_platform_2", 0},
	{},
};

static struct platform_driver ex_driver{
	.driver = {
		.name = "example-plarform",
		.owner = THIS_MODULE, 
	},
		.probe = ex_probe,
		.remove = ex_remove,
		id_table = ex_id_table,
};

module_platform_driver(ex_driver);

static __init int platf_init(void) {
	pr_info();
	return 0;
}

static __exit void platf_exit(void) {
}

module_init(platf_init);
module_exit(platf_exit);

MODULE_AUTHOR("Jules");
MODULE_DESCRIPTION("Learning platform");
MODULE_LICENSE("GPL v2");

