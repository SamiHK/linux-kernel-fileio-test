obj-m += device_file_lkm.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	-sudo rm /dev/characterdevice 
	-sudo rmmod device_file_lkm
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
test:
	-sudo rmmod device_file_lkm
	sudo dmesg -C
	sudo insmod device_file_lkm.ko
	dmesg
user:
	sudo cat /dev/characterdevice 
