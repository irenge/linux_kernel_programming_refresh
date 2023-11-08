#include<linux/module.h>
#include<linux/netdevice.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/etherdevice.h>

struct net_device *my_net;
struct my_priv *priv;
static struct net_device_stats *stats;

static int ndo_open(struct net_device *dev) {
	netdev_info(dev, "Hit: %s(%s)\n", __func__, dev->name);
	//	memcpy(dev->dev_addr, "\0SNULx", ETH_ALEN);
	/* start up the transmission queue */
	netif_start_queue(dev);
	return 0;
}

static int ndo_stop(struct net_device *dev) {
	netdev_info(dev, "Hit: %s(%s)\n", __func__, dev->name);
	/* shut down the transmission queue */
	netif_stop_queue(dev); /* release ports, irq */
	return 0;
}

/* Note this method is only needed on some; without it
 *  module will fail upon removal or use. At any rate there
 *  is a memory leak whenever you try to send a packet
 *  through in any case
 */

static void printline(struct net_device *dev, unsigned char *data, int n)
{
	char line[256], entry[16];
	int j;

	strcpy(line, "");
	for (j = 0; j < n; j++) {
		sprintf(entry, " %2x", data[j]);
		strcat(line, entry);
	}
	netdev_info(dev, "%s\n", line);
}


static void net_receive(struct sk_buff *skb, struct net_device *dev) {
//	static int pkts;

	netdev_info(dev, "\n packets received \n");
	stats->rx_bytes += skb->len;
	++stats->rx_packets;
	netif_rx(skb);
}


static int net_transmit(struct sk_buff *skb, struct net_device *dev) {
	int len; 
	char *data;
	int i;
	static int pkts;

	//struct my_data *priv = netdev_priv(dev);

	len = skb->len;
	data = skb->data;
	netif_trans_update(dev);

	netdev_info(dev, "\n%d packets sent \n", pkts);
	++pkts;
	/* print out 16 bytes per line */

	for (i = 0; i < skb->len; i += 16)
		printline(dev, &skb->data[i],
				(skb->len - i) < 16 ? skb->len - i : 16);

	netdev_info(dev, "\n");
	/* so we can free it in interrupt routine */
	//priv->skb = skb; /* remove to allow the loop back net_receive

	//mynet_hw_tx(data, len, dev);
	//dev_kfree_skb(skb);

	stats->tx_bytes += skb->len;
	++stats->tx_packets;

	/* loop back */
	net_receive(skb, dev);

	return 0;
}

/*
   static void mynet_rx(struct net_device *dev, int len, unsigned char *buf) {
   struct 
   */

static struct net_device_stats *mynet_stats(struct net_device *dev) {
	 netdev_info(dev, "Hit: %s(%s)\n", __func__, dev->name);
	 return &dev->stats;
}


static struct net_device_ops ndo = {
	.ndo_open = ndo_open,
	.ndo_stop = ndo_stop,
	.ndo_start_xmit = net_transmit,
	.ndo_get_stats = mynet_stats,
};

void get_mac(struct net_device *dev, char *mac, int len) {

	unsigned int i;
	int j;

	netdev_info(dev, "\nMac address length is %d\n", len);

	// Fill in the MAC address with random number 
	for (j = 0; j < len; ++j) {
		get_random_bytes(&i, sizeof(int)); //fill the entire bitspace of the int instead of only one byte
		i = i % 15;

		netdev_info(dev, "\ni = %d\n", i);
		mac[j] = (char)i;
	}

	dev_addr_mod(dev, 0, mac, len);

}


static void mynet_setup(struct net_device *dev)
{

	char phony[ETH_ALEN];

	netdev_info(dev, "%s(%s)\n", __func__, dev->name);

	get_mac(dev, phony, ETH_ALEN);

	//dev_addr_mod(dev, 0, phony, ETH_ALEN);

	//eth_hw_addr_random(dev);

	ether_setup(dev);
	dev->netdev_ops = &ndo;
	dev->flags |= IFF_NOARP;

	//	eth_hw_addr_random(dev);

	//dev->min_mtu = 10;
	//dev->max_mtu = 30;


	stats = &dev->stats;

	stats->collisions = 50;

}

static int __init net_load(void){ 

	/* Device registration */

	int ret;

	my_net = alloc_netdev(0, "mynet%d", NET_NAME_ENUM, mynet_setup);

	if(!my_net)
		return -ENOMEM;

	ret = register_netdev(my_net);

	if(ret) {
		pr_err("Error in registering device %s\n", my_net->name);
		free_netdev(my_net);
		return ret;
	}

	netdev_info(my_net, "Succeeded in loading %s!\n\n", dev_name(&my_net->dev));

	return 0;

}

static void __exit net_unload(void) {
	
	netdev_info(my_net, "<0> Cleaning Up the network module\n");
	unregister_netdev(my_net);
	free_netdev(my_net);


}

module_init(net_load);
module_exit(net_unload);

MODULE_AUTHOR("Jules Irenge");
MODULE_DESCRIPTION("Simple Network driver");
MODULE_LICENSE("GPL v2");
