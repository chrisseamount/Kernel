void keyboardFunction(unsigned char keyboard[128],unsigned char keycode, char* videoPtr, unsigned int windowPos)
{
  if(buffer[((windowPos%160)/2)] == '\0') // if we are at the end just work normal
  {
    buffer[((windowPos%160)/2)-1] = keyboard[keycode];
    videoPtr[windowPos++] = keyboard[keycode];  //write text to screen
    videoPtr[windowPos++] = 0x30; //set background color
  }
  else
  {//spooky code
    int stPtr = windowPos+2;
    while(buffer[((stPtr%160)/2)-1] != '\0') // find the end of the buffer
    {
      stPtr++;
    }
    while(stPtr != windowPos)
    {
      buffer[((stPtr%160)/2)-1] = buffer[((stPtr%160)/2)-2];//shift stuff around
      videoPtr[stPtr] = videoPtr[stPtr-2];
      stPtr-=2;
    }
    buffer[((windowPos%160)/2)-1] = keyboard[keycode];
    videoPtr[windowPos++] = keyboard[keycode];  //write text to screen
    videoPtr[windowPos++] = 0x30; //set background color
  }
  moveCursor(windowPos);
  return;
}

disp
