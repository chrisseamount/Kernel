#pragma once

#define LINES 25
#define COLUMNS_LINE 80
#define BYTES_ELEMENT 2
#define SCREENSIZE BYTES_ELEMENT * COLUMNS_LINE * LINES
//This is the definition section for the ports hard coded into the hardware.
#define KEYBOARD_DATA_PORT 0x60 // is the data port and stores the keycode from the keyboard
#define KEYBOARD_STATUS_PORT 0x64 // this is the status port and it returns a state from the interrupt table
// definitions for the idt table for 256 in size
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08
// special keycodes for backspace and enter more to be defined
#define ENTER_KEY_CODE 0x1C
#define BACKSPACE_KEY_CODE 0X0E

extern unsigned char keyboard_map[128];
extern void keyboard_handler(void);
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);

char *videoPtr = (char *) 0xb8000; //setting up video memory beginnning at 0xb8000
unsigned int dWindow = 0; // loop count for drawing video on screen.
unsigned int stringLocation = 0;
