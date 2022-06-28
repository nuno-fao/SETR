#if PCP
    #include "pcp.h"
#endif
#include "utils.h" // DO NOT MOVE THIS LINE



#if PIP
    #include "pip.h"
#endif

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
