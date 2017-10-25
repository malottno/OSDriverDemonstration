#include <linux/kernel.h>
#include <linux/module.h>

int init_module(void){
	printk(KERN_ALERT "Entering Network Driver\n");
	//Entering code

	return 0;
}

void cleanup_module(void){
	//Closing code
	printk(KERN_ALERT "Closing Network Driver\n");
	return;
}
