// Created by Matthew Perry, Anthony Kolendo and Chris Seamount
// Basic Kernal that makes Kernal name on page
#include "keyboard_map.h" // header holding the keyboard_map
#include "kernelFunctions.h"
#include "initPIC.h"
#include "keyboardFunctions.h"
#include "definitions.h"
//Keyboard definitions
//In an effort to get keyboard functionality we took code from another source, after looking around there seems to be alot of the same code passed around.
//Keyboard driver was taken from Arjun Sreedharan as he gave a great article on explaining what is occuring in the code.
//Comments were added to work though understanding the driver
// this section of defines is for creating the window size of 25 lines but 80 columns with each column containing 2 bits

//keyboard map is a table created in correlation with the keycode recived from the keyboard hardware from ports

//our main function for kernel main

void kernelMain(){
    moveCursor(5000000);
    flushBuffer();
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
    moveCursor(2);
    idt_init();
    kb_init();
    videoPtr[dWindow++] = '>';  //write text to screen
    videoPtr[dWindow++] = 0x30; //set background color
    //this is the keyboard being booted up
    //kprint(str);
    //while loop so we can type away.
    while(1);
    return;

}
