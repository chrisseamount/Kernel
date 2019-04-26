//called by the kernel, simply listens to the correct ports and selects the correct switch statement.
//function it calls will be in separate file. this will allow us to lock down this file.
//  functions are passed modifiers/data all at once.
//all this is going to do is take in a string and then dos tuff to it and hand it back to the kernel

#include "chell.h"
#include "kernelFunctions.h"
#include "keyboardFunctions.h"
#include "DtoH.h"
#include "DtoB.h"
#include "calc.h"
struct Screen_Vars screenvars = {(char *)0xb8000,0,0,0,0,0,0};
//these should be put into a file and checked against so that the user can easily add new commands.

unsigned char buffer[79]; // array for handling the input from users
unsigned char userString[79]; // array fro holding the users information
unsigned char stringtoPrint[79]; // string that is to be printed
char retString[79];
unsigned char word[] = "";
char command[] = "";
extern int lineNumber;


extern void moveCursor(unsigned int drawWindow);
void storeString();
int printCheck();
int checkString(const char* string);
void flushString(char* string);
void toPrint(const char* string);
void split(char string[]);
void newLine();
void stringCon(char* string1, char* string2);

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
  else if(checkString("clear"))
  {
    flushString(buffer);
    clear(screenvars.videoPtr);
    screenvars.windowPos = 0;
    moveCursor(screenvars.windowPos);
    screenvars.lineFlag = 1;
    return;
  }
  //if userstring = print()
  else if(checkString("print"))
  {
    newLine();
    toPrint(userString);
    return;
  }
  //if userstring = hex
  else if(checkString("hex"))
  {
    newLine();
    toPrint(DtoHMain(userString));
    return;
  }
  //if userstring = bin
  else if(checkString("bin"))
  {
    newLine();
    toPrint(DtoBMain(userString));
    return;
  }
  //if userstring = help
  else if(checkString("help"))
  {
    screenvars.helpFlag = 1;
    return;
  }
  //if userstring = add
  else if(checkString("add"))
  {
    newLine();
    split(userString);
    toPrint(add(word,userString));
    return;
  }
  //if userstring = mul
  else if(checkString("mul"))
  {
    newLine();
    split(userString);
    toPrint(mul(word,userString));
    return;
  }
  //if userstring = sub
  if(checkString("sub"))
  {
    newLine();
    split(userString);
    toPrint(sub(word,userString));
    return;
  }
  else
  {
    newLine();
    checkString(userString);
    stringCon("Unrecognized Command: \"" ,word);
    stringCon(retString, "\"");
    toPrint(retString);
    flushString(retString);
    return;
  }
}

void stringCon(char* string1, char* string2)
{
  int i = 0;
  while (string1[i] != '\0')
  {
    retString[i] = string1[i];
    i++;
  }
  int k = 0;
  while(i < 79)
  {
    retString[i] = string2[k];
    i++;
    k++;
  }
  retString[i+1] = '\0';
  return;
}
//take away only the first word. get rid of following space.
void split(char string[])
{
  flushString(word);
  unsigned char restOfString[79];
  flushString(restOfString);
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
// every time this is hit we should copy what is behind it this will allow us
// to know what ison the screen and what commands have been entered
void newLine()
{

    unsigned int lineSize = 160;
    screenvars.windowPos = screenvars.windowPos + (lineSize - screenvars.windowPos % (lineSize));

    if(screenvars.windowPos>=80*25*2)
    {
      scrollScreen(screenvars.videoPtr);
    }
}
