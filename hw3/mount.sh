#!/bin/zsh

mkfs.ext2 /dev/sbd0

mount /dev/sbd0 /home/bob/Documents/cs444/hw3/ramdisk

chmod -R 777 /home/bob/Documents/cs444/hw3/ramdisk
