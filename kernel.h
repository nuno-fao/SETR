#include "ticks_per_seconds.h"



#include "utils.h" // DO NOT MOVE THIS LINE
#include "pip.h"



typedef struct {
      Task *task;
      uint8_t uuid;
} _semaphore;


void iddle_task(void) { 
     finish_task();
} 
TASK(idle, 255, 0, 0, 40, iddle_task);


    

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
