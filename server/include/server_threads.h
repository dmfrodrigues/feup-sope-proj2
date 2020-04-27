#ifndef SERVER_THREADS_H_INCLUDED
#define SERVER_THREADS_H_INCLUDED

#include "message.h"

int server_create_thread(const message_t *m);

int server_wait_all_threads(void);

#endif //SERVER_THREADS_H_INCLUDED
