uint8_t _semaphore_counter = 0;

typedef struct {
      Task *task;
} _semaphore;

#define CREATE_SEMAPHORE(name) \
    _semaphore name##_semaphore = { \
      .task = 0, \
    }; 
    
#define LOCK(name,t) \
    noInterrupts(); \
    while(true){ \
      if(name##_semaphore.task == 0){ \
        name##_semaphore.task = &t; \
        interrupts(); \
        break; \
      }else{ \
          name##_semaphore.task->priority = t.priority; \
          t.state = TASK_BLOCKED; \
          interrupts(); \
          vPortYieldFromTick(0); \
      } \
    } 

#define UNLOCK(name) \
    noInterrupts(); \
    name##_semaphore.task->priority = name##_semaphore.task->original_priority; \
    for (int i = 0; i < task_count; i++) { \
      if(tasks[i]->state == TASK_BLOCKED){ \
        tasks[i]->state = TASK_WAITING; \
      } \
    } \
    name##_semaphore.task = 0; \
    interrupts(); \
    vPortYieldFromTick(0); 
