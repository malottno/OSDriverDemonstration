#!/bin/sh

insmod ./charDriver.ko $* || exit 1


rm -f /dev/charDevice[0-3]

mknod /dev/charDevice0 c $1 0
mknod /dev/charDevice1 c $1 1
