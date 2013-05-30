struct kar_dev { 
	struct kar_qset *data ; /*pointer to the starting address of the data*/
	int quantum ; 
	int qset; 
 	unsigned long size ; 
	struct cdev cdev ; /*char device structure*/
};
