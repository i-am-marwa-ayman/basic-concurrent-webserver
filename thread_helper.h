#include <pthread.h>
#ifndef __THREAD_HELPER__ 
#define THREAD_HELPER_

typedef struct {
    int size;
    int *buffer;
    int front;
    int rear;

    pthread_mutex_t lock;
    pthread_cond_t fill;
    pthread_cond_t empty;
} buffer_t ;
void init(int size);
void *master(int port);
void *worker();
#endif 
