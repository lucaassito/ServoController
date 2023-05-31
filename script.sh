#!/bin/bash

make clean
make
if test -f "exec.elf"; then
    echo "succesfully compiled"
    sshpass -p 'raspberrypi' scp exec.elf pi@10.42.0.41:/home/pi
else
echo "exec.elf was not created"
fi