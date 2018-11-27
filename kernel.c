// Created by Matthew Perry
// Basic Kernal that makes Kernal name on page

void sleep(void){
    for(int i = 0; i < 20000000; i ++){
    }
    return;
}


void kernalMain(void){
    const char *str = "This is the Kernal Loading up.";
    char *videoPtr = (char *) 0xb8000; //setting up video memory beginnning at 0xb8000
    unsigned int cWindow = 0; // loop counter for clearing window/
    unsigned int dWindow = 0; // loop count for drawing video on screen.
    unsigned int stringLocation = 0;   
     
    while (cWindow < 80*25*2) {
        //printing blank character
        videoPtr[cWindow] = ' ';
        //setting the attribute-byte - green on black screen
        videoPtr[cWindow+1] = 0x02;
        cWindow += 2;
    }
    

    // writing string to video memory
    while(str[stringLocation] != '\0') {
        //setting char in str to char at dWindow in video memory
        videoPtr[dWindow] = str[stringLocation];
        //setting attiribute-byte -green on black screen
        videoPtr[dWindow+1] = 0x02;
        ++stringLocation;
        dWindow += 2;
    }
    str = "...........";
    stringLocation = 0;
    while(str[stringLocation] != '\0') {
        sleep();
        //setting char in str to char at dWindow in video memory
        videoPtr[dWindow] = str[stringLocation];
        //setting attiribute-byte -green on black screen
        videoPtr[dWindow+1] = 0x02;
        ++stringLocation;
        dWindow += 2;
    }
    
    cWindow = 0;
    while (cWindow < 80*25*2) {
        //printing blank character
        videoPtr[cWindow] = ' ';
        //setting the attribute-byte - green on black screen
        videoPtr[cWindow+1] = 0x02;
        cWindow += 2;
    }

    str = "Welcome to your kernel!";
    dWindow = 40;
    stringLocation = 0;
    while(str[stringLocation] != '\0') {
        sleep();
        //setting char in str to char at dWindow in video memory
        videoPtr[dWindow] = str[stringLocation];
        //setting attiribute-byte -green on black screen
        videoPtr[dWindow+1] = 0x04;
        ++stringLocation;
        dWindow += 2;
    }
    sleep();
    cWindow = 0;
    while (cWindow < 80*25*2) {
        //printing blank character
        videoPtr[cWindow] = ' ';
        //setting the attribute-byte - green on black screen
        videoPtr[cWindow+1] = 0x02;
        cWindow += 2;
    }



    return;
    
}
