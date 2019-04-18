//called by the kernel, simply listens to the correct ports and selects the correct switch statement.
//function it calls will be in separate file. this will allow us to lock down this file.
//  functions are passed modifiers/data all at once.
//all this is going to do is take in a string and then dos tuff to it and hand it back to the kernel

#include "chell.h"
struct Screen_Vars screenvars = {(char *)0xb8000,0,0,0};
//these should be put into a file and checked against so that the user can easily add new commands.

unsigned char buffer[79]; // array for handling the input from users
unsigned char userString[79]; // array fro holding the users information
unsigned char stringtoPrint[79]; // string that is to be printed

unsigned char printString[] = "print("; // print command
unsigned char exitString[] = "exit"; // exit command
unsigned char clearString[] = "clear"; // clear command


void storeString();
int printCheck();
int checkString(const char* string);
void flushString(char* string);
void toPrint(const char* string);

void chellMain()
{
  //if userstring = exit
  if(checkString(exitString)){
    toPrint("Good bye!");
    /*const char* str = "Good Bye!";
    newLine();
    kprint(str);
    flushString(buffer);
    exitKernel = 1;
    moveCursor(50000);*/
  }
  //if  userstring = clearString
  if(checkString(clearString)){
    flushString(buffer);
  }
  //if userstring = print()
  if(printCheck()){
    /*newLine();
    kprint(stringtoPrint);
    newLine();*/
    return;
  }
}

//move string into the stringtoPrint
void toPrint(const char* string)
{
  for(int i = 0; i<79;i++){
    stringtoPrint[i] = string[i];
  }
  return;
}
//cleaning the char* passed in for and empty container
/*void flushString(char* string)
{
  int i = 0;
  while(string[i] != '\0')
  {
    string[i] = '\0';
    i++;
  }
}*/
//checking if print was called
int printCheck()
{
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
  while(userString[i]!=')' || userString[i+1]!='\0'){
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
// comparison of strings
int checkString(const char* string)
{
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
//memory storage of a string from the buffer
/*
void storeString()
{
  for(int i = 0; i<79;i++){
    userString[i] = buffer[i];
  }
  return;
}*/
