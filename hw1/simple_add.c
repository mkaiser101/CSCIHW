#include <linux/kernel.h>
#include <linux/linkage.h>

asmlinkage long sys_simple_add(int number1, int number2, int* result)
{
 *result = (number1+number2);
 printk(KERN_ALERT "Number one: %d \n", number1);
 printk(KERN_ALERT "Number two: %d \n", number2);
 printk(KERN_ALERT "Sum: %d \n", *result);
 return 0;
}
