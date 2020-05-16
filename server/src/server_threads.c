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

#define SLEEP_MICROSECONDS      100000      // Time to wait before asking again if all threads returned
#define EMPTY_FIFO_TIME_MILLIS  100         // Time the server will receive 2LATE requests after the time it was supposed to stop working
#define SEMAPHORE_SHARED        0           // Semaphore is not shared among several processes

int number_places;
int max_threads;
bool *spots = NULL;

int server_threads_init(int nplaces, int nthreads){
    spots = calloc(nplaces, sizeof(bool));
    number_places = nplaces;
    max_threads = nthreads;
    if(sem_init(&thread_semaphore, SEMAPHORE_SHARED, max_threads) != EXIT_SUCCESS) return EXIT_FAILURE;
    if(sem_init(&place_semaphore, SEMAPHORE_SHARED, number_places) != EXIT_SUCCESS) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int server_threads_clean(void){
    free(spots);
    return EXIT_SUCCESS;
}

/**
 * @brief Writes response to a given request.
 * 
 * @param request   Request that originated the response
 * @param response  Response to a request
 * @return int      EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int server_thread_answer(const message_t *request, const message_t *response){
    int ret = EXIT_SUCCESS;
    char private_fifo_path[PATH_MAX];
    if(sprintf(private_fifo_path, "/tmp/%d.%lu", request->pid, request->tid) < 0){
        if(output(response, op_GAVUP)) return EXIT_FAILURE;
        return EXIT_FAILURE;
    }
    int private_fifo_filedes = open(private_fifo_path, O_WRONLY);
    if(private_fifo_filedes < 0) { output(request, op_GAVUP); return EXIT_FAILURE; }
    if(write(private_fifo_filedes, response, sizeof(message_t)) != sizeof(message_t)) ret = EXIT_FAILURE;
    if(close(private_fifo_filedes)) ret = EXIT_FAILURE;
    return ret;
}

void* server_thread_func(void *arg){
    int *ret = malloc(sizeof(int));
    *ret = EXIT_SUCCESS;

    message_t *request = (message_t*)arg;

    /* Make confirmation message */
    message_t response = {
        .i = request->i,
        .pid = getpid(),
        .tid = pthread_self(),
        .dur = request->dur,
        .pl = request->pl
    };
    if(output(&response, op_ENTER))             { *ret = EXIT_FAILURE; return ret; }    // Confirm usage of bathroom
    if(server_thread_answer(request, &response)){ *ret = EXIT_FAILURE; return ret; }    // Open, write and close private fifo
    if(common_wait(request->dur))               { *ret = EXIT_FAILURE; return ret; }    // Actually use bathroom
    if(output(&response, op_TIMUP))             { *ret = EXIT_FAILURE; return ret; }    // Finished using the bathroom
    
    spots[request->pl] = false;

    if(sem_post(&thread_semaphore)){ fprintf(stderr, "Could not post\n"); *ret = EXIT_FAILURE; }
    if(sem_post(&place_semaphore )){ fprintf(stderr, "Could not post\n"); *ret = EXIT_FAILURE; }
    
    //Routine stuff
    free(arg);
    return ret;
}

int server_create_thread(const message_t *m){
    message_t *m_ = malloc(sizeof(message_t));
    *m_ = *m;

    while(sem_wait(&thread_semaphore)) if(errno != EINTR) { fprintf(stderr, "Could not wait\n"); return EXIT_FAILURE; }
    while(sem_wait(&place_semaphore )) if(errno != EINTR) { fprintf(stderr, "Could not wait\n"); return EXIT_FAILURE; }

    if (timeup_server) {
        output(m_, op_2LATE);
        message_t response = {
            .i = m_->i,
            .pid = getpid(),
            .tid = pthread_self(),
            .dur = -1,
            .pl = -1
        };
        server_thread_answer(m_, &response);

        if(sem_post(&thread_semaphore)){ fprintf(stderr, "Could not post\n"); return EXIT_FAILURE; }
        if(sem_post(&place_semaphore )){ fprintf(stderr, "Could not post\n"); return EXIT_FAILURE; }

        return EXIT_SUCCESS;
    }

    for (int i = 0 ; i < number_places ; i++){
        if (!spots[i]){
            m_->pl = i;
            spots[i] = true;
            pthread_t tid_dummy;
            pthread_create(&tid_dummy, NULL, server_thread_func, m_);
            return EXIT_SUCCESS;
        }
    }

    return EXIT_FAILURE;
}

int server_close_service(const char *public_fifo_path){
    int ret = EXIT_SUCCESS;
    
    // Open public fifo
    int public_fifo_filedes = open(public_fifo_path, O_RDONLY | O_NONBLOCK);
    if(public_fifo_filedes == -1 && errno != EINTR) ret = EXIT_FAILURE;
    
    milli_t start_time, now_time; common_gettime(&start_time); common_gettime(&now_time);

    message_t request;
    while(now_time-start_time <= EMPTY_FIFO_TIME_MILLIS){
        int r = read(public_fifo_filedes, &request, sizeof(message_t));
        if(r == -1 && errno != EAGAIN){ 
            break;
        }
        if(r == sizeof(message_t)){
            if(output(&request, op_RECVD)) ret = EXIT_FAILURE;
            if(output(&request, op_2LATE)) ret = EXIT_FAILURE;
            message_t response = {
                .i = request.i,
                .pid = getpid(),
                .tid = pthread_self(),
                .dur = -1,
                .pl = -1
            };
            if (server_thread_answer(&request, &response)) ret = EXIT_FAILURE;
            break;
        }
        common_gettime(&now_time);
    }

    if(close(public_fifo_filedes))  ret = EXIT_FAILURE;
    if(unlink(public_fifo_path))    ret = EXIT_FAILURE;
    if(server_wait_all_threads())   ret = EXIT_FAILURE;

    return ret;
}

int server_wait_all_threads(void){
    int x; 
    if (sem_getvalue(&thread_semaphore, &x)) return EXIT_FAILURE;
    while(x < max_threads){
        if(usleep(SLEEP_MICROSECONDS)) return EXIT_FAILURE;
        if (sem_getvalue(&thread_semaphore, &x)) return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}