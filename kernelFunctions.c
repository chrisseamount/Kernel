#include "kernelFunctions.h"
#include "chell.h"
#include "kernel.h"

int lineNumber = 0;

extern struct Screen_Vars screenvars;
//custom stringLen cause nothing exists
unsigned int stringLen(const char *str){ //checks string length
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

void sleep(){ //makeshift sleep function needs some revamping
    for(int i = 0; i < 2000000; i ++){
    }
    return;
}

void clear(char *videoPtr1){ //clears screen
  lineNumber = 0;
    unsigned int dWindow1 = 0;
    while (dWindow1 < 80*25*2) {
        //printing blank character
        videoPtr1[dWindow1] = ' ';
        //setting the attribute-byte - green on black screen
        videoPtr1[dWindow1+1] = 0x02;
        dWindow1 += 2;
    }
}
void scrollScreen(char *videoPtr1){ //clears screen
    unsigned int dWindow1 = 160; // start one line in
    char tempVidPtr[25 * 80 * 2];
    //pull everything out
    while (dWindow1 < 80*25*2) {
        tempVidPtr[dWindow1-160] = videoPtr1[dWindow1];
        //setting the attribute-byte - green on black screen
        dWindow1++;
    }
    clear(screenvars.videoPtr);
    //copy it back over
    sprint(tempVidPtr);
    screenvars.windowPos = (24);
    moveCursor(screenvars.windowPos);
    screenvars.lineFlag = 1;
}
void draw(const char *str,unsigned int dWindow1,char *videoPtr1,unsigned int stringLocation1){
  // writing string to video memory
    while(str[stringLocation1] != '\0') {
        //setting char in str to char at dWindow in video memory
        videoPtr1[dWindow1] = str[stringLocation1];
        //setting attiribute-byte -green on black screen
        videoPtr1[dWindow1+1] = 0x02;
        ++stringLocation1;
        dWindow1 += 2;
    }
    stringLocation1 = 0;
}

//a terrible slow draw but it based on how good your hardware is. as the time function is just a empty for loop.
//the drawSlow function takes in a string and then prints it to the video memory
void drawSlow(const char *str,unsigned int dWindow1,char *videoPtr1,unsigned int stringLocation1){
    while(str[stringLocation1] != '\0') {
        sleep();
        //setting char in str to char at dWindow in video memory
        videoPtr1[dWindow1] = str[stringLocation1];
        //setting attiribute-byte -green on black screen
        videoPtr1[dWindow1+1] = 0x02;
        ++stringLocation1;
        dWindow1 += 2;
    }
    stringLocation1 = 0;
}

//black line top is part of the drawbox function made to make a cool loading screen to the os
void borderTop(char *videoPtr1){
    // setting the cWindow or clear window place holder to the last bit of the first line
    unsigned int dWindow1 = 158;
    //then counting down to 0 to have a cool wrap effect.
    while(dWindow1 > 0){
        sleep();
        videoPtr1[dWindow1] = ' ';
        videoPtr1[dWindow1+1] = 0x02;
        dWindow1 -=2;
    }
    // writing the information to the video memory with frist bit as char second as attribute.
    videoPtr1[dWindow1] = ' ';
    videoPtr1[dWindow1+1] = 0x02;

}
// the border starting from the first bit of the last line and moving to the right
void borderBottom(char *videoPtr1){
    unsigned int dWindow1 = (2*80*24);
     while(dWindow1 < (80*2*25)){
        sleep();
        videoPtr1[dWindow1] = ' ';
        videoPtr1[dWindow1+1] = 0x02;
        dWindow1+=2;
    }
}
// border starting at the top and going down hitting the last bits of each line going up
void borderLeft(char *videoPtr1){
    unsigned int dWindow1 = 160;
    for(int i = 0; i < 23; i++){
        sleep();
        videoPtr1[dWindow1] = ' ';
        videoPtr1[dWindow1+1] = 0x02;
        dWindow1+=160;
    }
}
// starting at the bottom and working its way up hitting the first bits of each line going down
void borderRight(char *videoPtr1){
  unsigned int dWindow1 = (80*2*25);
    for(int i = 0; i < 23; i++){
        sleep();
        dWindow1 -=162;
        videoPtr1[dWindow1] = ' ';
        videoPtr1[dWindow1+1] = 0x02;
       dWindow1 +=2;
    }
}

//draw function to make the border around the window
void drawBox(char *videoPtr1){
    borderTop(videoPtr1);
    borderLeft(videoPtr1);
    borderBottom(videoPtr1);
    borderRight(videoPtr1);
}
