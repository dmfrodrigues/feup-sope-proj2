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
int atomic_lli_dtor(atomic_lli_t *p) __attribute__((warn_unused_result));
/**
 * @brief Set value of atomic_lli.
 * 
 * @param p atomic_lli to be set
 * @param n new value
 */
int atomic_lli_set(atomic_lli_t *p, long long int n) __attribute__((warn_unused_result));
/**
 * @brief Get value of atomic_lli.
 * 
 * @param p atomic_lli to get value from.
 * @return long long int value of the atomic_lli
 */
int atomic_lli_get(atomic_lli_t *p, long long int *n) __attribute__((warn_unused_result));
/**
 * @brief Increment atomic_lli.
 * 
 * @param p atomic_lli to be incremented
 * @return long long int value of atomic_lli before incrementing
 */
int atomic_lli_inc(atomic_lli_t *p) __attribute__((warn_unused_result));
/**
 * @brief Decrement atomic_lli.
 * 
 * @param p atomic_lli to be decremented.
 * @return long long int value
 */
int atomic_lli_dec(atomic_lli_t *p) __attribute__((warn_unused_result));

#endif //COMMON_ATOMIC_H_INCLUDED
