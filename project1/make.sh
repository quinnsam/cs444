#! /bin/bash

make clean
make -j5
make modules_install
make firmware_install
cp arch/x86/boot/bzImage /boot/cs411_project${1}_kernel
if [ -d /lib/modules/3.0.4 ]; then
	dracut --force /boot/initramfs-cs411_project${1}_kernel.img 3.0.4
elif [ -d /lib/modules/3.0.4+ ]; then
	dracut --force /boot/initramfs-cs411_project${1}_kernel.img 3.0.4+
else
	echo "Failed to generate initramfs."
fi
