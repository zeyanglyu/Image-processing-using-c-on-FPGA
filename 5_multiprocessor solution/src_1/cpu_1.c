#include <stdio.h>
#include "system.h"
#include "io.h"

#define TRUE 1
#define FLAG1 SHARED_ONCHIP_BASE             //used to indicate if data has been written in grayscale or not
#define FLAG2 SHARED_ONCHIP_BASE             //used to indicate if data has been written in grayscale or not
#define LOC_grayscale  SHARED_ONCHIP_BASE+3
#define LOC_resize SHARED_ONCHIP_BASE+1030           //address where resize data is written
extern void delay (int millisec);
void resize(unsigned char* orig)
{
    int sizeX=orig[0];
	int sizeY=orig[1];
	int fullsize=sizeX*sizeY;
	int i,j;
	unsigned char grayscale[sizeX*sizeY+3];
    printf ("resize:\n");
    for ( i=0;i<sizeX*sizeY+3;i++)
	{
		grayscale[i]=orig[i];//copy original data
	}
   IOWR_8DIRECT(FLAG1,0,0);
   unsigned char* resize_pointer = (unsigned char*) LOC_resize;
   unsigned char* image=resize_pointer;
   resize_pointer[0]=sizeX/2;
   resize_pointer[1]=sizeY/2;
   resize_pointer[2]=orig[2];
   for ( i=0;i<sizeY;i=i+2)
	{
		for ( j=0;j<sizeX;j=j+2)
		{
		resize_pointer[j/2+i*sizeX/4+3]=(unsigned char)((grayscale[i*sizeX+j+3]+grayscale[i*sizeX+j+4]+grayscale[(i+1)*sizeX+j+3]+grayscale[(i+1)*sizeX+j+4])/4);
		}
	}
for( i = 0; i < sizeY/2; i++)
	{
	for( j=0; j< sizeX/2;j++)
		{
		printf("%d ",(int)resize_pointer[j+(i*sizeX/2)+3]);
		}
	printf("\n");
	}
   IOWR_8DIRECT(FLAG2,0,1);
}

int main()
{
  printf("Hello from cpu_1!\n");

 while(1)
  {
    if((IORD_8DIRECT(FLAG1,0)==1)&&(IORD_8DIRECT(FLAG2, 0)==0))     //wait for data to be written

//should it wait for next function to finish reading???

    {
      resize(LOC_grayscale);
    }
  }
  return 0;

}
