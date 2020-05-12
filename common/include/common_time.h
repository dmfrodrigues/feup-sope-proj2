#ifndef COMMON_TIME_H_INCLUDED
#define COMMON_TIME_H_INCLUDED

typedef long long milli_t;
typedef long long micro_t;

/**
 * @brief Get number of milliseconds since epoch.
 * 
 * @param d     Pointer to milli_t where time will be stored
 * @return int  EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int common_gettime(milli_t *t);

/**
 * @brief Wait for millisecond intervals.
 * 
 * Waits in real milliseconds, not in processor milliseconds.
 * 
 * @param d     Number of milliseconds to wait
 * @return int  EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int common_wait(milli_t t);

#endif //COMMON_TIME_H_INCLUDED
