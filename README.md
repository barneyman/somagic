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


