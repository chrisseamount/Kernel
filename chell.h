//this holds the variables for easy access between files.
#pragma once
void storeString();
int printCheck();
int checkString(const char* string);
void flushString(char* string);
void toPrint(const char* string);
void chellMain();
void newLine();



struct Screen_Vars {
    char *videoPtr;
    unsigned int windowPos;
    unsigned int cWindow;
    unsigned int stringLocation;
    unsigned int exitKernel; // an exit controller
    unsigned int lineFlag;
};
