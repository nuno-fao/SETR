uint8_t _semaphore_counter = 0;

typedef struct {
      uint8_t tasks[MAX_TASKS];
      uint8_t ceiling;
} _semaphore;

#define CREATE_SEMAPHORE(name) \
    _semaphore name##_semaphore = { \
      .ceiling = 255,  \
    }; 
      
#define DECLARE(name,t) \ 

#define LOCK(name,t) \ 

#define UNLOCK(name) \ 
