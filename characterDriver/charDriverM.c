#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/mutex.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/semaphore.h>

#define BUF_LEN 500

static char *inFile = "/dev/tty10";
static char *outFile = "./driverOutput";

module_param(inFile,charp,0000);
module_param(outFile,charp,0000);

MODULE_PARM_DESC(inFile, "File path for input to character driver");
MODULE_PARM_DESC(outFile, "File path for output from character driver");
MODULE_AUTHOR("Nick Malott");
MODULE_DESCRIPTION("Character device driver demonstration module");
MODULE_VERSION("0.1");
MODULE_LICENSE("GPL");

static char dataBuf[BUF_LEN] = {0};
static char* data_ptr;
int characterCount = 0;
static struct semaphore charSem;

static struct cdev* charDevice;
static struct class *charClass = NULL;
static int major = 0;
static const char device[] = "charDriver-Example"; 


//Function Protoypes
static ssize_t c_read(struct file *f, char __user *buf, size_t len, loff_t *off);
static ssize_t c_write(struct file *f, const char __user *buf, size_t len, loff_t *off);
static int c_open(struct inode *i, struct file *f);
static int c_release(struct inode *i, struct file *f){
	return 0;
}


//Functions

/*
 *	c_read is called each time the file is read from
 *		-Check if the write lock is in use
 *			-add to the global count atomically of reading devices
 *		-Perform read, atomically decrement global count
 *		-Print some debug information to confirm the device was read successfully
 *		-Return the read data to user space
 */
static ssize_t c_read(struct file *f, char __user *buf, size_t len, loff_t *off){
	ssize_t retVal = 0;
	int b_read = 0;
	if(down_interruptible(&charSem)){;}

	copy_to_user(buf, dataBuf, strlen(dataBuf));
	up(&charSem);

	return strlen(dataBuf);
}




/*
 *	c_write is called each time the file is written to
 *		-Lock the file for processing - don't allow reads until write commits
 *			-Wait for current reads to finish
 *		-Copy write data to next open array indices in the buffer
 *			-If the data length exceeds the buffer length, begin writing back at the beginning
 *		-Print some debug information to confirm the device was written to successfully
 */
static ssize_t c_write(struct file *f, const char __user *buf, size_t len, loff_t *off){
	ssize_t retVal = 0;
	int s_mess = 0;	
	int i = 0;
	if(down_interruptible(&charSem)){;}

	for(i = 0; i < len; i++){
		dataBuf[characterCount % BUF_LEN] = buf[i];
		characterCount++;
	}
	up(&charSem);

	return len;
}




/*
 *	c_open is called each time the file is opened by a process
 *		-Lock the file for processing
 *		-Print some debug information to confirm the device was opened successfully
 */
static int c_open(struct inode *i, struct file *f){
	int retVal = 0;
	return retVal;
}




/*
 *	file_operations structure from fs.h associating callback functions with file operations
 *		This demonstration implements read, write, open, and release
 */
static struct file_operations fops={
	.owner=THIS_MODULE,
	.read=c_read,
	.write=c_write,
	.open=c_open,
	.release=c_release
};




/*
 *	init_module is called at the initialization of the module in the kernel
 *		-Register the character device and receive the MAJOR number
 *		-Create a device that can be accessed by respective MINOR number
 *		-Print some debug information to confirm the device was created successfully
 */
int init_module(void){
	void *ptr_error;
	//int result = 0;
	struct class *device_class;
	dev_t DEV_T;

	sema_init(&charSem,1);
//	printk(KERN_ALERT "Entering Character Driver\n");
	major = register_chrdev(0, device, &fops);


	if(major < 0){
//		printk(KERN_INFO "Register Failed! : %d\n", major);
		return major;
	}
	
 //  	printk("'mknod /dev/hello c %d 0'.\n", major);
	
	//Entering code
	DEV_T = MKDEV(major,1);
	device_class = class_create(THIS_MODULE, "charDevice");

	if(IS_ERR(ptr_error)){
		class_destroy(device_class);
		unregister_chrdev(0,"charDevice");
	}

	ptr_error = device_create(device_class, NULL, DEV_T, NULL, "charDevice");


//	printk(KERN_ALERT "\tinFile: %s\n",inFile);
///	printk(KERN_ALERT "\toutFile: %s\n", outFile);
//	printk(KERN_ALERT "\tmajor: %d\n", major);
//	printk(KERN_ALERT "\tdevice: %s\n",device);
	


	return 0;
}




/*
 *	cleanup_module is called at the removal of the module in the running kernel
 *		-Deregister the character device
 *		-Free any allocated memory and close the module
 */
void cleanup_module(void){
	//Closing code
//	printk(KERN_ALERT "Closing Character Driver\n");
	if(major != 0){
		unregister_chrdev(major, device);
	}
	return;
}

//module_init(init_module);
//module_exit();
