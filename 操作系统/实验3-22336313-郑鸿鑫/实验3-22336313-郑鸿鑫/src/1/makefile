build:
	nasm -f bin chs.asm -o chs.bin
	nasm -f bin bootloader.asm -o bootloader.bin
	dd if=chs.bin of=hd.img bs=512 count=1 seek=0 conv=notrunc
	dd if=bootloader.bin of=hd.img bs=512 count=5 seek=1 conv=notrunc
run:
	qemu-system-i386 -hda hd.img -serial null -parallel stdio 
clean:
	rm -fr *.bin