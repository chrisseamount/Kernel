// Created by Matthew Perry
// Basic Kernal that makes Kernal name on page
char *videoPtr = (char *) 0xb8000; //setting up video memory beginnning at 0xb8000
unsigned int dWindow = 0; // loop count for drawing video on screen.
unsigned int stringLocation = 0;  
unsigned int cWindow = 0; // loop counter for clearing window/

int stringLen(const char *str){
        int i = 0;
    while(str[i] != '\0'){
        i++;
    }    
    if(i%2 == 1){
       i++;
    }
return i;   
}

int centerLine(const char *str){
    int x = stringLen(str);
    return (80-x);
}

void sleep(){
    for(int i = 0; i < 2000000; i ++){
    }
    return;
}

void clear(){
    cWindow = 0;
    while (cWindow < 80*25*2) {
        //printing blank character
        videoPtr[cWindow] = ' ';
        //setting the attribute-byte - green on black screen
        videoPtr[cWindow+1] = 0x30;
        cWindow += 2;
    }
    cWindow = 0;
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

void newLine(){
    unsigned int lineSize = 160;
    dWindow = dWindow + (lineSize - dWindow % (lineSize));
}

void centerScreen(){
    dWindow = 0;
    for(int i = 0; i < 12; i++){
        newLine();
    }   
}
void blackLineTop(){
    cWindow = 158;
    while(cWindow > 0){
        sleep();
        videoPtr[cWindow] = ' ';
        videoPtr[cWindow+1] = 0x00;
        cWindow -=2;
    }
    videoPtr[cWindow] = ' ';
    videoPtr[cWindow+1] = 0x00;
  
}

void blackLineBottom(){
    cWindow = (2*80*24);
     while(cWindow < (80*2*25)){
        sleep();
        videoPtr[cWindow] = ' ';
        videoPtr[cWindow+1] = 0x00;
        cWindow+=2;
    }
}

void borderLeft(){
    cWindow = 160;
    for(int i = 0; i < 23; i++){
        sleep();
        videoPtr[cWindow] = ' ';
        videoPtr[cWindow+1] = 0x00;
        cWindow+=160;
    }
}

void borderRight(){
    for(int i = 0; i < 23; i++){
        sleep();
        cWindow -=162;
        videoPtr[cWindow] = ' ';
        videoPtr[cWindow+1] = 0x00;
        cWindow +=2;
    }
}


void drawBox(){
    blackLineTop();
    borderLeft();  
    blackLineBottom();
    borderRight();
}

void kernalMain(){
    unsigned int x = 0;
    const char *str = "This is the Kernal Loading up..";
    clear();
    centerScreen();
    x = centerLine(str);
    dWindow += x;
    drawSlow(str);
    str = ".........";
    drawSlow(str);
    clear();
    drawBox();
    str = "Welcome to OS Lite";
    centerScreen();
    dWindow += centerLine(str);
    drawSlow(str);
    str = "Home";
    newLine();
    dWindow += centerLine(str);
    draw(str);
    str = "Help";
    newLine();
    dWindow += centerLine(str); 
    draw(str);
    str = "Created by Matt Chris and Anthony";
    newLine();
    newLine();
    newLine();
    dWindow += centerLine(str);
    draw(str);
    return;
    
}
