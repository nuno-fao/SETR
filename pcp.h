uint8_t _semaphore_counter = 0;

typedef struct {
      uint8_t ceiling;
      void  *task;
} _semaphore;

_semaphore *semaphore_list[MAX_SEMAPHORES];

void addSem(_semaphore *s){
  semaphore_list[_semaphore_counter] = s;
  _semaphore_counter++;
}

#define CREATE_SEMAPHORE(name) \
    _semaphore name##_semaphore = { \
      .ceiling = 255,  \
      .task = 0,\
    }; \

#define ADD_SEMAPHORE(name) \
    addSem(&name##_semaphore);\
      
void declareSem(_semaphore *s,int c){
    if(c < s->ceiling){
      s->ceiling = c;
    }
}     

#define DECLARE(name,t) \
  interrupts();\
  declareSem(&name##_semaphore,t.priority); \
  noInterrupts();\



#define LOCK(name,t) \
  noInterrupts(); \
  while(true){\
    noInterrupts(); \
    bool blocked = false;\
    int current_ceiling = name##_semaphore.ceiling; \
    if(name##_semaphore.task == 0){\
      for (size_t i = 0; i < _semaphore_counter; i++) \
      { \
        if(semaphore_list[i]->task != 0 && t.priority >= semaphore_list[i]->ceiling){\
            ((Task*)semaphore_list[i]->task)->priority = t.priority; \
            blocked = true; \
        }\
      } \
    }\
    else{\
      blocked = true;\
      ((Task*)name##_semaphore.task)->priority = t.priority; \
    }\
    if(!blocked){\
      name##_semaphore.task = &t; \
      interrupts(); \
      break; \
    }\
    else{\
      t.state = TASK_BLOCKED; \
      interrupts(); \
      vPortYieldFromTick(0); \
    }\
  }

#define UNLOCK(name,t) \
    noInterrupts(); \
    ((Task*)name##_semaphore.task)->priority = ((Task*)name##_semaphore.task)->original_priority; \
    for (int i = 0; i < task_count; i++) { \
      if(tasks[i]->state == TASK_BLOCKED){ \
        tasks[i]->state = TASK_WAITING; \
      } \
    } \
    name##_semaphore.task = 0; \
    interrupts(); \
    vPortYieldFromTick(0); 
  