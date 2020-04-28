#ifndef CLIENT_TIME_H_INCLUDED
#define CLIENT_TIME_H_INCLUDED

#include "common_atomic.h"
/**
 * @brief Global variable to be set when server informs client of timeup
 * 
 */
extern atomic_lli_t *timeup_client;

#endif //CLIENT_TIME_H_INCLUDED
