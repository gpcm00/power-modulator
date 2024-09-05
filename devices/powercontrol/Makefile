obj-m += powctl.o

all: modules dt
	echo Builded Device Tree Overlay and kernel module

modules:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
dt: powctl.dts
	dtc -@ -I dts -O dtb -o powctl.dtbo powctl.dts
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf powctl.dtbo
	
