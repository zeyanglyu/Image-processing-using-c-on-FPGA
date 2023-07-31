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

#define graySDF1 1
#define asciiSDF2 2

void graySDF(unsigned char* orig)
{
	PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, graySDF1);

	int sizeX=orig[0];
	int sizeY=orig[1];
	int fullsize=sizeX*sizeY;
	int i,j;
	int grayscale_img[fullsize+3];
	int* imgP;
	unsigned char* share;
printf ("orig:\n");
for( i = 0; i < sizeY; i++)
	{
	for( j=0; j< sizeX;j++)
		{
		printf("%d ",(int)orig[j+(i*sizeX)+3]);
		}
	printf("\n");
	}

	grayscale_img[0]=sizeX;
	grayscale_img[1]=sizeY;
	grayscale_img[2]=orig[2];
	for(i=0;i<fullsize;i++)
	{
	//	grayscale_img[i + 3]= (0.3125 * orig[3 * i + 3 ] + 0.5625 * orig[3 * i + 4] + 0.125  * orig[3 * i + 5]);
    int t= 5*orig[3*i+3]+9*orig[3*i+4]+2*orig[3*i+5];
    grayscale_img[i+3]=t>>4;
	}
printf ("gray:\n");
for( i = 0; i < sizeY; i++)
	{
	for( j=0; j< sizeX;j++)
		{
		printf("%d ",grayscale_img[j+(i*sizeX)+3]);
		}
	printf("\n");
	}

	imgP = grayscale_img;
	share = (unsigned char*) SHARED_ONCHIP_BASE;
	for (i=0;i<fullsize+3;i++)
	{
		*share++ = (unsigned char)*imgP++;
	}
	PERF_END(PERFORMANCE_COUNTER_0_BASE, graySDF1);
//	resize (grayscale_img);

}

void asciiSDF(unsigned char* orig)
{
PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, asciiSDF2);

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
ASCII[i+3] = asciiLevels[((orig[i+3])/16)];
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

	PERF_END(PERFORMANCE_COUNTER_0_BASE, asciiSDF2);
}


/*
 * Example function for copying a p3 image from sram to the shared on-chip mempry
 */
 /*
void sram2sm_p3(unsigned char* base)
{
	int x, y;
	unsigned char* shared;

	shared = (unsigned char*) SHARED_ONCHIP_BASE;

	int size_x = *base++;
	int size_y = *base++;
	int max_col= *base++;
	*shared++  = size_x;
	*shared++  = size_y;
	*shared++  = max_col;
	printf("The image is: %d x %d!! \n", size_x, size_y);
	for(y = 0; y < size_y; y++)
	for(x = 0; x < size_x; x++)
	{
		*shared++ = *base++; 	// R
		*shared++ = *base++;	// G
		*shared++ = *base++;	// B
	}
}

*/
/*
 * Global variables
 */
int delay; // Delay of HW-timer

/*
 * ISR for HW Timer
 */
alt_u32 alarm_handler(void* context)
{
  OSTmrSignal(); /* Signals a 'tick' to the SW timers */

  return delay;
}

// Semaphores
OS_EVENT *Task1TmrSem;
OS_EVENT *Task1Sem;
OS_EVENT *Task2Sem;

// SW-Timer
OS_TMR *Task1Tmr;


/* Timer Callback Functions */
void Task1TmrCallback (void *ptmr, void *callback_arg){
  OSSemPost(Task1TmrSem);
}

void task1(void* pdata)
{
	INT8U err;
	INT8U value=0;
	INT8U current_image=0;

	while (1)
	{
		/* Extract the x and y dimensions of the picture */
		
		OSSemPend(Task1Sem,0,&err);

		PERF_RESET(PERFORMANCE_COUNTER_0_BASE);
		PERF_START_MEASURING (PERFORMANCE_COUNTER_0_BASE);
	//	PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, SECTION_1);

		/* Measurement here */
		graySDF(sequence1[current_image]);

		//PERF_END(PERFORMANCE_COUNTER_0_BASE, SECTION_1);

		/* Print report */
	//	perf_print_formatted_report
	//	(PERFORMANCE_COUNTER_0_BASE,
	//	ALT_CPU_FREQ,        // defined in "system.h"
	//	1,                   // How many sections to print
	//	"Section 1"        // Display-name of section(s).
	//	);

		/* Just to see that the task compiles correctly */
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE,value++);

		OSSemPost(Task2Sem);

		/* Increment the image pointer */
		current_image=(current_image+1) % sequence1_length;

	}
}

void task2(void* pdata) {
	INT8U err;
	unsigned char* share;
	share = (unsigned char*) SHARED_ONCHIP_BASE;
	while(1)
	{
	OSSemPend(Task2Sem, 0, &err);

	asciiSDF(share);


		/* Print report */
		perf_print_formatted_report
		(PERFORMANCE_COUNTER_0_BASE,
		ALT_CPU_FREQ,        // defined in "system.h"
		2,                   // How many sections to print
		"rgbToGray",        // Display-name of section(s).

		"printAscii"
		);
	OSSemPost(Task1Sem);
	}
}

void StartTask(void* pdata)
{
  INT8U err;
  void* context;

  static alt_alarm alarm;     /* Is needed for timer ISR function */

  /* Base resolution for SW timer : HW_TIMER_PERIOD ms */
  delay = alt_ticks_per_second() * HW_TIMER_PERIOD / 1000;
  printf("delay in ticks %d\n", delay);

  /*
   * Create Hardware Timer with a period of 'delay'
   */
  if (alt_alarm_start (&alarm,
      delay,
      alarm_handler,
      context) < 0)
      {
          printf("No system clock available!n");
      }

  /*
   * Create and start Software Timer
   */

   //Create Task1 Timer
   Task1Tmr = OSTmrCreate(0, //delay
                            TASK1_PERIOD/HW_TIMER_PERIOD, //period
                            OS_TMR_OPT_PERIODIC,
                            Task1TmrCallback, //OS_TMR_CALLBACK
                            (void *)0,
                            "Task1Tmr",
                            &err);

   if (DEBUG) {
    if (err == OS_ERR_NONE) { //if creation successful
      printf("Task1Tmr created\n");
    }
   }


   /*
    * Start timers
    */

   //start Task1 Timer
   OSTmrStart(Task1Tmr, &err);

   if (DEBUG) {
    if (err == OS_ERR_NONE) { //if start successful
      printf("Task1Tmr started\n");
    }
   }


   /*
   * Creation of Kernel Objects
   */

  Task1TmrSem = OSSemCreate(1);
	Task1Sem = OSSemCreate(1);
	Task2Sem = OSSemCreate(0);

  /*
   * Create statistics task
   */

  OSStatInit();

  /*
   * Creating Tasks in the system
   */

  err=OSTaskCreateExt(task1,
                  NULL,
                  (void *)&task1_stk[TASK_STACKSIZE-1],
                  TASK1_PRIORITY,
                  TASK1_PRIORITY,
                  task1_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  if (DEBUG) {
     if (err == OS_ERR_NONE) { //if start successful
      printf("Task1 created\n");
    }
   }

	 err=OSTaskCreateExt(task2,
	 								NULL,
	 								(void *)&task2_stk[TASK_STACKSIZE-1],
	 								TASK2_PRIORITY,
	 								TASK2_PRIORITY,
	 								task2_stk,
	 								TASK_STACKSIZE,
	 								NULL,
	 								0);

	 if (DEBUG) {
	 	 if (err == OS_ERR_NONE) { //if start successful
	 		printf("Task2 created\n");
	 	}
	  }

  printf("All Tasks and Kernel Objects generated!\n");

  /* Task deletes itself */

  OSTaskDel(OS_PRIO_SELF);
}


int main(void) {

  printf("MicroC/OS-II-Vesion: %1.2f\n", (double) OSVersion()/100.0);

  OSTaskCreateExt(
	 StartTask, // Pointer to task code
         NULL,      // Pointer to argument that is
                    // passed to task
         (void *)&StartTask_Stack[TASK_STACKSIZE-1], // Pointer to top
						     // of task stack
         STARTTASK_PRIO,
         STARTTASK_PRIO,
         (void *)&StartTask_Stack[0],
         TASK_STACKSIZE,
         (void *) 0,
         OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

  OSStart();

  return 0;
}
