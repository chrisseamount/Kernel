#include "keyboardFunctions.h"
#include "definitions.h"

unsigned char buffer[79];

void flushBuffer()
{
  int i = 0;
  while(buffer[i] != '\0')
  {
    buffer[i] = '\0';
    i++;
  }
}

void kb_init(void)
{
    /* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/
    write_port(0x21 , 0xFD);
}

//newline function
void newLine(){
    unsigned int lineSize = BYTES_ELEMENT*COLUMNS_LINE;
    dWindow = dWindow + (lineSize - dWindow % (lineSize));
}

void moveCursor(unsigned int drawWindow){
  unsigned short cursorLocation = (drawWindow/2);
  write_port(0x3D4 ,14);
  write_port(0x3D5, cursorLocation>>8);
  write_port(0x3D4, 15);
  write_port(0x3D5, cursorLocation);
}

//keyboardDrawFunction
void kprint(const char *str)
{
    unsigned int i = 0;
    while (str[i] != '\0' || str[i]) {
        videoPtr[dWindow++] = str[i++];
        videoPtr[dWindow++] = 0x30;
    }
}

void keyboard_handler_main()
{
    unsigned char status;
    char keycode;

    /* write EOI */
    write_port(0x20, 0x20);
    status = read_port(KEYBOARD_STATUS_PORT);
    /* Lowest bit of status will be set if buffer is not empty */
    if (status & 0x01) {
        keycode = read_port(KEYBOARD_DATA_PORT);
        if(keycode < 0){
            return;
        }
        if(keycode == BACKSPACE_KEY_CODE){  //checks for backspace key and replaces text on screen with a space
            if(dWindow == 0)  //if at top left corner dont do anything
            {
              return;
            }
            dWindow-=2;
            buffer[((dWindow%160)/2)-1] = '\0';
            videoPtr[dWindow]= ' ';
            moveCursor(dWindow);

            return;
        }
        if(keycode == ENTER_KEY_CODE) { //newlines if enter key is pressed
            newLine();
            kprint(buffer);
            flushBuffer();
            newLine();
            videoPtr[dWindow++] = '>';  //write text to screen
            videoPtr[dWindow++] = 0x30; //set background color
            moveCursor(dWindow);
            return;
        }
        buffer[((dWindow%160)/2)-1] = keyboard_map[(unsigned char) keycode];
        videoPtr[dWindow++] = keyboard_map[(unsigned char) keycode];  //write text to screen
        videoPtr[dWindow++] = 0x30; //set background color
        moveCursor(dWindow);
    }
}
