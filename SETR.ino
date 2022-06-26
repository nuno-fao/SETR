#include "kernel.h"

void task1(void) { 
    while (true) { 
        /*PLACE CODE HERE*/
        digitalWrite(d3, !digitalRead(d3));    // Toggle
        /*DON'T TOUCH PAST THIS LINE*/
        finish_task();
    } 
    return; 
} 
TASK(t1, 1, Hz_1,  0, STACK_SIZE_DEFAULT,&task1);


void delay(int ms){
    tasks[current_task]->state = TASK_WAITING;
    tasks[current_task]->delay+=DELAY_TO_TICKS(ms);
    finish_task();
}

void task2(void) { 
    while (true) { 
        /*PLACE CODE HERE*/
        digitalWrite(d2, !digitalRead(d2));
        /*DON'T TOUCH PAST THIS LINE*/
        finish_task();
    } 
    return; 
} 
TASK(t2, 2, Hz_1, 2000, STACK_SIZE_DEFAULT, &task2);


void task3(void) { 
    while (true) { 
        /*PLACE CODE HERE*/
        digitalWrite(d1, !digitalRead(d1));    // Toggle
        /*DON'T TOUCH PAST THIS LINE*/
        finish_task();
    } 
    return; 
} 
TASK(t3, 3, Hz_2, 0, STACK_SIZE_DEFAULT, &task3);


void setupFunction() { 
    /********************** CONFIGURE REQUIRED HARDWARE **********************/
    pinMode(d3, OUTPUT);
    pinMode(d2, OUTPUT);
    pinMode(d1, OUTPUT);
    pinMode(10, OUTPUT);
}

void codeFunction(){
    addTask(&t1,t1_stack);
    addTask(&t2,t2_stack);
    addTask(&t3,t3_stack);
}

int main(){
    kernel(setupFunction,codeFunction);
}