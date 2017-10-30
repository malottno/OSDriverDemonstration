
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/mutex.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");

int characterCount = 0;

//static struct cdev charDevice;
static DEFINE_MUTEX(charLock);
static char *inFile = "/dev/tty10";
static char *outFile = "./driverOutput";

module_param(inFile,charp,0000);
module_param(outFile,charp,0000);

MODULE_PARM_DESC(inFile, "File path for input to character driver");
MODULE_PARM_DESC(outFile, "File path for output from character driver");

static int major = 0;
static const char device[] = "charDriver-Example"; 
static ssize_t c_read(struct file *f, char __user *buf, size_t len, loff_t *off);
static ssize_t c_write(struct file *f, const char __user *buf, size_t len, loff_t *off);
static int c_open(struct inode *i, struct file *f);

static ssize_t c_read(struct file *f, char __user *buf, size_t len, loff_t *off){
	ssize_t retVal = 0;
	printk(KERN_INFO "charDriver: Read()\n");
	return retVal;
}

static ssize_t c_write(struct file *f, const char __user *buf, size_t len, loff_t *off){
	ssize_t retVal = 0;
	printk(KERN_INFO "charDriver: Write()\n");
	return retVal;
}

static int c_open(struct inode *i, struct file *f){
	int retVal = 0;
	printk(KERN_INFO "\tcharDriver: opened");
	return retVal;
}


static struct file_operations fops={
	.owner=THIS_MODULE,
	.read=c_read,
	.write=c_write,
	.open=c_open
};

int init_module(void){
	printk(KERN_ALERT "Entering Character Driver\n");
	major = register_chrdev(0, device, &fops);
	//Entering code
	printk(KERN_ALERT "\tinFile: %s\n",inFile);
	printk(KERN_ALERT "\toutFile: %s\n", outFile);


	return 0;
}





void cleanup_module(void){
	//Closing code
	printk(KERN_ALERT "Closing Character Driver\n");
	if(major != 0){
		unregister_chrdev(major, device);
	}
	return;
}

//module_init(characterDriver);
//module_exit(characterDriver_exit);
