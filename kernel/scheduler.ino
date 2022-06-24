int Sched_Init(void){
    for(int x=0; x<NT; x++)
        Tasks[x].func = 0;
    /* Also configures interrupt that periodically calls Sched_Schedule(). */
    noInterrupts(); // disable all interrupts
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    OCR1A = TICK_FREQUENCY;    // compare match register 16MHz/256/2kHz
    TCCR1B |= (1 << WGM12); // CTC mode
    TCCR1B |= (1 << CS12); // 256 prescaler
    TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
    interrupts(); // enable all interrupts
}

int Sched_AddT(void (*f)(void), int delay, int period, int priority, int stack_size, uint8_t* stack_start){ //TODO ordenar ao adicionar
    for(int x=0; x<NT; x++)
        if (!Tasks[x].func) {
            Tasks[x].period = period;
            Tasks[x].delay = delay;
            Tasks[x].exec = 0;
            Tasks[x].func = f;
            Tasks[x].priority = priority;
            Tasks[x].stack_ptr = pxPortInitialiseStack(stack_start+stack_size, f, 0);
            Tasks[x].stack_size = stack_size;
            Tasks[x].stack_array_ptr = stack_start;
            Tasks[x].state = DONE;
            orderTasks();
            return x;
        }
    orderTasks();
    return -1;
}


void Sched_Schedule(void){
    Serial.print("cona");
    for(int x=0; x<NT; x++) {
        if(Tasks[x].func){
            if(Tasks[x].delay){
                Tasks[x].delay--;
            } else {
                /* Schedule Task */
                Tasks[x].exec++;
                Tasks[x].state = READY;
                Tasks[x].delay = Tasks[x].period-1;
            }
        }
    }
}


void Sched_Dispatch(void){
    int prev_task = cur_task;
    for(int x=0; x<cur_task; x++) {
        if((Tasks[x].func)&&(Tasks[x].state == READY)) {
            
            Tasks[x].state=RUNNING;
            cur_task = x;
            pxCurrentTCB = Tasks[x].stack_ptr;
            /* Delete task if one-shot */
            if(!Tasks[x].period) removeTask(x);
        }
    }
}

void removeTask(int x){
    Tasks[x].func = 0;
    for (int i = x+1; i < NT; ++i) {
        Tasks[i-1] = Tasks[i];
    }
    Tasks[NT] = (Sched_Task_t){0};
}

void orderTasks(){
    int swapped = 1;
    while(swapped){
        swapped = 0;
        for (int i = 0; i < NT-1; ++i) {
            if(Tasks[i].priority < Tasks[i+1].priority){
                Sched_Task_t temp = Tasks[i];
                Tasks[i] = Tasks[i+1];
                Tasks[i+1] = temp;
                swapped = 1;
            }
        }
    }

}
