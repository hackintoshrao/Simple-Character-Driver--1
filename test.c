#include<linux/types.h>
#include<linux/kdev_t.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h> 
#include<linux/cdev.h>
#include<asm/uaccess.h>
#include<linux/errno.h>

static dev_t devno;
/*first 12 Bits of this 32-bit data type is used to store the MAJOR number of the device and i
  remaining 20 bits for the minor number*/ 
static struct cdev char_dev;

static int dev_open(struct inode *dev_inode,struct file *dev_file)
{	
	printk(KERN_INFO "\nRao's Device Opened");
	return 0;
}

static int dev_close(struct inode *dev_inode,struct file *dev_file)
{
        printk(KERN_INFO "\nRao's device closed");
	return 0;
}

static ssize_t dev_write(struct file *dev_file,const char __user *userbuf,size_t len,loff_t *off)
{
	char kbuf;
	int i;
	
	for(i=0;i<len;i++)
	{
		if(copy_from_user(&kbuf,userbuf+i,1))
		{
			return -EFAULT;
                }
                printk("\n%d<%c>",i+1,kbuf);
	}
	return len;
} 

 	
static struct file_operations dev_ops = {
	.owner=THIS_MODULE ,
	.open =dev_open,
        //.read =dev_read,
	.write=dev_write,
	.release=dev_close
};
 
static int __init constructor(void)/*initialization function,this runs when the module is loaded on top of kernel*/ 
{ 
 
 int ret; 

 if((ret=alloc_chrdev_region(&devno,0,1,"RAO's DRIVER"))<0) /*Used to dynamically allocate the major number for the device ,defined in <linux/fs.h>,The device is       
                                                             registered and A device by name "RAO's DRIVER " will be seen inside the file /proc/devices*/        
 	 return ret;
 printk(KERN_INFO "\nMajor NO=%d,MINOR NUMBER=%d\n",MAJOR(devno),MINOR(devno));
/*Kernel's equivalent of printf,the output can be viewed by using dmesg command*/ 
 cdev_init(&char_dev,&dev_ops);
/*kernel uses structure of type cdev to represent character devices internally,before using any driver functions its necessary to initialize them ,cdev_init takes care of that job*/
 if((ret=cdev_add(&char_dev,devno,1))<0)
 {
	unregister_chrdev_region(devno,1);
	return ret;
 }
 
 return 0;
}

static void __exit destructor(void)/*Kernel automatically loads this function when the module removed from top of kernel*/ 
{

 unregister_chrdev_region(devno,3); /*To unregister the registered device */
 printk(KERN_INFO "\nRAO's module removed");
}

module_init(constructor);/*Tells the kernel the name of the function to be called when the module is loaded*/
module_exit(destructor);/*Tells the kernel the name of the function to be called when the module is removed*/

MODULE_LICENSE("GPL");
MODULE_AUTHOR("karthic RAO");
/*More information about hte code is given to the kernel using MODULE_ macros*/
MODULE_DESCRIPTION("FIRST CHAR DRIVER:D");





