CXXARGS=-m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-threadsafe-statics -fno-exceptions -fno-leading-underscore -g
TESTCXXARGS=-m32 -fno-use-cxa-atexit -fno-threadsafe-statics -fno-builtin -fno-rtti -fno-exceptions -g
CXX=g++

ASARGS=--32
AS=as

LDARGS=-melf_i386
LD=ld

HANDLERS=keyboard.o syscall.o
PROGRAMS= #program.o
FILESYSTEMS=jfs.o testfs.o fsmanager.o
DRIVERS=port.o print.o interrupts.o interruptsstubs.o

CORE = gdt.o loader.o kernel.o allocator.o
OBJECTS = $(CORE) $(DRIVERS) $(HANDLERS) $(PROGRAMS) $(FILESYSTEMS)

%.o: %.cpp
	$(CXX) $(CXXARGS) -o $@ -c $<
%.o: %.s
	$(AS) $(ASARGS) -o $@ $<

alloctest: alloctest.cpp allocator.o
	$(CXX) $(TESTCXXARGS) alloctest.cpp allocator.o -o alloctest

mykernel.bin: linker.ld $(OBJECTS)
	$(LD) $(LDARGS) -T $< -o $@ $(OBJECTS)

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin

mykernel.iso: mykernel.bin grub.cfg
	mkdir -p iso/boot/grub
	cp mykernel.bin iso/boot
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -o mykernel.iso iso
	rm -rf iso

run: mykernel.iso
	qemu-system-i386 -cdrom mykernel.iso -m 2048 -boot d #-d int
debug: mykernel.iso
	gdb -x gdbscript

.PHONY: clean
clean:
	rm -f $(OBJECTS) mykernel.bin mykernel.iso *~

