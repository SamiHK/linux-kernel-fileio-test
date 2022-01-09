# linux-kernel-fileio-test
Question:  Write a service that writes to a file on kernel space from user-level and read from the user level, on an OS of your choosing

## Installations
Setup environment for Linux Development(Ubuntu) run the following commands:

install build-essential
> sudo apt-get install build-essential linux-headers-$(uname -r)

install libc6-dev
> sudo apt-get install libc6-dev

install libc6-dev-i386 (optional -> recommended in case of dependency errors)
> sudo apt-get install libc6-dev-i386

install libc6-dev-amd64 (optional -> recommended for AMD processor machine)
> sudo apt-get install libc6-dev-amd64 (if x86_64 architecture)

> uname -r -a

If you see something like `Linux user 5.11.0-44-generic #48~20.04.2-Ubuntu SMP Tue Dec 14 15:36:44 UTC 2021 x86_64 x86_64 x86_64 GNU/Linux` then your environment is ready for Linux Kernel development.

## Build
To build modules of this project use the following command.

> make clean && make

## Test Kernel Space
> make test

After running this command you have to execute a command as mentioned on Kernel space
e.g. sudo mknod /dev/characterdevice c Major Minor
## Test User Space
> make user


# Maunal Test Commands
1. Load Kernel Module
> sudo insmod device_file_lkm.ko

2. Check Kernel Module Info
> modinfo device_file_lkm.ko

3. Check if hello module is loaded, check for output like `Hello from File Writer Kernel!`
> tail /var/log/kern.log
OR
> sudo dmesg
OR
> vi /proc/devices

4. Remove Kernel Module
> sudo rmmod device_file_lkm.ko

5. Check if hello module is removed, check for output like `Goodbye from File Writer Kernel!`
> tail /var/log/kern.log
OR
> sudo dmesg
OR
> vi /proc/devices

6. mknod [NAME, TYPE, MAJOR, MINOR]
> sudo mknod /dev/characterdevice c Major Minor

7. Check contents from user space
> sudo cat /dev/characterdevice


# IMPORTANT
For release build please checkout `release` branch

# Note:
Architecture Drawing + Demo steps are shown on files *kernel_file_writer.png* && *testing-file-kernel.png*