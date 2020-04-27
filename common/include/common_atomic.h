#ifndef COMMON_ATOMIC_H_INCLUDED
#define COMMON_ATOMIC_H_INCLUDED

typedef struct atomic_lli atomic_lli_t;
atomic_lli_t* atomic_lli_ctor(void);
void atomic_lli_dtor(atomic_lli_t *p);
void atomic_lli_set(atomic_lli_t *p, long long int n);
long long int atomic_lli_get(atomic_lli_t *p);
long long int atomic_lli_postinc(atomic_lli_t *p);
long long int atomic_lli_postdec(atomic_lli_t *p);

#endif //COMMON_ATOMIC_H_INCLUDED
