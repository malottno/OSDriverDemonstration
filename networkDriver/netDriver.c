#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>

MODULE_LICENSE("GPL");
struct net_device *netDevice;
int net_open(struct net_device *dev) { netif_start_queue(dev); return 0; }
int net_stop(struct net_device *dev) { netif_stop_queue(dev); return 0; }
int net_start_xmit(struct sk_buff skb, struct net_device *dev){return 0;}
struct net_device_stats *net_stats(struct net_device *dev){return;}
int net_header(struct sk_buff skb, struct net_device *dev, unsigned short type, const void *daddr, const void *saddr, unsigned int len){return;}

struct data{
	struct net_device_stats stats;
	struct sk_buff *skb;
	struct os_packet *pkt;
	struct net_device *dev;
};

static const struct net_device_ops nops = {
	.ndo_open = net_open,
	.ndo_stop = net_stop,
	.ndo_start_xmit = net_start_xmit,
	.ndo_get_stats = net_stats,
};

static const struct header_ops hops = {
	.create = net_header
};

int init_module(void){
	int i;
	printk(KERN_ALERT "Entering Network Driver\n");
	//Entering code
	netDevice = alloc_etherdev(sizeof(struct data));

	netDevice->netdev_ops = &nops;
	netDevice->header_ops = &hops;

	for(i=0;i<6;i++) netDevice->dev_addr[i]=(unsigned char)i;

	for(i=6;i>0;i--) netDevice->broadcast[i]=(unsigned char)i;

	netDevice->hard_header_len=14;

	memcpy(netDevice->name, "NetDevEx\0",9);
	netDevice->flags |= IFF_NOARP;
	

	return 0;
}

void cleanup_module(void){
	struct data *d;

	//Closing code
	printk(KERN_ALERT "Closing Network Driver\n");

	if(netDevice){
		d = netdev_priv(netDevice);
		kfree(d->pkt);
		unregister_netdev(netDevice);
	}

	return;
}
