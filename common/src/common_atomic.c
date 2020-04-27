#include "common_atomic.h"

#include <stdlib.h>
#include <pthread.h>

struct atomic_lli {
    long long int n;
    pthread_mutex_t mutex;
};
atomic_lli_t* atomic_lli_ctor(void){
    atomic_lli_t *res = malloc(sizeof(atomic_lli_t));
    res->n = 0;
    pthread_mutex_init(&res->mutex, NULL);
    return res;
}
void atomic_lli_dtor(atomic_lli_t *p){
    pthread_mutex_destroy(&p->mutex);
    free(p);
}
void atomic_lli_set(atomic_lli_t *p, long long int n){
    pthread_mutex_lock(&p->mutex);
    p->n = n;
    pthread_mutex_lock(&p->mutex);
}
long long int atomic_lli_get(atomic_lli_t *p){
    long long int n;
    pthread_mutex_lock(&p->mutex);
    n = p->n;
    pthread_mutex_lock(&p->mutex);
    return n;
}
