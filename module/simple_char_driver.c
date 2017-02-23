#include<linux/init.h>
#include<linux/module.h>

#include<linux/fs.h>
#include<asm/uaccess.h>
#define BUFFER_SIZE 1024




#define deviceName "simple_character_device"
static char device_buffer[BUFFER_SIZE] = {'\0'};
static int numberOfTimesFileWasOpened = 0;
static int numberOfTimesFileWasClosed = 0;


static int availableSpace = 0;
static int bytesNotCopied = 0;
static int totalBytesRead = 0;
static int bytesToWrite = 0;
static int totalBytesCopied = 0;
ssize_t simple_char_driver_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	{
	/* *buffer is the userspace buffer to where you are writing the data you want to be read from the device file*/
	/*  length is the length of the userspace buffer*/
	/*  current position of the opened file*/
	/* copy_to_user function. source is device_buffer (the buffer defined at the start of the code) and destination is the userspace buffer *buffer */

	 /* note: *offset seems to always be zero but included it just in case */
	 bytesNotCopied = copy_to_user(buffer, device_buffer, *offset);
	 totalBytesRead = *offset - bytesNotCopied;

	 /* Nothing was read */
	 if(totalBytesRead == 0){
		 printk(KERN_ALERT "Unable to read buffer, it is either empty or there was an error!\n");
		 return totalBytesRead;
	 }


	 printk(KERN_ALERT "Read %d bytes; Buffer Message: %s\n", totalBytesRead, device_buffer);

	return 0;
}
}



ssize_t simple_char_driver_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{

	/* *buffer is the userspace buffer where you are writing the data you want to be written in the device file*/
	/*  length is the length of the userspace buffer*/
	/*  current position of the opened file*/
	/* copy_from_user function. destination is device_buffer (the buffer defined at the start of the code) and source is the userspace 		buffer *buffer */

	 /* note: *offset seems to always be zero but included it just in case */
	 availableSpace = BUFFER_SIZE - *offset;

	 /* Check availableSpace */
	 if(availableSpace <= length){ /*There is insufficient space for the copy */
		 bytesToWrite = availableSpace; /* Only copy until space is full */
	 }else{ /* All bytes can be copied */
	 	 bytesToWrite = length;
	 }

	 bytesNotCopied = copy_from_user(device_buffer + *offset, buffer, bytesToWrite);
	 totalBytesCopied = bytesToWrite - bytesNotCopied;

	 /* if the bytes not coppied equals bytesToCopy, none of the bytes were copied so the device is full */
	 if(totalBytesCopied == 0){
		 printk(KERN_ALERT "Could not copy message, device is full!\n");
		 return -1;
	 };

	 /* current location on buffer*/
	 *offset += totalBytesCopied;

	 printk(KERN_ALERT "Wrote %d bytes!\n", totalBytesCopied);

	return length;
}


int simple_char_driver_open (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is opened and also print the number of times this device has been opened until now*/
	numberOfTimesFileWasOpened++;
	printk(KERN_ALERT "The file has been opened. It has been opened %d times now.\n", numberOfTimesFileWasOpened);
	return 0;
}


int simple_char_driver_close (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is closed and also print the number of times this device has been closed until now*/
	numberOfTimesFileWasClosed++;
	printk(KERN_ALERT "The file has been closed. It has been closed %d times now.\n", numberOfTimesFileWasClosed);
	return 0;
}

struct file_operations simple_char_driver_file_operations = {

	.owner   = THIS_MODULE,
	.read = simple_char_driver_read,
	.write = simple_char_driver_write,
	.open = simple_char_driver_open,
	.release = simple_char_driver_close
	/* add the function pointers to point to the corresponding file operations. look at the file fs.h in the linux souce code*/
};

static int simple_char_driver_init(void)
{
	/* print to the log file that the init function is called.*/
	/* register the device */
	register_chrdev(255, deviceName, &simple_char_driver_file_operations);
	printk(KERN_ALERT "inside %s function\n",__FUNCTION__);
	return 0;
}

static int simple_char_driver_exit(void)
{
	/* print to the log file that the exit function is called.*/
	/* unregister  the device using the register_chrdev() function. */
	unregister_chrdev(255, deviceName);
	printk(KERN_ALERT "inside %s function\n",__FUNCTION__);
	return 0;
}


/* add module_init and module_exit to point to the corresponding init and exit function*/
module_init(simple_char_driver_init);
module_exit(simple_char_driver_exit);
