//this holds the variables for easy access between files.
#pragma once
struct Screen_Vars {
    char *videoPtr;
    unsigned int windowPos;
    unsigned int cWindow;
    unsigned int stringLocation;
};
struct Screen_Vars screenvars = {(char *)0xb8000,0,0,0};
