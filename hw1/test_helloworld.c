#include<stdio.h>
#include<linux/kernel.h>
#include<unistd.h>
#include<sys/syscall.h>






int main(){


printf("%ld\n", syscall(326));

}

