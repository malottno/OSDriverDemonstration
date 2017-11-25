#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <net/arp.h>
#include <linux/ip.h>

#define ARP_THA_OFFSET 18
#define ARP_SHA_OFFSET 8

MODULE_AUTHOR("Nick Malott");
MODULE_DESCRIPTION("Network device driver demonstration module");
MODULE_VERSION("0.1");
MODULE_LICENSE("GPL");

struct net_device *netDevice;
struct iphdr *ih;
struct data *d_priv;


//Function Prototypes (and definition of net_open, net_stop)
int net_open(struct net_device *dev) { netif_start_queue(dev); return 0; }
int net_stop(struct net_device *dev) { netif_stop_queue(dev); return 0; }
int net_start_xmit(struct sk_buff *skb, struct net_device *dev);
struct net_device_stats *net_stats(struct net_device *dev);
int net_header(struct sk_buff *skb, struct net_device *dev, unsigned short type, const void *daddr, const void *saddr, unsigned int len);



//Structures

// os_packet : stores
struct os_packet {
	struct net_device *dev;
	int datalen;
	u8 data[ETH_DATA_LEN];
};

// data : stores 
struct data{
	struct net_device_stats stats;
	struct sk_buff *skb;
	struct os_packet *pkt;
	struct net_device *dev;
};





//Functions

/*
 *	Handles transmission of data to the network interface
 */
static void os_tx_i_handler(struct net_device *dev){
	d_priv = netdev_priv(dev);
	if(netif_queue_stopped(d_priv->pkt->dev)) netif_wake_queue(d_priv->pkt->dev);
	return;
}


/*
 *	Handles receiving of data from the network interface
 */
static void os_rx_i_handler(struct net_device *dev){
	d_priv = netdev_priv(dev);
	//d_priv->stats.rx_bytes = 15;
	d_priv->skb = dev_alloc_skb(d_priv->pkt->datalen);
	memcpy(skb_put(d_priv->skb, d_priv->pkt->datalen), d_priv->pkt->data, d_priv->pkt->datalen);

	d_priv->skb->dev = dev;
	d_priv->skb->protocol = eth_type_trans(d_priv->skb,dev);

	netif_rx(d_priv->skb);

	if(netif_queue_stopped(d_priv->pkt->dev)) netif_wake_queue(d_priv->pkt->dev);
}


int net_start_xmit(struct sk_buff *skb, struct net_device *dev){
	char tmp[10];
	struct arphdr *arp_header;	
	u32 *saddr;
	u32 *daddr;	
	char *data = skb->data;
	int len = skb->len;
	d_priv = netdev_priv(dev);
	d_priv->skb = skb;


	ih = (struct iphdr *)(data+sizeof(struct ethhdr));

	if(skb->protocol == htons(ETH_P_ARP)){
		skb->protocol = htons(ARPOP_REPLY);
		arp_header = arp_hdr(skb);

		memcpy(tmp, (void *)arp_header + ARP_SHA_OFFSET, 10);
		memcpy((void *)arp_header + ARP_SHA_OFFSET, (void*)arp_header + ARP_THA_OFFSET, 10);
		memcpy((void *)arp_header + ARP_THA_OFFSET, tmp, 10);

		memcpy((u32*)ih->daddr, dev->dev_addr, dev->addr_len);
		
		skb->dev = dev;

		skb->protocol = eth_type_trans(skb, dev);
		ih->check = 0;
		ih->check = ip_fast_csum((unsigned char *)ih, ih->ihl);

		netif_rx(skb);
	}

	saddr = &ih->saddr;
	daddr = &ih->daddr;
	((u8*)saddr)[2] ^= 1;	//Switches the 3rd segment of the address
	((u8*)daddr)[2] ^= 1;	//for both source and destination	

	ih->check = 0;
	ih->check = ip_fast_csum((unsigned char *)ih, ih->ihl);
	


	if(daddr[2] == 1){
		d_priv = netdev_priv(netDevice);
		d_priv->pkt->datalen = len;
		memcpy(d_priv->pkt->data,data,len);
		os_rx_i_handler(netDevice);
		os_tx_i_handler(netDevice);
		d_priv = netdev_priv(netDevice);
		dev_kfree_skb(d_priv->skb);
	}
	else if(saddr[2] == 0){
		d_priv = netdev_priv(netDevice);
		d_priv->pkt->datalen = len;
		memcpy(d_priv->pkt->data,data,len);
		os_rx_i_handler(netDevice);
		os_tx_i_handler(netDevice);
		d_priv = netdev_priv(netDevice);
		dev_kfree_skb(d_priv->skb);
	}
	else{printk("Failed to initialize tx/rx interrupt handlers\n");}
	

	return 0;
}

struct net_device_stats *net_stats(struct net_device *dev){
	//struct net_device_stats local = ((struct data*)netdev_priv(dev))->stats;
	//local.rx_packets = 99;
	//local.rx_bytes = 99;
	
	//return &local;

	return &(((struct data*)netdev_priv(dev))->stats);
}

int net_header(struct sk_buff *skb, struct net_device *dev, unsigned short type, const void *daddr, const void *saddr, unsigned int len){
	struct ethhdr *eth = (struct ethhdr*)skb_push(skb,ETH_HLEN);

	/*copy the address given by the device to both source and
   		destination fields (from argument 'dev')*/
	memcpy(eth->h_source, dev->dev_addr, dev->addr_len);
	memcpy(eth->h_dest, eth->h_source, dev->addr_len);

	if(skb->protocol == htons (ETH_P_ARP)) {
		memcpy(eth->h_dest, eth->h_source, dev->addr_len);
		memcpy(eth->h_source, dev->dev_addr, ETH_ALEN);
		eth->h_dest[ETH_ALEN-1] = (eth->h_dest[ETH_ALEN-1] == 5) ? 6: 5;
		eth->h_proto = htons(type);
		return dev->hard_header_len;
	}

	eth->h_dest[ETH_ALEN-1] = (eth->h_dest[ETH_ALEN-1] == 5) ? 6: 5;

	//set the protocol field (from argument 'type')
	eth->h_proto = htons(type);

	return dev->hard_header_len;
}

static const struct net_device_ops nops = {
	.ndo_open=net_open,
	.ndo_stop=net_stop,
	.ndo_start_xmit = net_start_xmit,
	.ndo_get_stats = net_stats,
};

static const struct header_ops hops = {
	.create = net_header
};

int init_module(void){
	int i;
	int result;
	printk(KERN_ALERT "Entering Network Driver\n");
	//Entering code
	netDevice = alloc_etherdev(sizeof(struct data));

	netDevice->netdev_ops = &nops;
	netDevice->header_ops = &hops;

	printk(KERN_ALERT "Entering Network Driver2\n");
	for(i=0;i<6;i++) netDevice->dev_addr[i]=(unsigned char)i;

	for(i=6;i>0;i--) netDevice->broadcast[i]=(unsigned char)i;
	
	printk(KERN_ALERT "Device Address: %d:%d\n",netDevice->dev_addr[0], netDevice->dev_addr[1]);

	netDevice->hard_header_len=14;

	strcpy(netDevice->name, "NetDevEx");
	netDevice->flags |= IFF_NOARP;
	printk(KERN_ALERT "Registering\n");
	
	if((result = register_netdev(netDevice))){
		printk(KERN_ALERT "Registration failed...\n");
	
		return result;
	}
	

	return 0;
}

void cleanup_module(void){

	//Closing code
	printk(KERN_ALERT "Closing Network Driver\n");

	if(netDevice){
		d_priv = netdev_priv(netDevice);
		kfree(d_priv->pkt);
		unregister_netdev(netDevice);
	}

	return;
}
