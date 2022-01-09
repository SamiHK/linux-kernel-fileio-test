#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sami Haroon");
MODULE_DESCRIPTION("A Simple File Writer that WRITE to a file and provides necessary permissions to be READ be user space");
MODULE_VERSION("1.0.0");


#define SUCCESS 0
#define DEVICE_NAME "characterdevice" /* Dev name as it appears in /proc/devices   */
#define BUF_LEN 1024		/* Max length of the message from the device */

int num_bytes = 0;
volatile static int is_open = 0;

static int Major;	
static char message[BUF_LEN];
static char *msg_Ptr;

/*
 *
 Linux Kernel Character Device Driver
 REF:https://tldp.org/LDP/lkmpg/2.6/html/x569.html
 *
*/

/**
 * @brief reads from file
 * 
 * @param filePtr 
 * @param outputBytes 
 * @param nBytes 
 * @param offset 
 * @return ssize_t 
 */
ssize_t device_file_read(struct file *filePtr, char __user *outputBytes, size_t nBytes, loff_t *offset)
{
    int bytes_read = 0;

    printk(KERN_INFO "Offset is %d \n", offset);
    if(offset == NULL) return -EINVAL;

    printk(KERN_INFO "Offset is %d \n", *offset);
    if(*offset >= num_bytes) return SUCCESS;


    if (*msg_Ptr == 0)
		return 0;

	/* 
	 * Actually put the data into the outputBytes 
	 */
	while (nBytes && *msg_Ptr) {

		/* 
		 * The outputBytes is in the user data segment, not the kernel 
		 * segment so "*" assignment won't work.  We have to use 
		 * put_user which copies data from the kernel data segment to
		 * the user data segment. 
		 */
		put_user(*(msg_Ptr++), outputBytes++);

		nBytes--;
		bytes_read++;
	}
    
    printk(KERN_INFO "Offset is %d : && bytes_read is %d.\n", *offset, bytes_read);
    return bytes_read;
}

/**
 * @brief writes to file
 * 
 * @param filePtr 
 * @param inputBytes 
 * @param nBytes 
 * @param offset 
 * @return ** ssize_t 
 */
ssize_t device_file_write (struct file *filePtr, const char __user *inputBytes, size_t nBytes, loff_t *offset)
{
    printk(KERN_INFO "Error -- cannot handle write!\n");
    return -EINVAL;
}


/**
 * @brief opens file
 * 
 * @param inodePtr 
 * @param filePtr 
 * @return int 
 */
int device_file_open (struct inode *inodePtr, struct file *filePtr)
{
    if(is_open == 1)
    {
        printk(KERN_INFO "Error -- Device already open!\n");
        return -EBUSY;
    }
    is_open = 1;

    msg_Ptr = message;
    try_module_get(THIS_MODULE);
    return SUCCESS;
}

/**
 * @brief closes file
 * 
 * @param inodePtr 
 * @param filePtr 
 * @return int 
 */
int device_file_release (struct inode *inodePtr, struct file *filePtr)
{
    if(is_open == 0)
    {
        printk(KERN_INFO "Error -- Device was not opened!\n");
        return -EBUSY;
    }
    is_open = 0;
    module_put(THIS_MODULE);
    return SUCCESS;
}

/**
 * @brief file operations definations for ioctl
 */
struct file_operations fops = {
	.read = device_file_read,
	.write = device_file_write,
	.open = device_file_open,
	.release = device_file_release
};

/**
 * @brief kernel initializer 
 * @return int 
 */
static int __init file_write_lkm_init(void)
{
    printk(KERN_INFO "Hello from File Writer Kernel!\n");
    strncpy(message, "This would be the string!\n", (BUF_LEN-1));

    num_bytes = strlen(message);
    
    printk(KERN_INFO "\n");
    printk(KERN_INFO "Copies: %s, Length: %d!\n", message, num_bytes);

    int Major = register_chrdev(0, DEVICE_NAME, &fops);

    if (Major < 0) {
	  printk(KERN_ALERT "Registering char device failed with %d\n", Major);
	  return Major;
	}
    
    printk(KERN_INFO "The %s device is Major: %d\n", DEVICE_NAME, Major);
    printk(KERN_INFO "IMPORTANT! please run the following command on terminal\n");
    printk(KERN_INFO "'sudo mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);

    return SUCCESS;
}


/**
 * @brief kernel terminator 
 * @return int 
 */
static void __exit file_write_lkm_exit(void)
{   
    printk(KERN_INFO "Goodbye from File Writer Kernel!\n");
}


module_init(file_write_lkm_init);
module_exit(file_write_lkm_exit);