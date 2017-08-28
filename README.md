somagic
=======

my rewrite of (currently) the easycap somagic kernel driver, specifically for my rpi

using 3.12.26 Raspbian (debian/wheezy)

The original kernel code was pulled from instructions at https://code.google.com/p/easycap-somagic-linux/wiki/BuildingKernelModule
That URL references DIFFERENT source to the kernel source in the easycap-somagic-linux google repository

The intention is to have a stable kernel driver for me to access an STB from my pi

the rpi simply can't keep up currently, i'm using the FSM USB driver and 
i've overclocked to turbo - i'm going to try exposing lower FPS streams
and also have a bash at using the GPU encoder

This is my first leap into linux coding, so, excuse any blatant errors

[(please refer to https://www.raspberrypi.org/documentation/linux/kernel/building.md 
for help on getting source/building the kernel)

the basic steps for building the kernel are 
1. get the code!
2. get the toolset
3. configure what/how it's built ( *_defconfig)
4. build kernel
5. build modules
6. install them
]


to get this compiling, two choices

A. to Compile this module only

1. get your linux source
2. cd linux/drivers/media/usb
3. git clone https://github.com/barneyman/somagic.git
4. go back to your linux directory
5. make M=drivers/media/usb/somagic modules modules_install

B. to add to your kernel build

1. as above
2. as above
3. as above
4. edit Kconfig, add the following line in the MEDIA_ANALOG_TV_SUPPORT block `source "drivers/media/usb/somagic/Kconfig"`
5. edit Makefile, add the following in block of similar commands `obj-$(CONFIG_VIDEO_SMI2021) += somagic/`
6. go back to your linux directory
7. make menuconfig 
--1. add SMI2021 module (in device drivers/multimedia support/Media USB Adaptors/Somagic)
8. build your kernel and modules
--1. make
--2. make modules
--3. make modules_install
--4. (and copy the img over)

 


