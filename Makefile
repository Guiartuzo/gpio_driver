obj-m += lll_gpio_driver.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

# Device Tree Overlay
DTS  := lll_gpio_driver_overlay.dts
DTBO := mygpio.dtbo

OVERLAY_DIR := $(shell if [ -d /boot/firmware/overlays ]; then \
	echo /boot/firmware/overlays; \
	else echo /boot/overlays; fi)

all: modules dtbo

modules:
	make -C $(KDIR) M=$(PWD) modules

dtbo: $(DTBO)

$(DTBO): $(DTS)
	dtc -@ -I dts -O dtb -o $@ $<

install: all
	sudo cp $(DTBO) $(OVERLAY_DIR)/

clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -f $(DTBO)
