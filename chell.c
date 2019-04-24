//called by the kernel, simply listens to the correct ports and selects the correct switch statement.
//function it calls will be in separate file. this will allow us to lock down this file.
//  functions are passed modifiers/data all at once.
//all this is going to do is take in a string and then dos tuff to it and hand it back to the kernel

#include "chell.h"
#include "kernelFunctions.h"
#include "keyboardFunctions.h"
#include "DtoH.h"
struct Screen_Vars screenvars = {(char *)0xb8000,0,0,0,0,0};
//these should be put into a file and checked against so that the user can easily add new commands.

unsigned char buffer[79]; // array for handling the input from users
unsigned char userString[79]; // array fro holding the users information
unsigned char stringtoPrint[79]; // string that is to be printed
unsigned char word[] = "";


extern void moveCursor(unsigned int drawWindow);
void storeString();
int printCheck();
int checkString(const char* string);
void flushString(char* string);
void toPrint(const char* string);
void split(char string[]);
void newLine();

void chellMain()
{
  //take the string and break into function part, then make the data bits
  //split debugging
  split(userString);

  //if userstring = exit
  if(checkString("exit"))
  {
    newLine();
    toPrint("Good bye!");
    screenvars.exitKernel = 1;
    return;
  }
  //if  userstring = clearString
  if(checkString("clear"))
  {
    flushString(buffer);
    clear(screenvars.videoPtr);
    screenvars.windowPos = 0;
    moveCursor(screenvars.windowPos);
    screenvars.lineFlag = 1;
    return;
  }
  //if userstring = print()
  if(checkString("print"))
  {
    newLine();
    toPrint(userString);
    return;
  }
  if(checkString("hex"))
  {
    newLine();
    DtoHMain(userString);
    return;
  }


}

void split(char string[])
{
  flushString(word);
  unsigned char restOfString[79];
  int i = 0;
  while(string[i] != ' ' && i<=79)
  {
    word[i] = string[i]; // get the first word
    i++;
  }
  i++;
  int j = i;
  while(i<=79)
  {
    restOfString[i-j] = string[i]; // move everything else down the line
    i++;
  }
  i = 0;
  while(i<=79)
  {
    string[i] = restOfString[i]; //put it back into the string
    i++;
  }
}
//move string into the stringtoPrint
void toPrint(const char* string)
{
  for(int i = 0; i<79;i++)
  {
    stringtoPrint[i] = string[i];
  }
  return;
}
// comparison of strings
int checkString(const char* string)
{
  int i = 0;
  while(word[i]!='\0'){
    if(string[i]!=word[i]){
      return 0;
    }
    i++;
  }
  if(string[i]!='\0'){
    return 0;
  }
  return 1;
}
//newline function
void newLine()
{
    unsigned int lineSize = 160;
    screenvars.windowPos = screenvars.windowPos + (lineSize - screenvars.windowPos % (lineSize));
}
