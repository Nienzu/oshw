obj-m += oshw2.o

EXEC = user

all: 
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc oshw2_user.c -o user
clean: 
	rm -f $(EXEC) *.ko *.o *.mod.* *.order *.symvers perf.*
