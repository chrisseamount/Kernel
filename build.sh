#!/bin/bash
cd ~/Documents/Kernel
nasm -f elf32 kernel.asm -o kasm.o
gcc -fno-stack-protector -m32 -c kernelFunctions.c -o kcf.o
gcc -fno-stack-protector -m32 -c kernel.c -o kc.o
gcc -fno-stack-protector -m32 -c initPIC.c -o ip.o
gcc -fno-stack-protector -m32 -c keyboardFunctions.c -o kf.o
ld -m elf_i386 -T link.ld -o kernel kasm.o kc.o kcf.o ip.o kf.o
qemu-system-i386 -kernel kernel
