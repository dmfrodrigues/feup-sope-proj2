#ifndef SERVER_SIGNAL_H_INCLUDED
#define SERVER_SIGNAL_H_INCLUDED

#include <signal.h>
#include <stdbool.h>

extern volatile sig_atomic_t timeup;

/**
 * @brief Install handlers.
 * 
 * @return int EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int server_install_handlers(void);

#endif //SERVER_SIGNAL_H_INCLUDED
