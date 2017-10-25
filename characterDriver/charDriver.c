#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

int init_module(void){
	printk(KERN_ALERT "Entering Character Driver\n");
	//Entering code

	return 0;
}

void cleanup_module(void){
	//Closing code
	printk(KERN_ALERT "Closing Character Driver\n");
	return;
}

//module_init(characterDriver);
//module_exit(characterDriver_exit);
