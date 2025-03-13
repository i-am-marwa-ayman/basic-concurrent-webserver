#include <stdio.h>
#include "request.h"
#include "io_helper.h"
#include "thread_helper.h"


buffer_t *b;

void init(int size) {
    b->size = size;
    b->front = -1;
    b->rear = -1;

    b->buffer = (int)malloc(size * sizeof(int));

    pthread_mutex_init(&b->lock,NULL);
    pthread_con_init(&b->fill,NULL);
    pthread_con_init(&b->empty,NULL);
}

void *master(int port){
    int listen_fd = open_listen_fd_or_die(port);
	while (1){
		struct sockaddr_in client_addr;
		int client_len = sizeof(client_addr);
		int conn_fd = accept_or_die(listen_fd, (sockaddr_t *)&client_addr, (socklen_t *)&client_len);

        Pthread_mutex_lock(&b->lock);
        while (b->rear == b->size - 1){
            Pthread_cond_wait(&b->empty, b->lock);
        }
        
        if(b->front == -1){
            b->front = 0;
        }

        b->buffer[++b->rear] = conn_fd;

        Pthread_cond_signal(&b->fill);
        Pthread_mutex_unlock(b->lock);
	}
}
void *worker(){
    Pthread_mutex_lock(&b->lock);
    while (b->front == -1 || b->front == b->rear + 1){
        Pthread_cond_wait(&b->fill, &b->lock);
    }
    int conn_fd = b->buffer[b->front++];
    request_handle(conn_fd);
	close_or_die(conn_fd);
    Pthread_cond_signal(&b->empty);
    Pthread_mutex_unlock(&b->lock);
}