#include "kernelFunctions.h"
char *videoPtr = (char *) 0xb8000; //setting up video memory beginnning at 0xb8000
unsigned int dWindow = 0; // loop count for drawing video on screen.
unsigned int stringLocation = 0;
unsigned int cWindow = 0; // loop counter for clearing window/


//custom stringLen cause nothign exists
int stringLen(const char *str){ //checks string length
        int i = 0;
    while(str[i] != '\0'){
        i++;
    }
    //fixing the 2 bit size requirement for not crazy things happening with the video pointer
    if(i%2 == 1){
       i++;
    }
return i;
}

int centerLine(const char *str){  //centers text on screen
    int x = stringLen(str);
    return (80-x);
}
void newLine1(){
    unsigned int lineSize =2*80;
    dWindow = dWindow + (lineSize - dWindow % (lineSize));
}


void sleep(){ //makeshift sleep function needs some revamping
    for(int i = 0; i < 2000000; i ++){
    }
    return;
}

void clear(){ //clears screen
    dWindow = 0;
    while (dWindow < 80*25*2) {
        //printing blank character
        videoPtr[dWindow] = ' ';
        //setting the attribute-byte - green on black screen
        videoPtr[dWindow+1] = 0x30;
        dWindow += 2;
    }
    dWindow = 0;
    stringLocation = 0;
}
void draw(const char *str){
  // writing string to video memory
    while(str[stringLocation] != '\0') {
        //setting char in str to char at dWindow in video memory
        videoPtr[dWindow] = str[stringLocation];
        //setting attiribute-byte -green on black screen
        videoPtr[dWindow+1] = 0x30;
        ++stringLocation;
        dWindow += 2;
    }
    stringLocation = 0;
}

//a terrible slow draw but it based on how good your hardware is. as the time function is just a empty for loop.
//the drawSlow function takes in a string and then prints it to the video memory
void drawSlow(const char *str){
    while(str[stringLocation] != '\0') {
        sleep();
        //setting char in str to char at dWindow in video memory
        videoPtr[dWindow] = str[stringLocation];
        //setting attiribute-byte -green on black screen
        videoPtr[dWindow+1] = 0x30;
        ++stringLocation;
        dWindow += 2;
    }
    stringLocation = 0;
}


// there are 25 lines so line 13 is the middle of the screen.
void centerScreen(){
    dWindow= 0;
    for(int i = 0; i < 12; i++){
        newLine1();
    }

}
//black line top is part of the drawbox function made to make a cool loading screen to the os
void borderTop(){
    // setting the cWindow or clear window place holder to the last bit of the first line
    dWindow = 158;
    //then counting down to 0 to have a cool wrap effect.
    while(dWindow > 0){
        sleep();
        videoPtr[dWindow] = ' ';
        videoPtr[dWindow+1] = 0x00;
        dWindow -=2;
    }
    // writing the information to the video memory with frist bit as char second as attribute.
    videoPtr[dWindow] = ' ';
    videoPtr[dWindow+1] = 0x00;

}
// the border starting from the first bit of the last line and moving to the right
void borderBottom(){
    dWindow = (2*80*24);
     while(dWindow < (80*2*25)){
        sleep();
        videoPtr[dWindow] = ' ';
        videoPtr[dWindow+1] = 0x00;
        dWindow+=2;
    }
}
// border starting at the top and going down hitting the last bits of each line going up
void borderLeft(){
    dWindow = 160;
    for(int i = 0; i < 23; i++){
        sleep();
        videoPtr[dWindow] = ' ';
        videoPtr[dWindow+1] = 0x00;
        dWindow+=160;
    }
}
// starting at the bottom and working its way up hitting the first bits of each line going down
void borderRight(){
    for(int i = 0; i < 23; i++){
        sleep();
        dWindow -=162;
        videoPtr[dWindow] = ' ';
        videoPtr[dWindow+1] = 0x00;
       dWindow +=2;
    }
}

//draw function to make the border around the window
void drawBox(){
    borderTop();
    borderLeft();
    borderBottom();
    borderRight();
}
