#ifndef CLIENT_TBDNAME_H_INCLUDED
#define CLIENT_TBDNAME_H_INCLUDED

#include <sys/stat.h> // mode_t

/**
 * @brief Generates a random number of ms
 * 
 * @return int 
 */
int client_generate_random_ms(int lower = 0, int upper = 10);


int client_open_public_fifo(const char *pathname, mode_t mode = 777, int *fd);

int client_create_open_private_fifo(const char *pathname, mode_t mode = 777, int *fd);

#endif //CLIENT_TBDNAME_H_INCLUDED