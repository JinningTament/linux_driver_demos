obj-m += dev_t.o
KDIR:=/home/luojin/source/linux/rk356x_linux/kernel
PWD?=$(shell pwd)
all:
		make -C $(KDIR) M=$(PWD) modules
clean:
		make -C $(KDIR) M=$(PWD) clean