#include<stdio.h>
#include<unistd.h>
#include<sys/syscall.h>


int main(void)

{
    int a, b;
    int ret;
    
    printf("%ld\n", syscall(327,8,12,&ret)); 			
}

