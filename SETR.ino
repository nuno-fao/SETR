#define NORMAL 1
#define PIP 0
#define DEADLOCK 0
#define PCP 0

#define TICK_FREQUENCY      625
#define STACK_SIZE_DEFAULT  100
#define MAX_TASKS           20
#define MAX_SEMAPHORES      2

#include "kernel.h"

#define d1  11
#define d2  12
#define d3  13
#define d4  10

void asyncDelay(int ms){
    tasks[current_task]->state = TASK_READY;
    tasks[current_task]->delay+=DELAY_TO_TICKS(ms);
    finish_task();
}

volatile void delay(){

    digitalWrite(d4, !digitalRead(d4));
    unsigned volatile int i = 0;
    unsigned volatile int j = 0;
    unsigned volatile int t = 0;
    unsigned volatile int u = 0;

    while(t<50000){
        while(j<50000){
            while(i<50000){
                while(u<50000){
                    asm("nop;");
                    u++;
                }
                i++;
            }
            j++;
        }
        t++;
    }
    

}

#if NORMAL
    void task1(void);
    TASK(t1, 1, 1000,  0, STACK_SIZE_DEFAULT,task1);
    void task1(void) { 
            digitalWrite(d3, !digitalRead(d3));    // Toggle
    } 


    void task2(void);
    TASK(t2, 2, 1000, 2000, STACK_SIZE_DEFAULT, task2);
    void task2(void) { 
            digitalWrite(d2, !digitalRead(d2));
    } 

    void task3(void);
    TASK(t3, 3, 500, 0, STACK_SIZE_DEFAULT, task3);
    void task3(void) { 
            digitalWrite(d1, !digitalRead(d1));    // Toggle
    } 
#endif

#if PIP
    CREATE_SEMAPHORE(s1);
    CREATE_SEMAPHORE(s2);
    void task1(void);
    TASK(t1, 1, 1000,  0, STACK_SIZE_DEFAULT,task1);
    #if DEADLOCK
        void task1(void) { 
                LOCK(s1,t1);
                LOCK(s2,t1);
                digitalWrite(d3, !digitalRead(d3));    // Toggle
                UNLOCK(s2);
                UNLOCK(s1);
        } 
    #else
        void task1(void) { 
                LOCK(s1,t1);
                digitalWrite(d3, !digitalRead(d3));    // Toggle
                UNLOCK(s1);
        } 
    #endif
    void task2(void);
    TASK(t2, 2, 1000, 2000, STACK_SIZE_DEFAULT, task2);
    void task2(void) { 
            digitalWrite(d2, !digitalRead(d2));
    } 

    void task3(void);
    TASK(t3, 3, 5000, 0, STACK_SIZE_DEFAULT, task3);
    #if DEADLOCK
        void task3(void) { 
                LOCK(s2,t3);
                for(int i = 0;i< 10;i++){
                    delay();
                }
                LOCK(s1,t3);
                //asyncDelay(4000);
                digitalWrite(d1, !digitalRead(d1));    // Toggle
                UNLOCK(s2);
                UNLOCK(s1);
        } 
    #else
        void task3(void) { 
            LOCK(s1,t3);
            for(int i = 0;i< 10;i++){
                delay();
            }
            digitalWrite(d1, !digitalRead(d1));    // Toggle
            UNLOCK(s1);
        } 
    #endif
#endif

#if PCP
    CREATE_SEMAPHORE(s1);
    CREATE_SEMAPHORE(s2);
    
    void task1(void);
    TASK(t1, 1, 1000,  0, STACK_SIZE_DEFAULT,task1);
    #if DEADLOCK
        void task1(void) {
                DECLARE(s1,t1);
                DECLARE(s2,t1);
                LOCK(s1,t1);
                LOCK(s2,t1);
                digitalWrite(d3, !digitalRead(d3));    // Toggle
                UNLOCK(s2,t1);
                UNLOCK(s1,t1);
        } 
    #else
        void task1(void) { 
                DECLARE(s1,t1);
                LOCK(s1,t1);
                digitalWrite(d3, !digitalRead(d3));    // Toggle
                UNLOCK(s1,t1);
        } 
    #endif

    void task2(void);
    TASK(t2, 2, 1000, 2000, STACK_SIZE_DEFAULT, task2);
    void task2(void) { 
            digitalWrite(d2, !digitalRead(d2));
    } 

    void task3(void);
    TASK(t3, 3, 5000, 0, STACK_SIZE_DEFAULT, task3);
    #if DEADLOCK
        void task3(void) { 
                DECLARE(s1,t3);
                DECLARE(s2,t3);
                LOCK(s2,t3);
                for(int i = 0;i< 10;i++){
                    delay();
                }
                LOCK(s1,t3);
                digitalWrite(d1, !digitalRead(d1));    // Toggle
                UNLOCK(s2,t3);
                UNLOCK(s1,t3);
        } 
    #else
        void task3(void) { 
            DECLARE(s1,t3);
            LOCK(s1,t3);
            for(int i = 0;i< 10;i++){
                delay();
            }
            digitalWrite(d1, !digitalRead(d1));    // Toggle
            UNLOCK(s1,t3);
        } 
    #endif
#endif

void setupFunction() { 
    /********************** CONFIGURE REQUIRED HARDWARE **********************/
    pinMode(d3, OUTPUT);
    pinMode(d2, OUTPUT);
    pinMode(d1, OUTPUT);
    pinMode(d4, OUTPUT);


    #if PCP
        #if DEADLOCK
            ADD_SEMAPHORE(s1);
            ADD_SEMAPHORE(s2);
        #else
            ADD_SEMAPHORE(s1);
        #endif
    #endif
}

void codeFunction(){
    addTask(&t1,t1_stack);
    addTask(&t2,t2_stack);
    addTask(&t3,t3_stack);
}

int main(){
    kernel(setupFunction,codeFunction);
}
