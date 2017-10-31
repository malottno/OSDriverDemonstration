
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/mutex.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/types.h>

MODULE_LICENSE("GPL");

int characterCount = 0;
#define BUF_LEN 80

static char data[BUF_LEN];
static char* data_ptr;

static struct cdev* charDevice;
static struct class *charClass = NULL;
static DEFINE_MUTEX(charLock);
static char *inFile = "/dev/tty10";
static char *outFile = "./driverOutput";

module_param(inFile,charp,0000);
module_param(outFile,charp,0000);

MODULE_PARM_DESC(inFile, "File path for input to character driver");
MODULE_PARM_DESC(outFile, "File path for output from character driver");

static int major = 0;
static const char device[] = "charDriver-Example"; 


//Function Protoypes
static ssize_t c_read(struct file *f, char __user *buf, size_t len, loff_t *off);
static ssize_t c_write(struct file *f, const char __user *buf, size_t len, loff_t *off);
static int c_open(struct inode *i, struct file *f);


//Functions
static ssize_t c_read(struct file *f, char __user *buf, size_t len, loff_t *off){
	ssize_t retVal = 0;
	int b_read = 0;

	printk(KERN_INFO "charDriver: Read()\n");

	sprintf(data, "This is a test of %d bytes read\n", b_read);

	while(len && *data_ptr){
		put_user(*(data_ptr++), buf++);
		len--;
		b_read++;
	}


	return b_read;
}

static ssize_t c_write(struct file *f, const char __user *buf, size_t len, loff_t *off){
	ssize_t retVal = 0;
	printk(KERN_INFO "charDriver: Write()\n");
	return retVal;
}

static int c_open(struct inode *i, struct file *f){
	int retVal = 0;
	printk(KERN_INFO "\tcharDriver: opened");

	try_module_get(THIS_MODULE);

	return retVal;
}

static struct file_operations fops={
	.owner=THIS_MODULE,
	.read=c_read,
	.write=c_write,
	.open=c_open
};

int init_module(void){
	void *ptr_error;
	//int result = 0;
	struct class *device_class;
	dev_t DEV_T;

	printk(KERN_ALERT "Entering Character Driver\n");
	major = register_chrdev(0, device, &fops);


	if(major < 0){
		printk(KERN_INFO "Register Failed! : %d\n", major);
		return major;
	}
	
   	printk("'mknod /dev/hello c %d 0'.\n", major);
	
	//Entering code
	DEV_T = MKDEV(major,1);
	device_class = class_create(THIS_MODULE, "charDevice");

	if(IS_ERR(ptr_error)){
		class_destroy(device_class);
		unregister_chrdev(0,"charDevice");
	}

	ptr_error = device_create(device_class, NULL, DEV_T, NULL, "charDevice");


	printk(KERN_ALERT "\tinFile: %s\n",inFile);
	printk(KERN_ALERT "\toutFile: %s\n", outFile);
	printk(KERN_ALERT "\tmajor: %d\n", major);
	printk(KERN_ALERT "\tdevice: %s\n",device);
	


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

//module_init(init_module);
//module_exit();
