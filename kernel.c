// Created by Matthew Perry, Anthony Kolendo and Chris Seamount
// Basic Kernal that makes Kernal name on page
#include "keyboard_map.h" // header holding the keyboard_map
#include "kernelFunctions.h"
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
#define KEYBOARD_DATA_PORT 0x60 // is the data port and stores the key code from the keyboard
#define KEYBOARD_STATUS_PORT 0x64 // this is the status port and it returns a state from the interrupt table
// definitions for the idt table for 256 in size
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08
// special key codes for backspace and enter more to be defined
#define ENTER_KEY_CODE 0x1C
#define BACKSPACE_KEY_CODE 0X0E
#define SHIFT_KEY_CODE 0x2A
#define CAPS_KEY_CODE 0x3A
// Arrow keys
#define LEFT_ARROW  0x4B
#define UP_ARROW    0x48
#define RIGHT_ARROW 0x4D
#define DOWN_ARROW  0x50

//things from assembly
extern void keyboard_handler(void);
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);
// forward deceleration for functions defined in this file.
void flushString(char* string);
void kprint();
void storeString();
int checkString(const char* string);
int printCheck();
//defining some variables that stay const
char *videoPtr = (char *) 0xb8000; //setting up video memory beginnning at 0xb8000
unsigned int windowPos = 0; // loop count for drawing video on screen.
unsigned int stringLocation = 0;
unsigned int cWindow = 0; // loop counter for clearing window/
unsigned char buffer[79]; // array for handling the input from users
unsigned char userString[79]; // array fro holding the users information
unsigned char exitString[] = "exit"; // exit command
unsigned char clearString[] = "clear"; // clear command
unsigned char printString[] = "print("; // print command
unsigned char stringtoPrint[79]; // string that is to be printed

int hang = 1; // a hang for waiting on input
int exitKernel = 0; // an exit controller
int caps = 0; // caps controller
int storeCaps; // it stores capts

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
    windowPos = windowPos + (lineSize - windowPos % (lineSize));
}
//starting up the keyboard
void kb_init(void)
{
    //this sets the mask
    write_port(0x21 , 0xFD);
}
//move cursor function that takes the current draw window position and writes to the ports of the vga move cursor
void moveCursor(unsigned int drawWindow){
  unsigned short cursorLocation = (drawWindow/2);
  //sending in the interrupt 14 to the cursor position to inform of low bits
  write_port(0x3D4 ,14);
  // taking the low bits for location
  write_port(0x3D5, cursorLocation>>8);
  //sending the interrupt 15 to inform
  write_port(0x3D4, 15);
  //passing high bits
  write_port(0x3D5, cursorLocation);
}

void keyboardFunction(unsigned char keyboard[128],unsigned char keycode)
{
  if(buffer[((windowPos%160)/2)] == '\0') // if we are at the end just work normal
  {
    buffer[((windowPos%160)/2)-1] = keyboard[keycode];
    videoPtr[windowPos++] = keyboard[keycode];  //write text to screen
    videoPtr[windowPos++] = 0x30; //set background color
  }
  else
  {//spooky code
    int stPtr = windowPos+2;
    while(buffer[((stPtr%160)/2)-1] != '\0') // find the end of the buffer
    {
      stPtr++;
    }
    while(stPtr != windowPos)
    {
      buffer[((stPtr%160)/2)-1] = buffer[((stPtr%160)/2)-2];
      videoPtr[stPtr] = videoPtr[stPtr-2];
      stPtr-=2;
    }
    buffer[((windowPos%160)/2)-1] = keyboard[keycode];
    videoPtr[windowPos++] = keyboard[keycode];  //write text to screen
    videoPtr[windowPos++] = 0x30; //set background color
  }
  moveCursor(windowPos);
  return;
}

// definition of all the keyboard actions that occur when a key is pressed
void keyboard_handler_main(void)
{
    unsigned char status;
    unsigned char keycode;

    /* write EOI */
    write_port(0x20, 0x20);
    status = read_port(KEYBOARD_STATUS_PORT);
    /* Lowest bit of status will be set if buffer is not empty */
    if (status & 0x01) {

        keycode = read_port(KEYBOARD_DATA_PORT);

        if(keycode>=0x58 && keycode!=0xAA)
        {
          return;
        }

        /*if(hang==1){
          hang=0; // do for fancy hanging
          return;
        }*/

        if(keycode == BACKSPACE_KEY_CODE){  //checks for backspace key and replaces text on screen with a space
            if(buffer[0]=='\0')  //if at top left corner dont do anything
            // if the thing before it was a null term then replace thing with null term
            // take a mask and the shift left by the amount of a char????
            {
              return;
            }
            windowPos-=2;
            int storeWindow = windowPos;
            if(buffer[((windowPos%160)/2)] == '\0')
            {
              buffer[((windowPos%160)/2)-1] = '\0'; // end of string? replace with '\0'
              videoPtr[windowPos]= ' ';
            }
            else
            {
                while(buffer[((windowPos%160)/2)] != '\0')
                {
                  buffer[((windowPos%160)/2)-1] = buffer[((windowPos%160)/2)];
                  videoPtr[windowPos] = videoPtr[windowPos+2];// not end of string shift it all down.
                  buffer[((windowPos%160)/2)] = '\0';
                  videoPtr[windowPos+2] = ' ';
                  windowPos+=2;
                }
            }
            windowPos = storeWindow;
            moveCursor(windowPos);

            return;
        }
        if(keycode == ENTER_KEY_CODE) { //newlines if enter key is pressed
            storeString();
            flushString(stringtoPrint);
            if(checkString(exitString)){
              const char* str = "Good Bye!";
              newLine();
              kprint(str);
              flushString(buffer);
              exitKernel = 1;
              moveCursor(50000);
              return;
            }
            if(checkString(clearString)){
              flushString(buffer);
              clear(videoPtr);
              windowPos = 0;
              videoPtr[windowPos++] = '>';  //write text to screen
              videoPtr[windowPos++] = 0x30; //set background color
              moveCursor(windowPos);
              return;
            }
            if(printCheck()){
              newLine();
              kprint(stringtoPrint);
              newLine();
              flushString(buffer);
              flushString(stringtoPrint);
              videoPtr[windowPos++] = '>';  //write text to screen
              videoPtr[windowPos++] = 0x30; //set background color
              moveCursor(windowPos);
              return;
            }

            flushString(buffer);
            newLine();
            videoPtr[windowPos++] = '>';  //write text to screen
            videoPtr[windowPos++] = 0x30; //set background color
            moveCursor(windowPos);
            return;
        }
        if(keycode == LEFT_ARROW){
          if(((windowPos%160)/2)-1 <= 0)  // if at the cursor is at the input ">"
          {
            return;
          }
          windowPos-=2;
          moveCursor(windowPos);
          return;
        }
        if(keycode == RIGHT_ARROW){
          if(buffer[((windowPos%160)/2)-1] == '\0')  //if the cursor is at the end of the words then stop
          {
            return;
          }
          windowPos+=2;
          moveCursor(windowPos);
          return;
        }
        if(keycode == UP_ARROW){
          // go UP through list of previous commands
          return;
        }
        if(keycode == DOWN_ARROW){
          // go DOWN through list of previous commands
          return;
        }
        if(keycode == CAPS_KEY_CODE){
          if(caps == 0)
          {
            caps = 1;
            return;
          }
          caps = 0;
          return;
        }

        if(keycode == SHIFT_KEY_CODE){
          caps = 3;
          return;
        }

        if(keycode == 0xAA)
        {
          caps = 0;
          return;
        }

        if(buffer[77]!='\0')
        {
          moveCursor(windowPos-2);
          return;
        }

        //switch case for handling upercase
        switch (caps) {
          case 0:
            keyboardFunction(keyboard_map,keycode);
            return;
          case 1:
            keyboardFunction(caps_keyboard_map,keycode);
            return;
          case 3:
            keyboardFunction(shift_keyboard_map,keycode);
            return;
          default:
            keyboardFunction(keyboard_map,keycode);
            return;
        }
    }
}

//keyboardDrawFunction
void kprint(const char *str)
{
    unsigned int i = 0;
    while (str[i] != '\0' || str[i]) {
        videoPtr[windowPos++] = str[i++];
        videoPtr[windowPos++] = 0x30;

    }

}
//checking if print was called
int printCheck(){
  int i = 0;
  while(printString[i]!='\0'){
    if(userString[i]!=printString[i]){
      return 0;
    }
    if(userString[i]=='('&& printString[i]=='('){
      i++;
      break;
    }
    else if(userString[i]!='('&& printString[i]=='('){
      return 0;
    }
    i++;
  }
  int x = 0;
  while(userString[i]!=')'){
    if(userString[i] == '\0'){
      return 0;
    }
    stringtoPrint[x] = userString[i];
    x++;
    i++;
  }
  if(userString[i+1]=='\0'){
    return 1;
  }
  return 0;
}
//memory storage of a string from the buffer
void storeString(){
  for(int i = 0; i<79;i++){
    userString[i] = buffer[i];
  }
  return;
}
// comparison of strings
int checkString(const char* string){
  int i = 0;
  while(userString[i]!='\0'){
    if(string[i]!=userString[i]){
      return 0;
    }
    i++;
  }
  if(string[i]!='\0'){
    return 0;
  }
  return 1;
}
//cleaning the char* passed in for and empty container
void flushString(char* string)
{
  int i = 0;
  while(string[i] != '\0')
  {
    string[i] = '\0';
    i++;
  }
}
//our main function for kernel main

void kernelMain(){
    moveCursor(5000000);
    flushString(buffer);
    const char *str = "This is the Kernal Loading up..";
    clear(videoPtr);
    windowPos = 0;
    windowPos = (80*2*13)+60;
    drawSlow(str,windowPos,videoPtr,stringLocation);
    str = "....................................";
    drawSlow(str,windowPos,videoPtr,stringLocation);
    clear(videoPtr);
    windowPos = 0;
    //drawBox(videoPtr);
    str = "Welcome to OS Lite";
    windowPos += (160*13);
    windowPos += 60;
    drawSlow(str,windowPos,videoPtr,stringLocation);
    str = "Created by Matt Chris and Anthony";
    windowPos+= 320;
    draw(str,windowPos,videoPtr,stringLocation);
    str = "";
    windowPos+=160;
    draw(str,windowPos,videoPtr,stringLocation);
    str = "Press 'Enter' To Continue";
    windowPos+= 160;
    draw(str,windowPos,videoPtr,stringLocation);
    //starting the interrupt
    idt_init();
    //starting the keyboard
    kb_init();
    //while(hang==1);
    str = "";
    clear(videoPtr);
    windowPos = 0;
    moveCursor(2);
    videoPtr[windowPos++] = '>';  //write text to screen
    videoPtr[windowPos++] = 0x30; //set background color
    //while loop so we can type away.
    while(exitKernel==0);
    //outw(0xB004, 0x00002000);
    return;

}
