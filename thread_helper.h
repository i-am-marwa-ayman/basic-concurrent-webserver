#include <pthread.h>
#ifndef __THREAD_HELPER__ 
#define THREAD_HELPER_

typedef struct {
    int size;
    int *buffer;
    int front;
    int rear;
    int cur_size;

    pthread_mutex_t lock;
    pthread_cond_t fill;
    pthread_cond_t empty;
} buffer_t ;

// threading functions
void init(int size);
void *master(void *arg);
void *worker();
void destroy();

// queue functions
int is_full();
int is_empty();
int fix_index(int index);
void add(int new_element);
int pop();
#endif 
