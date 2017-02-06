To Use:
**********
./a.out
dmesg
**********

Contact info: Matthew Kaiser 103305050
Email:  maka1279@colorado.edu


Makefile:

General makefile some modifictations made for system call
The makefile is stored in x86/kernel

simple_add.c:

The system call for simple adding of two integers 
The file is stored in x86/kernel

syscall_x64.tbl

In this file I added the system call into the 64 bit section for system calls
This file is stored in x86/entry/syscalls

syscalls.h

This is the header file which holds prototypes of the system calls
This file is stored in include/linux 

syslog:

Log of the system calls
I used the command from the forum cp /var/log/syslog syslog to implant this file in my homework folder

test_add_number.c & test_helloworld.c

Test files for the system calls
I stored them in x86/entry



