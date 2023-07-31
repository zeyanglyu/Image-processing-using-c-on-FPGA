// Skeleton for lab 2
//
// Task 1 writes periodically RGB-images to the shared memory
//
// No guarantees provided - if bugs are detected, report them in the Issue tracker of the github repository!

#include <stdio.h>
#include "altera_avalon_performance_counter.h"
#include "includes.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"
#include "sys/alt_alarm.h"
#include "system.h"
#include "io.h"
#include "images_alt.h"

#define DEBUG 1

#define HW_TIMER_PERIOD 100 /* 100ms */

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
OS_STK    task1_stk[TASK_STACKSIZE];
OS_STK    task2_stk[TASK_STACKSIZE];
OS_STK    StartTask_Stack[TASK_STACKSIZE];

/* Definition of Task Priorities */

#define STARTTASK_PRIO      1
#define TASK1_PRIORITY      10
#define TASK2_PRIORITY      11

/* Definition of Task Periods (ms) */
#define TASK1_PERIOD 10000

#define grayscale1 1
#define toascii2 2

void grayscale(unsigned char* orig)
{	
	PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, grayscale1);

	int sizeX=orig[0];
	int sizeY=orig[1];
	int fullsize=sizeX*sizeY;
	int i,j;
	unsigned char grayscale_img[fullsize+3];

	grayscale_img[0]=sizeX;
	grayscale_img[1]=sizeY;
	grayscale_img[2]=orig[2];
	for( i=0;i<fullsize;i++)
	{
		grayscale_img[i + 3]= (char)0.3125 * orig[3 * i + 3 ] + 0.5625 * orig[3 * i + 4] + 0.125  * orig[3 * i + 5];
	}
	PERF_END(PERFORMANCE_COUNTER_0_BASE, grayscale1);  
	asciiSDF(grayscale_img);
}


void asciiSDF(unsigned char* orig)
{
PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, toascii2);

int sizeX = orig[0], sizeY = orig[1];
int i,j;
int fullsize=sizeX*sizeY;
unsigned char ASCII[sizeX * sizeY+3];
char asciiLevels[16] = {' ','.',':','-','=','+','/','t','z','U','w','*','0','#','%','@'};
unsigned char* imgP;
unsigned char* share;

ASCII[0] = sizeX ;
ASCII[1] = sizeY ;
ASCII[2] = orig[2] ;
	printf ("ASCII:\n");

for( i = 0; i < sizeX * sizeY; i++)
{
ASCII[i+3] = asciiLevels[((orig[i+3])*16/255)];
}
for( i = 0; i < sizeY; i++)
	{
	for( j=0; j< sizeX;j++)
		{
		printf("%c ",ASCII[j+(i*sizeX)+3]);
		}
	printf("\n");
	}
		imgP = ASCII;
	share = (unsigned char*) SHARED_ONCHIP_BASE;
	for (i=0;i<fullsize+3;i++)
	{
		*share++ = *imgP++;
	}

	PERF_END(PERFORMANCE_COUNTER_0_BASE, toascii2);
}




int main(void) {

  printf("MicroC/OS-II-Vesion: %1.2f\n", (double) OSVersion()/100.0);
  char number_of_images =4;
  char current_image = 0;

  do
{
   /* Extract the x and y dimensions of the picture */
		

		PERF_RESET(PERFORMANCE_COUNTER_0_BASE);
		PERF_START_MEASURING (PERFORMANCE_COUNTER_0_BASE);

		/* Measurement here */
	    grayscale (sequence1[current_image]);


		/* Print report */
		perf_print_formatted_report
		(PERFORMANCE_COUNTER_0_BASE,            
		ALT_CPU_FREQ,        // defined in "system.h"
		2,                   // How many sections to print
		"grayscale",        // Display-name of section(s).
  	    
    	"toASCII" 
		);   

		/* Increment the image pointer */
		current_image=(current_image+1) % number_of_images;
}while(1);
  

  return 0;
}
