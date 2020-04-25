#ifndef CLIENT_TBDNAME_H_INCLUDED
#define CLIENT_TBDNAME_H_INCLUDED

#include <sys/types.h>
#include <sys/stat.h>

#define NANOSECOND_MULTIPLIER  1000000 // from ms to ns

/**
 * @brief Generates a random number of ms
 * 
 * @return long 
 */
long client_generate_random_ms(int lower, int upper); // use default values: lower = 0, upper = 10


int client_open_public_fifo(const char *pathname, mode_t mode, int *fd); // use default values: mode = 777

int client_create_open_private_fifo(const char *pathname, mode_t mode, int *fd); // use default values: mode = 777


int client_create_thread();

void *client_execute_thread(void *arg);

#endif //CLIENT_TBDNAME_H_INCLUDED