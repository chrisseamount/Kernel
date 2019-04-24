//decimal to hex converter.
#include "DtoH.h"
#include "chell.h"
char hexNumber[79];
char* DtoHMain(const char* stringNumber)
{
  char temphexNumber[79];
  int temp = 0;
  int dec = 0;
  int i = 0;
  // get the actual integer value
  while(i<=79)
  {
    if(stringNumber[i] == '\0') {break;} // if at the end of the given userString
    int n = (int)stringNumber[i]; // get the integer as an ascii char
    n-=48; // make that the real number
    dec=dec*10; // times dec by ten so that it adds up correctly.
    dec+=n;
    i++;
  }
  i = 0;
  while(dec != 0 )
  {
    temp = 0;
    temp = dec % 16;
    if(temp < 10)
    {
      temphexNumber[i] = (char)(temp + 48);
      i++;
    }
    else
    {
      temphexNumber[i] = (char)(temp + 55);
      i++;
    }
    dec = dec / 16;
  }
  //gotta flip the number around
  int k = 0;
  hexNumber[i+1] = '\0';
  for(int j = i-1; j>=0; j--)
  {
    hexNumber[k] = temphexNumber[j];
    k++;
  }
  return hexNumber;
}
