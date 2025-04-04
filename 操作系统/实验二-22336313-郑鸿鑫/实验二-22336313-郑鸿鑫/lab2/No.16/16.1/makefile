run:
	@qemu-system-i386 -hda hd.img -serial null -parallel stdio 
debug:
	@nasm -o 16.1.o -g -f elf32 16.1.asm
	@ld -o 16.1.symbol -melf_i386 -N 16.1.o -Ttext 0x7c00
	@qemu-system-i386 -hda hd.img -s -S -parallel stdio -serial null
build:
	@nasm -f bin 16.1.asm -o 16.1.bin
	@qemu-img create hd.img 10m
	@dd if=16.1.bin of=hd.img bs=512 count=1 seek=0 conv=notrunc
clean:
	@rm -fr *.bin *.o