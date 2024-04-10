#include<linux/module.h>
#include<linux/netdevice.h>
#include <linux/init.h>
#include <linux/version.h>


struct net_device *my_net;
struct my_priv *priv;

static int ndo_open(struct net_device *dev) {
	netdev_info(dev, "Hit: %s(%s)\n", __func__, dev->name);
	/* start up the transmission queue */
	netif_start_queue(dev);
	return 0;
}

static int ndo_stop(struct net_device *dev) {
	netdev_info(dev, "Hit: %s(%s)\n", __func__, dev->name);
	/* shut down the transmission queue */
	netif_stop_queue(dev);
	return 0;
}

/* Note this method is only needed on some; without it
 *  module will fail upon removal or use. At any rate there
 *  is a memory leak whenever you try to send a packet
 *  through in any case
 */

static int stub_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
        dev_kfree_skb(skb);
        return 0;
}


static struct net_device_ops ndo = {
	.ndo_open = ndo_open,
	.ndo_stop = ndo_stop,
	.ndo_start_xmit = stub_start_xmit,
};

static void mynet_setup(struct net_device *dev)
{
	
	unsigned int i;
	int j;
        char phony[ETH_ALEN];
        
	netdev_info(dev, "%s(%s)\n", __func__, dev->name);
	pr_info("\nMac address length is %d\n", ETH_ALEN);

	/* Fill in the MAC address with a phoney */
        for (j = 0; j < ETH_ALEN; ++j) {
		 get_random_bytes(&i, sizeof(int)); //fill the entire bitspace of the int instead of only one byte
	         i = i % 15;

		 pr_info("\ni = %d\n", i);
		 phony[j] = (char)i;
	}

	dev_addr_mod(dev, 0, phony, ETH_ALEN);

	ether_setup(dev);
	dev->netdev_ops = &ndo;

}

static int __init net_load(void){ 

	/* Device registration */

	my_net = alloc_netdev(0, "mynet%d", NET_NAME_ENUM, mynet_setup);
	if(!my_net)
		return -ENOMEM;
	if(register_netdev(my_net))
		pr_info("Error in registering device %s\n", my_net->name);
	netdev_info(my_net, "Succeeded in loading %s!\n\n", dev_name(&my_net->dev));
        return 0;

}

static void __exit net_unload(void) {
	netdev_info(my_net, "Unloading stub network module\n\n");
	unregister_netdev(my_net);
	free_netdev(my_net);

}

module_init(net_load);
module_exit(net_unload);

MODULE_AUTHOR("Jules Irenge");
MODULE_AUTHOR("Jerry Cooperstein");
MODULE_AUTHOR("Tatsuo Kawasaki");
MODULE_DESCRIPTION("Intro network");
MODULE_LICENSE("GPL v2");
