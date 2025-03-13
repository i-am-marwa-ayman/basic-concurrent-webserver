#include <stdio.h>
#include "request.h"
#include "io_helper.h"
#include "thread_helper.h"

char default_root[] = ".";

//
// ./wserver [-d <basedir>] [-p <portnum>] 
// 
int main(int argc, char *argv[]) {
    int c;
    char *root_dir = default_root;
    int port = 10000;
    int thread_num = 1;
	int buffer_size = 1;
    while ((c = getopt(argc, argv, "d:p:t:b:")) != -1)
	switch (c) {
	case 'd':
	    root_dir = optarg;
	    break;
	case 'p':
	    port = atoi(optarg);
	    break;
	case 't':
	    thread_num = atoi(optarg);
	    break;
	case 'b':
	    buffer_size = atoi(optarg);
	    break;
	default:
	    fprintf(stderr, "usage: wserver [-d basedir] [-p port]\n");
	    exit(1);
	}

    // run out of this directory
    chdir_or_die(root_dir);

    // now, get to work

	pthread_t master_thread;
	pthread_t worker_thread[thread_num];
	init(buffer_size);

	if(pthread_create(&master_thread, NULL, &master, (void *)&port) != 0){
		perror("error");
	}
	for(int i = 0;i < thread_num;i++){
		if(pthread_create(&worker_thread[i], NULL, &worker, NULL) != 0){
			perror("error");
		}
	}
	if(pthread_join(master_thread, NULL) != 0){
		perror("error");
	}
	for(int i = 0;i < thread_num;i++){
		if(pthread_join(worker_thread[i], NULL) != 0){
			perror("error");
		}
	}
	destroy();
	return 0;
}


    


 
