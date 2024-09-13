# Automatically generate lists of sources using wildcard.
C_SOURCES = $(wildcard kernel/*.c kernel/drivers/vga/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h kernel/drivers/vga/*.h drivers/*.h)

# Convert *.c filenames to *.o as a list of object files to build.
OBJ = ${C_SOURCES:.c=.o}

# Clean, build, and run.
all: clean os-image run

# Removes all built files.
clean:
	rm -f boot/*.bin
	rm -f drivers/*.o
	rm -f example/*.bin example/*.o
	rm -f kernel/*.bin kernel/*.o
	rm -f kernel/drivers/vga/*.o
	rm -f os-image

# Kernel code --------------------------------------------------------------------------------------

# Create kernel binary by linking kernel entry code, kernel main, and other object files.
kernel/kernel.bin: kernel/enter_kernel.o $(OBJ)
	x86_64-elf-ld -melf_i386 -o $@ -Ttext 0x1000 --oformat binary $^

# Create executable image by prepending kernel binary with boot sector.
os-image: boot/load_kernel.bin kernel/kernel.bin
	cat $^ > $@

# Compile assembly sources into binary.
%.bin: %.asm
	nasm $< -f bin -o $@

# Assemble the kernel entry to an object file.
%.o: %.asm
	nasm $< -f elf -o $@

# Compile C sources into object files.
%.o: %.c
	x86_64-elf-gcc -m32 -ffreestanding -c $< -o $@

# --------------------------------------------------------------------------------------------------

# Run operating system on emulated x86.
run: os-image
	qemu-system-x86_64 -drive file=$<,format=raw -net none -no-reboot -no-shutdown -monitor stdio -icount shift=7
