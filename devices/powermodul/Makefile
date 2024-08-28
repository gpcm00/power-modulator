obj-m += powmodul.o

all: modules dt
	echo Builded Device Tree Overlay and kernel module

modules:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
dt: testoverlay.dts powmodul.dts
	dtc -@ -I dts -O dtb -o powmodul.dtbo powmodul.dts
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf powmodul.dtbo
	
