/**************************************************************
* Class:  CSC-415-01 Fall 2020
* Name: Phyoe Thu
* Student ID: 918656575
* Project: Assignment 6 - Device Driver
*
* File: char_driver.c
*
* Description: This program includes functions to initialize &
* create file operations (open, release, read, write, ioctl), 
* initialize the driver, and remove the driver.
**************************************************************/

#include <linux/module.h> 
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/ioctl.h> 
#include <linux/uaccess.h>
#include <linux/init.h> 

#define DEVICE_NAME "calculatorDriver" // Device name 

// Define ioctl numbers based on ioctl function and input parameters 
#define IOCTL_READ _IOR('a', 'a', int *) // Read data from the driver
#define IOCTL_WRITE _IOW('a', 'b', int *) // Write data to the driver 

// Initializing and exiting driver
static int init_driver(void);
static void exit_driver(void);

// Functions for file operations
int device_open(struct inode *, struct file *);
int device_release(struct inode *, struct file *);
ssize_t device_read(struct file *, char *, size_t, loff_t *);
ssize_t device_write(struct file *, const char *, size_t, loff_t *); 
long device_ioctl(struct file *, unsigned int, unsigned long);

// Structure for file operations
struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
    .unlocked_ioctl = device_ioctl
};

// Variables declaration
int major; // Major number for the driver
int open_count = 0; // Track the count for open
int value1 = 0; // First value for math operation
int value2 = 0; // Second value for math operation
int result = 0; // Result of the operation
int operator = 0; // Operator for the math operation

int count = 0; // Count for the write 

// Initialize the driver
static int init_driver(void) 
{
    printk("Inside %s function \n", __FUNCTION__); // Print out the function name on Kernel log
    printk(KERN_INFO "My device driver has been started... \n"); // Print out the info on Kernel log

    major = register_chrdev(0, DEVICE_NAME, &fops); // Register the device and get the major number

    if (major < 0) // Error check for major number
    {
        printk(KERN_ALERT "Failed to register the device with: %d \n", major); // Print out alert message on Kernel log
        return major;
    }

    printk(KERN_INFO "Major number for device driver is: %d \n", major); // Print out the major number on Kernel log

    return 0;
}

// Exit and remove the driver
static void exit_driver(void)
{
    printk("Inside %s function \n", __FUNCTION__); // Print out the function name on Kernel log
    printk(KERN_INFO "My device driver is about to be exited... \n"); // Print out the info on Kernel log

    unregister_chrdev(major, DEVICE_NAME); // Unregister the device

    printk(KERN_INFO "My device driver has been removed... \n"); // Print out the info on Kernel log
}

// Open the device
int device_open(struct inode *inode, struct file *file)
{
    printk("Inside %s function \n", __FUNCTION__); // Print out the function name on Kernel log
    printk(KERN_INFO "My device driver is about to be opened... \n"); // Print out the info on Kernel log

    if (open_count) // If open_count is 0
    {
        printk(KERN_ALERT "The device is already opened... \n"); // Print out alert message on Kernel log
        return -1;
    }

    open_count++; // Increment the count for open
    try_module_get(THIS_MODULE); // Increase the module usage count
    
    return 0;
}

// Close the device
int device_release(struct inode *inode, struct file *file)
{
    printk("Inside %s function \n", __FUNCTION__); // Print out the function name on Kernel log
    printk(KERN_INFO "My device driver is about to be closed... \n"); // Print out the info on Kernel log

    if (!open_count) // If open_count is not 0, which means the device wasn't open the in first place
    {
        printk(KERN_ALERT "The device wasn't opened yet... \n"); // Print out alert message on Kernel log
        return -1;
    }

    open_count--; // Decrement the count for open
    module_put(THIS_MODULE); // Reduce the module usage count

    return 0;
}

// Read the device
ssize_t device_read(struct file *file, char *buffer, size_t length, loff_t *offset)
{ 
    printk("Inside %s function \n", __FUNCTION__); // Print out the function name on Kernel log
    printk(KERN_INFO "My device driver is about to be read... \n");

    return 0;
}

// Write the device
ssize_t device_write(struct file *file, const char *buffer, size_t length, loff_t *offset)
{
    printk("Inside %s function \n", __FUNCTION__); // Print out the function name on Kernel log
    printk(KERN_INFO "My device driver is about to be written... \n");

    return 0;
}

// IO control for user application
long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg) 
{
    printk("Inside %s function \n", __FUNCTION__); // Print out the function name on Kernel log

    switch(cmd)
    {
        case IOCTL_WRITE: // Ask the user to write an input for the operator, value1 and value2
            if (count == 0)
            {
                copy_from_user(&operator, (int *) arg, sizeof(operator)); // Copy the operator from the user
                printk(KERN_INFO "Operator: %d \n", operator); // Print out the operator on Kernel log
                count++; // Increment the count and asks the user for value1
                break;
            }
            else if (count == 1)
            {   
                copy_from_user(&value1, (int *) arg, sizeof(value1)); // Copy the value1 from the user
                printk(KERN_INFO "Value 1: %d \n", value1); // Print out value1 on Kernel log
                count++; // Increment the count and asks the user for value2
                break;
            }
            else if (count == 2)
            {
                copy_from_user(&value2, (int *) arg, sizeof(value2)); // Copy the value2 from the user
                printk(KERN_INFO "Value 2: %d \n", value2); // Print out value2 on Kernel log
                count++; // Increment the count, count becomes 3, and asks the user for operator 
                break;
            }
            // fall through
        case IOCTL_READ: // Do calculations and read back the results
            if (operator == 1) // Addition
            {
                result = value1 + value2; // Add value1 and value2
                printk(KERN_INFO "Result: %d \n", result); // Print out the result on Kernel log
            }
            else if (operator == 2) // Subtraction
            {
                result = value1 - value2; // Subtrat value1 and value2
                printk(KERN_INFO "Result: %d \n", result); // Print out the result on Kernel log
            }
            else if (operator == 3) // Multiplication
            {
                result = value1 * value2; // Multiply value1 and value2
                printk(KERN_INFO "Result: %d \n", result); // Print out the result on Kernel log
            }
            else if (operator == 4) // Division
            {
                result = value1 / value2; // Divide value1 and value2
                printk(KERN_INFO "Result: %d \n", result); // Print out the result on Kernel log
            }
            else
            {
                break;
            }

            copy_to_user((int *) arg, &result, sizeof(result)); // Copy the results to the user 
            break;
    }

    if (count == 3)
    {
        count = 0;
    }

    return 0;
}

module_init(init_driver); // Defines the function to be called at module insertion time 
module_exit(exit_driver); // Defines the function to be called at module removal time 

MODULE_LICENSE("GPL"); // GNU Public License 
MODULE_AUTHOR("Phyoe Thu"); // Author name
MODULE_DESCRIPTION("Simple calculator driver"); // Description of the driver 