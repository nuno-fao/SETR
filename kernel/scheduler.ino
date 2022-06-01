
int Sched_AddT(void (*f)(void), int delay, int period, int priority);

typedef struct {
    /* period in ticks */
    int period;
    /* ticks until next activation */
    int delay;
    /* function pointer */
    void (*func)(void);
    /* activation counter */
    int exec;
    /* task priority */
    int priority;
} Sched_Task_t;

#define NT 20
Sched_Task_t Tasks[NT];
int cur_task = NT;


int Sched_Init(void){
    for(int x=0; x<NT; x++)
        Tasks[x].func = 0;
    /* Also configures interrupt that periodically calls Sched_Schedule(). */
    noInterrupts(); // disable all interrupts
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

//OCR1A = 6250; // compare match register 16MHz/256/10Hz
//OCR1A = 31250; // compare match register 16MHz/256/2Hz
    OCR1A = 31;    // compare match register 16MHz/256/2kHz
    TCCR1B |= (1 << WGM12); // CTC mode
    TCCR1B |= (1 << CS12); // 256 prescaler
    TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
    interrupts(); // enable all interrupts
}

int Sched_AddT(void (*f)(void), int delay, int period, int priority){ //TODO ordenar ao adicionar
    for(int x=0; x<NT; x++)
        if (!Tasks[x].func) {
            Tasks[x].period = period;
            Tasks[x].delay = delay;
            Tasks[x].exec = 0;
            Tasks[x].func = f;
            orderTasks();
            return x;
        }
    orderTasks();
    return -1;
}


void Sched_Schedule(void){
    for(int x=0; x<NT; x++) {
        if(Tasks[x].func){
            if(Tasks[x].delay){
                Tasks[x].delay--;
            } else {
                /* Schedule Task */
                Tasks[x].exec++;
                Tasks[x].delay = Tasks[x].period-1;
            }
        }
    }
}


void Sched_Dispatch(void){
    int prev_task = cur_task;
    for(int x=0; x<cur_task; x++) {
        if((Tasks[x].func)&&(Tasks[x].exec)) {
            Tasks[x].exec=0;
            cur_task = x;
            interrupts();
            Tasks[x].func();
            noInterrupts();
            cur_task = prev_task;
            /* Delete task if one-shot */ //TODO remove task from list if one-shot
            if(!Tasks[x].period) removeTask
        }
    }
}

void removeTask(int x){
    Tasks[x].func = 0;
    for (int i = x+1; i < NT; ++i) {
        Tasks[i-1] = Tasks[i]
    }
    Tasks[NT] = (Sched_Task_t){0};
}

void orderTasks(){
    int swapped = 1;
    while(swapped){
        swapped = 0;
        for (int i = 0; i < NT-1; ++i) {
            if(Tasks[i].priority > Tasks[i+1].priority){
                Sched_Task_t temp = Tasks[i];
                Tasks[i] = Tasks[i+1];
                Tasks[i+1] = temp;
                swapped = 1;
            }
        }
    }
}
