main:
	cd ramdisk &&  \
	cpio -i -t -F ../ramdisk.img > list && \
	cpio -o -H newc -O lk.img < list && \
	cpio -o -H newc -O lk.img < list && \
	gzip lk.img && \
	mkimage -A arm -O linux -T ramdisk -C none -a 0x40800000 -n "ramdisk" -d lk.img.gz  ../ramdisk-uboot.img && \
	rm lk.img.gz list
	
decompress:
	dd if=ramdisk-uboot_origin.img bs=64 skip=1 of=ramdisk.img.gz
	gunzip ramdisk.img.gz
	mkdir ramdisk
	cd ramdisk && cpio -i -F ../ramdisk.img

cp:
	cp ramdisk-uboot.img  /mnt/hgfs/Desktop/

#clean:
#	rm ramdisk  ramdisk.* ramdisk-uboot.img -rf
