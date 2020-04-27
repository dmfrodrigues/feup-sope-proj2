#include "server_threads.h"

#include "common_time.h"
#include "common_atomic.h"
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

int server_threads_init(void){

    return EXIT_SUCCESS;
}

int server_threads_clean(void){

    return EXIT_SUCCESS;
}

int server_thread_answer(const message_t *m, const message_t *confirm){
    int ret = EXIT_SUCCESS;
    char privfifo_path[PATH_MAX];
    if(sprintf(privfifo_path, "/tmp/%d.%lu", m->pid, m->tid) < 0) return EXIT_FAILURE;
    int privfifo_fd = open(privfifo_path, O_WRONLY);
    if(privfifo_fd < 0) return EXIT_FAILURE;
    if(write(privfifo_fd, confirm, sizeof(message_t)) != sizeof(message_t)) ret = EXIT_FAILURE;
    if(close(privfifo_fd)) ret = EXIT_FAILURE;
    return ret;
}

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
    if(server_thread_answer(m, &confirm)) { *ret = EXIT_FAILURE; return ret; }
    //     Actually use bathroom
    if(common_wait(m->dur));
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
