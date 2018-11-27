;;kernal.asm
;;made by Matt Perry
bits 32 ;;setting mode to 32 bit
section .text ;;start of the command list
    align 4
    dd 0x1BADB002 ;; magic number for allowing boot
    dd 0x00  ;; flags field set to zero
    dd - (0x1BADB002 + 0x00) ;; checksum that should set total between flag and magic nubmer back to zero.

global start
extern kernalMain  ;; kernamlMain is our main function defined in the c file

start:
    cli ;; clear all interrupts
    mov esp, stack_space ;; setting the stack pointer addresss
    call kernalMain ;;calling the kernalMain function
    hlt ;; halt the CPU only awakes on interrupts and interrupts are blocked.

section .bss
resb 8192 ;; creaking 8kb space for the stack
stack_space:  
