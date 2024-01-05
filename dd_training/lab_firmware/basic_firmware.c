#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/firmware.h>

#define FWFILE "first_fwfile"

static const struct firmware *fw;

static void my_release(struct device *dev)
{
        dev_info(dev, "releasing firmware device\n");
}
static struct device dev = {
	.release=my_release
};

static __init int lfw_init(void)
{
       int ret, ref;

       dev_set_name(&dev, "first firmware device");

       ret = device_register(&dev);
       if(ret < 0) {
	       pr_alert("Failed to register the firmwarei device\n");
	       return ret;
       }
       dev_info(&dev,"Firmware device inserted: bearing a charmed life.\n");
       // 
        ref = request_firmware(&fw, FWFILE, &dev);
       if(ref) {
                pr_err("requesting firmware failed\n");
                device_unregister(&dev);
		return ref;
       }
           
       dev_info(&dev,"firmware contents=\n%s\n", fw->data);
       
       return 0;
}
static __exit void ulfw_exit(void)
{
	release_firmware(fw);
	dev_alert(&dev, "Firmware unloaded: Out, out, brief candle!\n");
	device_unregister(&dev);

}

module_init(lfw_init);
module_exit(ulfw_exit);

MODULE_FIRMWARE(FWFILE);
MODULE_LICENSE("GPL");

