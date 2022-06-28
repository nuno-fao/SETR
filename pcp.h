uint8_t _semaphore_counter = 0;

typedef struct {
      uint8_t priorities[MAX_TASKS];
      uint8_t priorities_counter;
      uint8_t ceiling;
      bool  locked;
      void  *task;
} _semaphore;

#define CREATE_SEMAPHORE(name) \
    _semaphore name##_semaphore = { \
      .priorities = {0}, \
      .priorities_counter = 0, \
      .ceiling = 255,  \
      .locked = false, \
      .task = 0,\
    }; 
      
#define DECLARE(name,t) \
  noInterrupts(); \
  if(t.priority < name##_semaphore.ceiling){ \
    name##_semaphore.ceiling = t.priority; \
  } \
  name##_semaphore.priorities[name##_semaphore.priorities_counter] = t.priority; \
  name##_semaphore.priorities_counter++; \
  t.semaphores[t.semaphores_counter] = &name##_semaphore;\
  t.semaphores_counter++; \
  interrupts();

#define LOCK(name,t) \
  noInterrupts(); \
  while(true){ \
    noInterrupts(); \
    bool is_block = false;\
    for (size_t i = 0; i < t.semaphores_counter; i++) \
    { \
      if(t.semaphores[i]->locked || t.semaphores[i]->ceiling < t.priority){\
        is_block = true;\
        break;\
      }\
    } \
    if(is_block){\
        ((Task*)name##_semaphore.task)->priority = t.priority; \
        t.state = TASK_BLOCKED; \
        interrupts(); \
        vPortYieldFromTick(0); \
    }\
    else{\
      name##_semaphore.task = &t; \
      name##_semaphore.locked = true; \
      interrupts(); \
      break; \
    }\
  }\

#define UNLOCK(name,t) \
  noInterrupts();\
  ((Task*)name##_semaphore.task)->priority = ((Task*)name##_semaphore.task)->original_priority; \
  t.semaphores_counter = 0;\
  for (int i = 0; i < task_count; i++) { \
    if(tasks[i]->state == TASK_BLOCKED){ \
      tasks[i]->state = TASK_WAITING; \
    } \
  } \
  bool colapsed = false; \
  for (uint8_t i = 0; i < name##_semaphore.priorities_counter - 1; i++) \
  { \
      if(name##_semaphore.priorities[i] == t.priority && !colapsed){\
        name##_semaphore.priorities[i] = -1;\
        colapsed = true;\
      }\
      if(name##_semaphore.priorities[i] == -1){\
        name##_semaphore.priorities[i] = name##_semaphore.priorities[i+1]; \
        name##_semaphore.priorities[i+1] = -1; \
      }\
  } \
  name##_semaphore.priorities_counter--; \
  name##_semaphore.ceiling = 255;\
  for (uint8_t i = 0; i < name##_semaphore.priorities_counter; i++) \
  { \
      if(name##_semaphore.priorities[i] < name##_semaphore.ceiling){\
        name##_semaphore.ceiling = name##_semaphore.priorities[i]; \
      }\
  } \
  name##_semaphore.locked = false; \
  name##_semaphore.task = 0; \
  interrupts();\
  vPortYieldFromTick(0);
  