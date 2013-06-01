#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/slab.h>
static dev_t first; // Global variable for the first device number 
static struct cdev c_dev; // Global variable for the character device structure
static struct class *cl; // Global variable for the device class
struct simulate_qset {
	void **data ; 
	struct simulate_qset *next ; 
} 
struct simulate_dev  { 
	struct simulate_qset *data ; 
	int quantum;
	int qset ; 
	unsigned long size  ;
	struct cdev simulate_cdev;
};
static int simulate_open(struct inode *i, struct file *f)
{
  printk(KERN_INFO "Driver: open()\n");
  return 0;
}
  static int simulate_close(struct inode *i, struct file *f)
{
  printk(KERN_INFO "Driver: close()\n");
  return 0;
}
  static ssize_t simulate_read(struct file *f, char __user *buf, size_t
  len, loff_t *off)
{
  printk(KERN_INFO "Driver: read()\n");
  return 0;
}
  static ssize_t simulate_write(struct file *f, const char __user *buf,
  size_t len, loff_t *off)
{
  printk(KERN_INFO "Driver: write()\n");
  return len;
}
  static struct file_operations simulate_fops =
{
  .owner = THIS_MODULE,
  .open = simulate_open,
  .release = simulate_close,
  .read = simulate_read,
  .write = simulate_write
};
 static void scull_setup_cdev(struct simulate_dev *dev, dev_t devno) 
{
	

	cdev_init(&dev->cdev, &scull_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &scull_fops;
	err = cdev_add(&dev->cdev, devno, 1);
		
	if(err)
		printk(KERN_NOTICE "Error %d adding scull%d",err, index);
}


static int __init simulate_init(void) /* Constructor */
{
  printk(KERN_INFO "\nModule Inserted");
  if (alloc_chrdev_region(&first, 0, 1, "") < 0)
  {
    return -1;
  }
    if ((cl = class_create(THIS_MODULE, "simulate_driver")) == NULL)
  {
    unregister_chrdev_region(first, 1);
    return -1;
  }
    if (device_create(cl, NULL, first, NULL, "RAO_SIM") == NULL)
  {
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    return -1;
  }
    cdev_init(&c_dev, &simulate_fops);
    if (cdev_add(&c_dev, first, 1) == -1)
  {
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    return -1;
  }
  return 0;
}
 
static void __exit simulate_exit(void) /* Destructor */
{
  cdev_del(&c_dev);
  device_destroy(cl, first);
  class_destroy(cl);
  unregister_chrdev_region(first, 1);
  printk(KERN_INFO "\nModule removed");
}
 
module_init(simulate_init);
module_exit(simulate_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Karthic Rao");
