#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

int init_module(void){
	printk(KERN_ALERT "Entering Block Driver\n");
	//Entering code

	return 0;
}

void cleanup_module(void){
	//Closing code
	printk(KERN_ALERT "Closing Block Driver\n");
	return;
}

