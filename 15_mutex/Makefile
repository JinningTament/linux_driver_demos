obj-m += file.o
KDIR:=/home/luojin/source/linux/rk356x_linux/kernel
PWD?=$(shell pwd)
all:
		make -C $(KDIR) M=$(PWD) modules
clean:
		make -C $(KDIR) M=$(PWD) clean