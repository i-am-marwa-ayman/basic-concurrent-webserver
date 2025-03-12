#include <stdio.h>
#include <pthread.h>
#include "request.h"
#include "io_helper.h"


typedef struct {
    int size;
    int count;
    int *buffer;
    int empty;
    int full;

    pthread_mutex_t lock;
    pthread_cond_t is_full;
    pthread_cond_t is_empty;
} buffer_t ;

buffer_t *b;

void init(int size) {
    b->size = size;
    b->count = 0;
    b->empty = 1;
    b->full = 0;

    b->buffer = (int)malloc(size * sizeof(int));

    pthread_mutex_init(&b->lock,NULL);
    pthread_con_init(&b->full,NULL);
    pthread_con_init(&b->empty,NULL);
}

void *master(int port){
    int listen_fd = open_listen_fd_or_die(port);
	while (1){
		struct sockaddr_in client_addr;
		int client_len = sizeof(client_addr);
		int conn_fd = accept_or_die(listen_fd, (sockaddr_t *)&client_addr, (socklen_t *)&client_len);

        Pthread_mutex_lock(&b->lock);
        while (b->full == 1){
            Pthread_cond_wait(&b->empty, b->lock);
        }

        b->buffer[(b->count++) % b->size] = conn_fd;

        Pthread_cond_signal(&b->full);
        Pthread_mutex_unlock(b->lock);
	}
}
void *worker(){
    Pthread_mutex_lock(&b->lock);
    while (b->count == 0){
        Pthread_cond_wait(&b->full, &b->lock);
    }
    int conn_fd = b->buffer[b->count--];
    request_handle(conn_fd);
	close_or_die(conn_fd);
    Pthread_cond_signal(&b->empty);
    Pthread_mutex_unlock(&b->lock);
}