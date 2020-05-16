#ifndef SERVER_SIGNAL_H_INCLUDED
#define SERVER_SIGNAL_H_INCLUDED

#include <signal.h>
#include <stdbool.h>

volatile sig_atomic_t timeup_server;

/**
 * @brief Install handlers.
 * 
 * @return int EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int server_install_handlers(void) __attribute__((warn_unused_result));

#endif //SERVER_SIGNAL_H_INCLUDED
