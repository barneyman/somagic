somagic
=======

my rewrite of (currently) the easycap somagic kernel driver, specifically for my rpi

using 3.12.26 Raspbian (debian/wheezy)

The original kernel code was pulled from instructions at https://code.google.com/p/easycap-somagic-linux/wiki/BuildingKernelModule
That URL references DIFFERENT source to the kernel source in the easycap-somagic-linux google repository

The intention is to have a stable kernel driver for me to access an STB from my pi

This is my first leap into linux coding, so, excuse any blatant errors

NB - it currently (for debug purposes) only commits one frame every 25 - this WILL be remedied (it's SEEING all the frames)

