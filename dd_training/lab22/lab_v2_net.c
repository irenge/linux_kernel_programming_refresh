#include<linux/module.h>
#include <linux/etherdevice.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include<linux/netdevice.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/etherdevice.h>
#include <linux/in.h>
#include <linux/skbuff.h>


struct net_device *my_net;

//struct my_priv *priv;

struct my_priv {
	struct net_device_stats *stats;
	int status;
	int pkt_len;
	u8 *pkt_data;
	struct sk_buff *skb;
	spinlock_t lock;
};

struct my_priv *priv;

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


void mynet_hw_tx(char *buf, int len, struct net_device *dev) {
	struct net_device *dest;
//	struct my_priv *priv;

	if(len  < sizeof(struct ethhdr) + sizeof(struct iphdr)) {
		pr_info("\nmynet: Hmm... packet too short (%i octets)\n", len);
		return;
	}

	dest = my_net;
	priv = netdev_priv(dev);

	priv->pkt_len = len;
	priv->pkt_data = buf;
	
	pr_info("\nmynet tx \n");
	dev_kfree_skb(priv->skb);
}

void mynet_rx(struct net_device *dev, int len, unsigned char *buf, struct my_priv *priv) {
	struct sk_buff *skb;
	
	skb = dev_alloc_skb(len + 2);
	skb_reserve(skb, 2);
	memcpy(skb_put(skb, len), buf, len);
	skb->dev = dev;
	skb->protocol = eth_type_trans(skb, dev);
	skb->ip_summed = CHECKSUM_UNNECESSARY;
	
	priv->stats->rx_packets++;
	netif_rx(skb);
	return;
}

static int net_transmit(struct sk_buff *skb, struct net_device *dev) {

	int len; 
	char *data;
	int i;
	static int pkts;

	struct my_priv *priv = netdev_priv(dev);

	len = skb->len;
	data = skb->data;
	netif_trans_update(dev);

	priv->skb = skb;

	mynet_hw_tx(data, len, dev);

	return 0;
}

static struct net_device_stats *mynet_stats(struct net_device *dev) {
	netdev_info(dev, "Hit: %s(%s)\n", __func__, dev->name);
	return &dev->stats;
}


static struct net_device_ops ndo = {
	.ndo_open = ndo_open,
	.ndo_stop = ndo_stop,
	.ndo_start_xmit = net_transmit,
//	.ndo_get_stats = mynet_stats,
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


//	stats = &dev->stats;
//
//	stats->collisions = 50;

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
