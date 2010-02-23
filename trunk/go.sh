#!/bin/bash
sudo mount -o sync /dev/loop7 /mnt
sudo rm -f /mnt/boot/kernel0
sudo cp -v kernel0 /mnt/boot
sudo umount /mnt
bochs
