#include <stdio.h>
#include "system.h"
#include "io.h"

#define TRUE 1
#define FLAG2 SHARED_ONCHIP_BASE             //used to indicate if data has been written in grayscale or not
#define LOC_resize SHARED_ONCHIP_BASE+1030           //address where resize data is written
#define LOC_toASCII  SHARED_ONCHIP_BASE+1289          //address where toASCII data is written
extern void delay (int millisec);
void toASCII(unsigned char* orig)
{
    unsigned char* ascii_pointer = (unsigned char*) LOC_resize;
    unsigned char* image=ascii_pointer;
    char asciiLevels[16] = {' ','.',':','-','=','+','/','t','z','U','w','*','0','#','%','@'};
    char sizeX=orig[0];
	char sizeY=orig[1];
	int fullsize=sizeX*sizeY;
    unsigned char ASCII[sizeX * sizeY+3];
    ASCII[0] = sizeX ;
    ASCII[1] = sizeY ;
    ASCII[2] = orig[2] ;
	printf ("ASCII:\n");

for( i = 0; i < sizeX * sizeY; i++)
{
ASCII[i+3] = asciiLevels[(int)((orig[i+3])*16/255)];
}
for( i = 0; i < sizeY; i++)
	{
	for( j=0; j< sizeX;j++)
		{
		printf("%c ",ASCII[j+(i*sizeX)+3]);
		}
	printf("\n");
	}
    IOWR_8DIRECT(FLAG2,0,0);
}

int main()
{
  printf("Hello from cpu_2!\n");
  while(1)
  {
    if((IORD_8DIRECT(FLAG2,0)==1))     //wait for data to be written

//should it wait for next function to finish reading???

    {
      toASCII(LOC_resize);
    }
  }
  return 0;


}
