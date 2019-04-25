#!/bin/bash
nasm -f elf32 kernel.asm -o kasm.o
gcc -fno-stack-protector -m32 -c kernelFunctions.c -o kcf.o
gcc -fno-stack-protector -m32 -c kernel.c -o kc.o
gcc -fno-stack-protector -m32 -c keyboardFunctions.c -o kf.o
gcc -fno-stack-protector -m32 -c chell.c -o ch.o
gcc -fno-stack-protector -m32 -c DtoH.c -o dth.o
gcc -fno-stack-protector -m32 -c DtoB.c -o dtb.o
gcc -fno-stack-protector -m32 -c calc.c -o cal.o
ld -m elf_i386 -T link.ld -o kernel kasm.o kc.o kcf.o kf.o ch.o dth.o dtb.o cal.o
qemu-system-i386 -kernel kernel
