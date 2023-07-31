#include <stdio.h>
#include "altera_avalon_performance_counter.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"
#include "sys/alt_alarm.h"
#include "system.h"
#include "io.h"
#include "images_alt.h"


#define TRUE 1
#define grayscale1 1
#define FLAG1 SHARED_ONCHIP_BASE             //used to indicate if data has been written in grayscale or not

#define LOC_grayscale  SHARED_ONCHIP_BASE+3
extern void delay (int millisec);

void grayscale(unsigned char* orig)
{	
	//PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, grayscale1);

	int sizeX=orig[0];
	int sizeY=orig[1];
	int fullsize=sizeX*sizeY;
	int i,j;
	//unsigned char grayscale_img[fullsize+3];
    unsigned char* grayscale_pointer = (unsigned char*) LOC_grayscale;
    unsigned char* image=grayscale_pointer;
	*grayscale_pointer++=sizeX;
	*grayscale_pointer++=sizeY;
	*grayscale_pointer++=orig[2];
	for( i=0;i<fullsize;i++)
	{
	*grayscale_pointer++=(unsigned char) 0.3125 * orig[3 * i + 3 ] + 0.5625 * orig[3 * i + 4] + 0.125  * orig[3 * i + 5];
	}
printf ("gray:\n");
for( i = 0; i < sizeY; i++)
	{
	for( j=0; j< sizeX;j++)
		{
		printf("%d ",(int)image[j+(i*sizeX)+3]);
		}
	printf("\n");
	}
	//PERF_END(PERFORMANCE_COUNTER_0_BASE, grayscale1);  
}

int main()
{
  printf("Hello from cpu_0!\n");
  int count = 0;
  int number_of_image = 4;
  while(count<4)
{ 
  if(IORD_8DIRECT(FLAG1, 0)==0)
{
  printf("%d\n",count);
  grayscale(sequence1[count]);
  IOWR_8DIRECT(FLAG1,0,1); 
  count++;
}

}
  

    while (TRUE) { /* ... */ }
   

	//PERF_RESET(PERFORMANCE_COUNTER_0_BASE);
	//PERF_START_MEASURING (PERFORMANCE_COUNTER_0_BASE);

	

  return 0;
  
}
