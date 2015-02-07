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

to get this compiling, two choices

a. to Compile this module only
1. get your linux source
2. cd /linux/drivers/media/usb
3. git clone https://github.com/barneyman/somagic.git
4. make M=drivers/media/usb/somagic modules

b. to add to your kernel build
1, 2 & 3 from above
4. edit Kconfig and Makefile in drivers/media/usb to refer to somagic
5. make menuconfig (in linux dir), add SMI2021 module

 


