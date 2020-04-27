#include "server_threads.h"

#include "common_time.h"
#include "output.h"

#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#define SLEEP_MICROSECONDS 100000

static int num_threads = 0;
static pthread_mutex_t num_threads_mutex = PTHREAD_MUTEX_INITIALIZER;

static int num_processed_requests = 0;
static pthread_mutex_t num_processed_requests_mutex = PTHREAD_MUTEX_INITIALIZER;

void* server_thread_func(void *arg){
    int *ret = malloc(sizeof(int));
    *ret = EXIT_SUCCESS;

    message_t *m = (message_t*)arg;

    // Use bathroom
    //     Make confirmation message
    message_t confirm = {
        .i = m->i,
        .pid = getpid(),
        .tid = pthread_self(),
        .dur = m->dur
    };
    pthread_mutex_lock(&num_processed_requests_mutex);
    confirm.pl = num_processed_requests++;
    pthread_mutex_unlock(&num_processed_requests_mutex);
    //     Confirm usage of bathroom
    if(output(&confirm, op_ENTER)) { *ret = EXIT_FAILURE; return ret; }
    //     Open, write and close private fifo
    char privfifo_path[PATH_MAX];
    sprintf(privfifo_path, "/tmp/%d.%lu", m->pid, m->tid);
    int privfifo_fd = open(privfifo_path, O_WRONLY);
    write(privfifo_fd, &confirm, sizeof(message_t));
    close(privfifo_fd);
    //     Actually use bathroom
    double start; if(common_gettime(&start)) { *ret = EXIT_FAILURE; return ret; }
    double end; if(common_gettime(&end)) { *ret = EXIT_FAILURE; return ret; }
    while(end - start < m->dur){
        if(common_gettime(&end)) { *ret = EXIT_FAILURE; return ret; }
    }
    //     Finished using the bathroom
    if(output(&confirm, op_TIMUP)) { *ret = EXIT_FAILURE; return ret; }
    //Routine stuff
    pthread_mutex_lock(&num_threads_mutex);
    --num_threads;
    pthread_mutex_unlock(&num_threads_mutex);
    free(arg);
    return ret;
}

int server_create_thread(const message_t *m){
    pthread_mutex_lock(&num_threads_mutex);
    ++num_threads;
    pthread_mutex_unlock(&num_threads_mutex);

    message_t *m_ = malloc(sizeof(message_t));
    *m_ = *m;

    pthread_t tid_dummy;
    pthread_create(&tid_dummy, NULL, server_thread_func, m_);

    return EXIT_SUCCESS;
}

int server_wait_all_threads(void){
    while(true){
        pthread_mutex_lock(&num_threads_mutex);
        if(num_threads <= 0){ pthread_mutex_unlock(&num_threads_mutex); return EXIT_SUCCESS;}
        pthread_mutex_unlock(&num_threads_mutex);
        if(usleep(SLEEP_MICROSECONDS)) return EXIT_FAILURE;
    }
}
