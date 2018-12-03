// Created by Matthew Perry, Anthony Kolendo and Chris Seamount
// Basic Kernal that makes Kernal name on page
#include "keyboard_map.h" // header holding the keyboard_map
#include "kernelFunctions.h" //header holding the basic kernel functions
//Keyboard definitions
//In an effort to get keyboard functionality we took code from another source, after looking around there seems to be alot of the same code passed around.
//Keyboard driver was taken from Arjun Sreedharan as he gave a great article on explaining what is occuring in the code.
//Comments were added to work though understanding the driver
// this section of defines is for creating the window size of 25 lines but 80 columns with each column containing 2 bits
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
//keyboard map is a table created in correlation with the keycode recived from the keyboard hardware from ports
extern unsigned char keyboard_map[128];
extern void keyboard_handler(void);
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);

char *videoPtr = (char *) 0xb8000; //setting up video memory beginnning at 0xb8000
unsigned int dWindow = 0; // loop count for drawing video on screen.
unsigned int stringLocation = 0;
// IDT entry is the interrupt descriptor talbe. we are defining this table to use in the kernel. IE making our own interrupts. Intel reserved the first 32.
struct IDT_entry {
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits;
};
struct IDT_entry IDT[IDT_SIZE];


//function created by Arjun Sreedharan
//from https://arjunsreedharan.org/post/82710718100/kernels-101-lets-write-a-kernel
void idt_init(void)
{
    unsigned long keyboard_address;
    unsigned long idt_address;
    unsigned long idt_ptr[2];

    /* populate IDT entry of keyboard's interrupt */
    keyboard_address = (unsigned long)keyboard_handler;
    IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
    IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
    IDT[0x21].zero = 0;
    IDT[0x21].type_attr = INTERRUPT_GATE;
    IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

    /*     Ports
    *    PIC1   PIC2
    *Command 0x20   0xA0
    *Data    0x21   0xA1
    */

    /* ICW1 - begin initialization */
    write_port(0x20 , 0x11);
    write_port(0xA0 , 0x11);

    /* ICW2 - remap offset address of IDT */
    /*
    * In x86 protected mode, we have to remap the PICs beyond 0x20 because
    * Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
    */
    write_port(0x21 , 0x20);
    write_port(0xA1 , 0x28);

    /* ICW3 - setup cascading */
    write_port(0x21 , 0x00);
    write_port(0xA1 , 0x00);

    /* ICW4 - environment info */
    write_port(0x21 , 0x01);
    write_port(0xA1 , 0x01);
    /* Initialization finished */

    /* mask interrupts */
    write_port(0x21 , 0xff);
    write_port(0xA1 , 0xff);

    /* fill the IDT descriptor */
    idt_address = (unsigned long)IDT ;
    idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
    idt_ptr[1] = idt_address >> 16 ;

    load_idt(idt_ptr);
}
//newline function
void newLine(){
    unsigned int lineSize = BYTES_ELEMENT*COLUMNS_LINE;
    dWindow = dWindow + (lineSize - dWindow % (lineSize));
}

void kb_init(void)
{
    /* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/
    write_port(0x21 , 0xFD);
}

void keyboard_handler_main(void)
{
    unsigned char status;
    char keycode;

    /* write EOI */
    write_port(0x20, 0x20);

    status = read_port(KEYBOARD_STATUS_PORT);
    /* Lowest bit of status will be set if buffer is not empty */
    if (status & 0x01) {
        keycode = read_port(KEYBOARD_DATA_PORT);
        if(keycode < 0){
            return;
        }
        if(keycode == BACKSPACE_KEY_CODE){  //checks for backspace key and replaces text on screen with a space
            if(dWindow == 0)  //if at top left corner dont do anything
            {
              return;
            }
            dWindow-=2;
            videoPtr[dWindow]= ' ';
            return;
        }
        if(keycode == ENTER_KEY_CODE) { //newlines if enter key is pressed
            newLine();
            return;
        }


        videoPtr[dWindow++] = keyboard_map[(unsigned char) keycode];  //write text to screen
        videoPtr[dWindow++] = 0x30; //set background color
    }
}

//this is the print function to print the string we are getting from the keyboard drivers.
void kprint(const char *str)
{
    unsigned int i = 0;
    while (str[i] != '\0') {
        videoPtr[dWindow++] = str[i++];
        videoPtr[dWindow++] = 0x30;
    }
}


//our main function for kernel main

void kernelMain(){
    //int for storing string len from our string len function used to center text on screen
    unsigned int x = 0;
    const char *str = "This is the Kernal Loading up..";
    clear(videoPtr);
    dWindow = 0;
    dWindow = (80*2*13)+60;
    drawSlow(str,dWindow,videoPtr,stringLocation);
    str = "....................................";
    drawSlow(str,dWindow,videoPtr,stringLocation);
    clear(videoPtr);
    dWindow = 0;
    drawBox(videoPtr);
    str = "Welcome to OS Lite";
    dWindow += (160*13);
    dWindow += 60;
    drawSlow(str,dWindow,videoPtr,stringLocation);
    str = "Home";
    dWindow+= 160;
    draw(str,dWindow,videoPtr,stringLocation);
    str = "Help";
    dWindow+=160;
    draw(str,dWindow,videoPtr,stringLocation);
    str = "Created by Matt Chris and Anthony";
    dWindow+= 160;
    draw(str,dWindow,videoPtr,stringLocation);
    sleep();
    sleep();
    sleep();
    str = "";
    sleep();
    sleep();
    clear(videoPtr);
    dWindow = 0;
    //this is the keyboard being booted up
    idt_init();
    kb_init();
    //while loop so we can type away.
    while(1);
    return;

}
