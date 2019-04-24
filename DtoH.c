//decimal to hex converter.
#include "DtoH.h"
#include "chell.h"
char hexNumber[79];
void DtoHMain(const char* decNumber)
{
  int temp = 0;
  int dec = 0;
  int i = 0;
  // get the actual integer value
  while(i<=79)
  {
    if(decNumber[i] == '\0') {break;}
    int n = (int)decNumber[i];
    n-=30;
    dec=dec*10;
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
      hexNumber[i] = (char)(temp + 48);
      i++;
    }
    else
    {
      hexNumber[i] = (char)(temp + 65);
      i++;
    }
    dec = dec / 16;
  }
  //gotta flip the number around
  for(int j = i-1; j>=0; j--)
  {
    char x[2];
    x[0] = hexNumber[j];
    x[1] = '\0';
    toPrint(x);
  }
  return;
}
