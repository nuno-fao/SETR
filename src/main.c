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

void iddle_task(void) __attribute__ ( ( OS_task ) );
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


void task1(void) __attribute__ ( ( OS_task ) );
void task1(void) { 
    while (true) { 
        /*PLACE CODE HERE*/
        digitalWrite(13, !digitalRead(13));    // Toggle
        /*DON'T TOUCH PAST THIS LINE*/
        finish_task();
    } 
    return; 
} 

TASK(t1, 1, Hz_1,  0, STACK_SIZE_DEFAULT,&task1);


void task2(void) __attribute__ ( ( OS_task ) );
void task2(void) { 
    while (true) { 
        /*PLACE CODE HERE*/
        digitalWrite(12, !digitalRead(12));    // Toggle
        /*DON'T TOUCH PAST THIS LINE*/
        finish_task();
    } 
    return; 
} 
TASK(t2, 2, Hz_1, 2000, STACK_SIZE_DEFAULT, &task2);


void task3(void) __attribute__ ( ( OS_task ) );
void task3(void) { 
    while (true) { 
        /*PLACE CODE HERE*/
        digitalWrite(11, !digitalRead(11));    // Toggle
        /*DON'T TOUCH PAST THIS LINE*/
        finish_task();
    } 
    return; 
} 
TASK(t3, 3, Hz_2, 0, STACK_SIZE_DEFAULT, &task3);


/***************************** END OF TASKS CODE *****************************/

int main() { 

    /********************** CONFIGURE REQUIRED HARDWARE **********************/

    pinMode(13, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(10, OUTPUT);

    

    /********************* END OF HARDWARE CONFIGURATION *********************/

    addTask(&idle,idle_stack); /* DON'T TOUCH THIS */

    /********************* REGISTER TASKS IN THE KERNEL **********************/

    addTask(&t1,t1_stack);
    addTask(&t2,t2_stack);
    addTask(&t3,t3_stack);
    
    /***************************** END OF CODE *******************************/
    
    hardwareInit(); 
    while (true) { 
        asm("nop"); /* interrrupts will stop this. */ 
    } 
 }