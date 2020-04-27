#ifndef COMMON_TIME_H_INCLUDED
#define COMMON_TIME_H_INCLUDED

typedef long long micro_t;

/**
 * @brief Start time counting.
 * 
 * @return int EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int common_starttime(micro_t *micros_since_epoch);

/**
 * @brief Set starting point in time.
 * 
 * @param micros_since_epoch    Number of microseconds since the epoch
 * @return int                  EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int common_set_starttime(micro_t micros_since_epoch);

/**
 * @brief Get number of milliseconds since time was started.
 * 
 * @param d     Pointer to double where time will be stored
 * @return int  EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int common_gettime(double *d);

int common_wait(double d);

#endif //COMMON_TIME_H_INCLUDED
