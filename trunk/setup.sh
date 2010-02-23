#!/bin/bash
#sudo mknod -m 0660 /dev/loop7 b 7 7
sudo chown root:disk /dev/loop7
sudo losetup -o 32256 /dev/loop7 c.img
