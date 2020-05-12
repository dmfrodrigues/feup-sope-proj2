#include "common_atomic.h"

#include <stdlib.h>
#include <pthread.h>

struct atomic_lli {
    long long int n;
    pthread_mutex_t mutex;
};
atomic_lli_t* atomic_lli_ctor(void){
    atomic_lli_t *res = malloc(sizeof(atomic_lli_t));
    if(res == NULL) return NULL;
    res->n = 0;
    if(pthread_mutex_init(&res->mutex, NULL)){ free(res); res = NULL; }
    return res;
}
int atomic_lli_dtor(atomic_lli_t *p){
    if(p == NULL) return EXIT_SUCCESS;
    if(pthread_mutex_destroy(&p->mutex)) return EXIT_FAILURE;
    free(p);
    return EXIT_SUCCESS;
}
void atomic_lli_set(atomic_lli_t *p, long long int n){
    pthread_mutex_lock(&p->mutex);
    p->n = n;
    pthread_mutex_unlock(&p->mutex);
}
long long int atomic_lli_get(atomic_lli_t *p){
    long long int n;
    pthread_mutex_lock(&p->mutex);
    n = p->n;
    pthread_mutex_unlock(&p->mutex);
    return n;
}
long long int atomic_lli_postinc(atomic_lli_t *p){
    long long int n;
    pthread_mutex_lock(&p->mutex);
    n = p->n++;
    pthread_mutex_unlock(&p->mutex);
    return n;
}
long long int atomic_lli_postdec(atomic_lli_t *p){
    long long int n;
    pthread_mutex_lock(&p->mutex);
    n = p->n--;
    pthread_mutex_unlock(&p->mutex);
    return n;
}
