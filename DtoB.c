//decimal to hex converter.
#include "DtoB.h"
#include "chell.h"

char binNumber[79];
char* DtoBMain(const char* stringNumber)
{
  flushString(binNumber);
  char tempbinNumber[79];
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
  int k=0;
  i = 0;
  while(dec>0)
  {
    tempbinNumber[k] = (char)((dec % 2)+48);
    dec = dec/2;
    k++;
    i++;
  }
  binNumber[k+1] = '\0';
  k = 0;
  for(int j = i-1; j>=0; j--)
  {
    binNumber[k] = tempbinNumber[j];
    k++;
  }
  return binNumber;
}
