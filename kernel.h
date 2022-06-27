#include "ticks_per_seconds.h"

/*****************************************************************************/
/******************************* NECESSARY GLOBAL VARS *******************************/
/*****************************************************************************/
#define TICK_FREQUENCY      625
#define STACK_SIZE_DEFAULT  100
#define MAX_TASKS           20
/*****************************************************************************/

// Include kernel code
#include "utils.h" // DO NOT MOVE THIS LINE


#define d1  11
#define d2  12
#define d3  13

void iddle_task(void) { 
    while (true) { 
        asm("nop");
        finish_task();
    } 
    return; 
} 
TASK(idle, 255, 0, 0, 40, &iddle_task);



/**************************** ADD YOUR TASKS HERE ****************************/


// void name##_f(void) __attribute__ ( ( OS_task ) );
//  void name##_f(void) { 
//     while (true) { 
//         /*PLACE CODE HERE*/

//         /*DON'T TOUCH PAST THIS LINE*/
//          finish_task();
//     } 
//     return; 
//  } 
//  TASK(example, 1, Hz_1, &name##_f);

    

int kernel(void setup(),void code()) { 
    /********************** CONFIGURE REQUIRED HARDWARE **********************/

    setup();

    /********************* END OF HARDWARE CONFIGURATION *********************/

    addTask(&idle,idle_stack); /* DON'T TOUCH THIS */

    /********************* REGISTER TASKS IN THE KERNEL **********************/

    code();
    
    /***************************** END OF CODE *******************************/
    
    hardwareInit(); 
    while (true) { 
        asm("nop"); /* interrrupts will stop this. */ 
    } 
 }
