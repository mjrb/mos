# MOS
an operating system I'm working on based on the "Write Your Own Operating System"
series on [YouTube](https://www.youtube.com/watch?v=1rnA6wpF0o4&list=PLHh55M_Kq4OApWScZyPl5HhgsTJS9MZ6M).
here is the [wyoos repo](https://github.com/AlgorithMan-de/wyoos).

__this is not encredibly polished at all__

It's interesting playing around with really low level stuff on i386 and seeing the stuff
that's just really weird because of backwards compatibility. I kinda want to rewrite the
interrupt thing in a way that might be cleaner, but for now I'm following the series to
just try to get interrupts working. so far its getting interrupts for the hardware timer
but I'm trying to get keyboard ones too.

I also got text to wrap and scroll automatically for what that's worth.

## requirements
- g++
- 32 bit compilation libs (for g++ -m32)
- qemu (you can use some other vm thing but i used this one)
- xorriso (there's a way to give qemu bzimages but i haven't bothered)
  + __IMPORTANT__ if you don't have xorriso grub-mkrescue might silently fail :-/
    install xorriso to avoid confusion
- grub-mkrescue

## debug requirements
- gdb

## other notes
the debugging thing is cool but it took me a while to setup
i had to play around and guess where to add-symbols-file mykernel.bin to get
things to line up. qemu will allow you to treat your kernel kinda like a normal program
using remote gdb stuff. see gdbscript

also it doesn't mater if you use qemu-system-i386 or qemu-system-x86_64, but if your
qemu package is like the Ubuntu 18.04 one and you try to run `$ qemu` you will see
that there is no binary for some reason and you need to pick from qemu-* or qemu-system-*