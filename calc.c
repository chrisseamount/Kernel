#include "calc.h"
#include "chell.h"

char sumArr[79];
char* add(const char* num1, const char* num2)
{
  char tempSum[79];
  flushString(sumArr);
  int dec1 = 0;
  int dec2 = 0;
  int i = 0;
  // get the actual integer value
  while(i<=79)
  {
    if(num1[i] == '\0') {break;} // if at the end of the given userString
    int n = (int)num1[i]; // get the integer as an ascii char
    n-=48; // make that the real number
    dec1=dec1*10; // times dec by ten so that it adds up correctly.
    dec1+=n;
    i++;
  }
  i = 0;
  while(i<=79)
  {
    if(num2[i] == '\0') {break;} // if at the end of the given userString
    int n = (int)num2[i]; // get the integer as an ascii char
    n-=48; // make that the real number
    dec2=dec2*10; // times dec by ten so that it adds up correctly.
    dec2+=n;
    i++;
  }
  int sum = dec1+dec2;
  int j = 0;
  while(sum>0)
  {
    tempSum[j] = (char)((sum % 10)+48);
    sum = sum / 10;
    j++;
  }

  int k = 0;
  for(int a = j-1; a>=0; a--)
  {
    sumArr[k] = tempSum[a];
    k++;
  }

  return sumArr;
}

char* sub(const char* num1, const char* num2)
{
  char tempSum[79];
  flushString(sumArr);
  int dec1 = 0;
  int dec2 = 0;
  int i = 0;
  int negFlag = 0;
  // get the actual integer value
  while(i<=79)
  {
    if(num1[i] == '\0') {break;} // if at the end of the given userString
    int n = (int)num1[i]; // get the integer as an ascii char
    n-=48; // make that the real number
    dec1=dec1*10; // times dec by ten so that it adds up correctly.
    dec1+=n;
    i++;
  }
  i = 0;
  while(i<=79)
  {
    if(num2[i] == '\0') {break;} // if at the end of the given userString
    int n = (int)num2[i]; // get the integer as an ascii char
    n-=48; // make that the real number
    dec2=dec2*10; // times dec by ten so that it adds up correctly.
    dec2+=n;
    i++;
  }

  int j = 0;

  if(dec2>dec1)
  {
    int tempdec = dec2;
    dec2 = dec1;
    dec1 = tempdec;
    negFlag = 1;
  }

  int sum = dec1-dec2;

  while(sum>0)
  {
    tempSum[j] = (char)((sum % 10)+48);
    sum = sum / 10;
    j++;
  }

  if(negFlag == 1)
  {
    tempSum[j] = '-';
    j++;
  }

  int k = 0;
  for(int a = j-1; a>=0; a--)
  {
    sumArr[k] = tempSum[a];
    k++;
  }

  return sumArr;
}
