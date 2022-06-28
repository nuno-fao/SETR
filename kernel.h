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

    //run user setup code 
    setup();

    addTask(&idle,idle_stack); /* DON'T TOUCH THIS */

    //run user code
    code();
    
    
    hardwareInit(); 
    while (true) { 
        asm("nop");
    } 
 }
