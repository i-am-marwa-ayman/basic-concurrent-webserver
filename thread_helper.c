#include <stdio.h>
#include "request.h"
#include "io_helper.h"
#include "thread_helper.h"


buffer_t *b;

void init(int size) {
    b = (buffer_t *)malloc(sizeof(buffer_t));
    if (!b) {
        perror("Failed to allocate buffer");
        exit(1);
    }

    b->size = size;
    b->front = -1;
    b->rear = -1;
    b->buffer = (int*)malloc(size * sizeof(int));

    pthread_mutex_init(&b->lock,NULL);
    pthread_cond_init(&b->fill,NULL);
    pthread_cond_init(&b->empty,NULL);

    return;
}

void *master(void *arg){
    int port = *(int*) arg;
    int listen_fd = open_listen_fd_or_die(port);
	while (1){
		struct sockaddr_in client_addr;
		int client_len = sizeof(client_addr);
		int conn_fd = accept_or_die(listen_fd, (sockaddr_t *)&client_addr, (socklen_t *)&client_len);

        pthread_mutex_lock(&b->lock);
        while (b->rear == b->size - 1){
            pthread_cond_wait(&b->empty, &b->lock);
        }
        
        if(b->front == -1){
            b->front = 0;
        }

        b->buffer[++b->rear] = conn_fd;

        pthread_cond_signal(&b->fill);
        pthread_mutex_unlock(&b->lock);
	}
    return NULL;
}
void *worker(){
    pthread_mutex_lock(&b->lock);
    while (b->front == -1 || b->front == b->rear + 1){
        pthread_cond_wait(&b->fill, &b->lock);
    }
    int conn_fd = b->buffer[b->front++];
    request_handle(conn_fd);
	close_or_die(conn_fd);
    pthread_cond_signal(&b->empty);
    pthread_mutex_unlock(&b->lock);
    return NULL;
}

void destroy(){
    if(b){
        pthread_mutex_destroy(&b->lock);
        pthread_cond_destroy(&b->empty);
        pthread_cond_destroy(&b->fill);
        if(!b->buffer){
            free(b->buffer);
        }
        free(b);
    }
    return;
}