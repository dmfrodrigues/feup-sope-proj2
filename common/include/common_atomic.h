#ifndef COMMON_ATOMIC_H_INCLUDED
#define COMMON_ATOMIC_H_INCLUDED

/**
 * @brief Atomic long long int.
 */
typedef struct atomic_lli atomic_lli_t;
/**
 * @brief atomic_lli constructor.
 * 
 * Initializes variable to 0.
 * 
 * @return atomic_lli_t* New atomic_lli
 */
atomic_lli_t* atomic_lli_ctor(void);
/**
 * @brief atomic_lli destructor.
 * 
 * Also destructs memory pointed by p; thus you do not need to free p, but you should set it to NULL.
 * 
 * @param p atomic_lli to be deleted
 */
void atomic_lli_dtor(atomic_lli_t *p);
/**
 * @brief Set value of atomic_lli.
 * 
 * @param p atomic_lli to be set
 * @param n new value
 */
void atomic_lli_set(atomic_lli_t *p, long long int n);
/**
 * @brief Get value of atomic_lli.
 * 
 * @param p atomic_lli to get value from.
 * @return long long int value of the atomic_lli
 */
long long int atomic_lli_get(atomic_lli_t *p);
/**
 * @brief Post-increment atomic_lli.
 * 
 * @param p atomic_lli to be post-incremented
 * @return long long int value of atomic_lli before incrementing
 */
long long int atomic_lli_postinc(atomic_lli_t *p);
/**
 * @brief Post-decrement atomic_lli.
 * 
 * @param p atomic_lli to be post-decremented.
 * @return long long int value
 */
long long int atomic_lli_postdec(atomic_lli_t *p);

#endif //COMMON_ATOMIC_H_INCLUDED
