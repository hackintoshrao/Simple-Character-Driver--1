#include<linux/cdev.h>
#include<linux/types.h>
#include<kdev_t.h>
#include<linux/fs.h>
struct rao_dev { 
	struct rao_qset *data ; /*pointer to the starting address of the data*/
	int quantum ; 
	int qset; 
 	unsigned long size ; 
	struct cdev cdev ; /*char device structure*/
};
struct file_operations rao_fops {
	.owner = THIS_MODULE,
	.read  = rao_read,
	.write = rao_write,
	.open  = rao_open,
	.release = rao_release,
};

static void rao_setup_cdev(struct rao_dev *dev) { 
	
