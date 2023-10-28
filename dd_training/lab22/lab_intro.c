#include<linux/module.h>
#include<linux/netdevice.h>

struct net_device *my_net;
struct my_priv *priv;

int ndo_open(struct net_device *dev);
int ndo_stop(struct net_device *dev);

static struct net_device_ops ndo = {
	.ndo_open = my_open,
	.ndo_stop = my_stop
};


static void mynet_setup(struct net_device *dev)
{
	ether_setup(dev);
	dev->netdev_ops = &ndo;

}

static int __init net_load(void){

priv = netdev_priv(my_net);

my_net = alloc_netdev(sizeof(priv), "mynet%d", NET_NAME_ENUM, mynet_setup);

register_netdev(my_net);






}


