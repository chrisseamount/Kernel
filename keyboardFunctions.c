#include "keyboardFunctions.h"
#include "chell.h"

extern struct Screen_Vars screenvars;

void keyboardFunction(unsigned char keyboard[128],unsigned char keycode,unsigned char buffer[79])
{
  if(buffer[((screenvars.windowPos%160)/2)] == '\0') // if we are at the end just work normal
  {
    dispText(keyboard, keycode, buffer);
  }
  else
  {//spooky code
    int stPtr = screenvars.windowPos+2;
    while(buffer[((stPtr%160)/2)-1] != '\0') // find the end of the buffer
    {
      stPtr++;
    }
    while(stPtr != screenvars.windowPos)
    {
      buffer[((stPtr%160)/2)-1] = buffer[((stPtr%160)/2)-2];//shift stuff around
      screenvars.videoPtr[stPtr] = screenvars.videoPtr[stPtr-2];
      stPtr-=2;
    }
    dispText(keyboard, keycode, buffer);
  }
  return;
}
void dispText(unsigned char keyboard[128],unsigned char keycode,unsigned char buffer[79])
{
  buffer[((screenvars.windowPos%160)/2)-1] = keyboard[keycode];
  screenvars.videoPtr[screenvars.windowPos++] = keyboard[keycode];  //write text to screen
  screenvars.videoPtr[screenvars.windowPos++] = 0x02; //set background color
}
