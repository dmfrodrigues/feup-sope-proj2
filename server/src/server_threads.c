#include "server_threads.h"

#include "common_time.h"
#include "common_atomic.h"
#include "output.h"
#include "server_signal.h"

#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <semaphore.h>

#define SLEEP_MICROSECONDS 100000

#define NOT_SHARED  0

atomic_lli_t* num_processed_requests = NULL;

int places;
int max_threads;
bool *spots = NULL;
bool atLeastOneSpotOpen = true;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 


int server_threads_init(int nplaces, int nthreads){
    num_processed_requests = atomic_lli_ctor();
    spots = calloc(nplaces, sizeof(bool));
    places = nplaces;
    max_threads = nthreads;
    if(sem_init(&s, NOT_SHARED, max_threads) != EXIT_SUCCESS) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int server_threads_clean(void){
    atomic_lli_dtor(num_processed_requests); num_processed_requests = NULL;
    free(spots);
    return EXIT_SUCCESS;
}

int server_thread_answer(const message_t *m, const message_t *confirm){
    int ret = EXIT_SUCCESS;
    char privfifo_path[PATH_MAX];
    if(sprintf(privfifo_path, "/tmp/%d.%lu", m->pid, m->tid) < 0) return EXIT_FAILURE;
    int privfifo_fd = open(privfifo_path, O_WRONLY);
    if(privfifo_fd < 0) { output(m, op_GAVUP); return EXIT_FAILURE; }
    if(write(privfifo_fd, confirm, sizeof(message_t)) != sizeof(message_t)) ret = EXIT_FAILURE;
    if(close(privfifo_fd)) ret = EXIT_FAILURE;
    return ret;
}

void* server_thread_func(void *arg){
    int *ret = malloc(sizeof(int));
    *ret = EXIT_SUCCESS;

    message_t *m = (message_t*)arg;

    /* Use bathroom */{
        /* Make confirmation message */
        message_t confirm; {
            confirm.i = m->i;
            confirm.pid = getpid();
            confirm.tid = pthread_self();
            confirm.dur = m->dur;
            confirm.pl = m->pl;
        }
        if(output(&confirm, op_ENTER))          { *ret = EXIT_FAILURE; return ret; }        // Confirm usage of bathroom
        if(server_thread_answer(m, &confirm))   { *ret = EXIT_FAILURE; return ret; }        // Open, write and close private fifo
        if(common_wait(m->dur))                 { *ret = EXIT_FAILURE; return ret; }        // Actually use bathroom
        if(output(&confirm, op_TIMUP))          { *ret = EXIT_FAILURE; return ret; }        // Finished using the bathroom
        
        pthread_mutex_lock(&mutex);
        spots[m->pl] = false;
        atLeastOneSpotOpen = true;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);

        sem_post(&s);
    }
    //Routine stuff
    free(arg);
    return ret;
}

int try_entering(message_t *m_){
    pthread_mutex_lock(&mutex);
    while (!atLeastOneSpotOpen)
    {
        pthread_cond_wait(&cond, &mutex); 
    }

    if (timeup_server) {
        output(m_, op_2LATE);
        message_t confirm; {
            confirm.i = m_->i;
            confirm.pid = getpid();
            confirm.tid = pthread_self();
            confirm.dur = -1;
            confirm.pl = -1;
        }
        server_thread_answer(m_, &confirm);

        atLeastOneSpotOpen = true;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);

        sem_post(&s);

        return EXIT_SUCCESS;
    }

    for (int i = 0 ; i < places ; i++){
        if (!spots[i]){
            m_->pl = i;
            spots[i] = true;
            pthread_mutex_unlock(&mutex);
            pthread_t tid_dummy;
            pthread_create(&tid_dummy, NULL, server_thread_func, m_);
            return EXIT_SUCCESS;
        }
    }

    atLeastOneSpotOpen = false;
    pthread_mutex_unlock(&mutex);
    try_entering(m_);
    return EXIT_FAILURE;
}

int server_create_thread(const message_t *m){
    message_t *m_ = malloc(sizeof(message_t));
    *m_ = *m;

    try_entering(m_);
    
    return EXIT_SUCCESS;
}

int server_wait_all_threads(void){
    int x; 
    if (sem_getvalue(&s, &x)) return EXIT_FAILURE;
    while(x < max_threads){
        if(usleep(SLEEP_MICROSECONDS)) return EXIT_FAILURE;
        if (sem_getvalue(&s, &x)) return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int server_close_service(char* fifoname){
    int ret = EXIT_SUCCESS;
    
    // Open public fifo to clear "buffer"
    int fifo_des = open(fifoname, O_RDONLY | O_NONBLOCK);
    if(fifo_des == -1 && errno != EINTR) ret = EXIT_FAILURE;
     
    milli_t start_time, now_time; common_gettime(&start_time); common_gettime(&now_time);
    // Read one message (??)
    message_t m;
    while(now_time-start_time <= 100.0){
        int r = read(fifo_des, &m, sizeof(message_t));
        if(r == -1 && errno != EAGAIN){ 
            break;
        }
        if(r == sizeof(message_t)){
            if(output(&m, op_RECVD)) ret = EXIT_FAILURE;
            if(output(&m, op_2LATE)) ret = EXIT_FAILURE;
            message_t confirm; {
                confirm.i = m.i;
                confirm.pid = getpid();
                confirm.tid = pthread_self();
                confirm.dur = -1;
                confirm.pl = -1;
            }
            if (server_thread_answer(&m, &confirm)) ret = EXIT_FAILURE;
            break;
        }
        common_gettime(&now_time);
    }

    close(fifo_des);
    if(unlink(fifoname)) ret = EXIT_FAILURE;

    if(server_wait_all_threads()) ret = EXIT_FAILURE;

    return ret;
}
