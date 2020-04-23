#include "server_args.h"
#include "server_signal.h"
#include "message.h"
#include "output.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

server_args_t args;

int init(int argc, char* argv[]){
    if(server_args_ctor(&args, argc, argv)) return EXIT_FAILURE;
    if(server_install_handlers()) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

void cleanup(void){
    if(server_args_dtor(&args)){
        const char *buf = "Could not destruct args\n";
        write(STDERR_FILENO, buf, strlen(buf));
    }
}

int main(int argc, char *argv[]){
    int ret = EXIT_SUCCESS;

    if(atexit(cleanup)) return EXIT_FAILURE;
    if(init(argc, argv)) return EXIT_FAILURE;

    if(alarm(args.nsecs)) return EXIT_FAILURE;
    
    mkfifo(args.fifoname, 0660);
    int fifo_des = open(args.fifoname, O_RDONLY);
    if(fifo_des == -1){
        if(unlink(args.fifoname)) ret = EXIT_FAILURE;
        if(errno == EINTR) return ret;
        else               return EXIT_FAILURE;
    }

    message_t m;
    while(read(fifo_des, &m, sizeof(message_t)) == sizeof(message_t)){
        if(output(&m, op_RECVD)){ ret = EXIT_FAILURE; break; }
        
    }
    if(errno != EINTR) ret = EXIT_FAILURE;

    if(close(fifo_des)) ret = EXIT_FAILURE;
    if(unlink(args.fifoname)) ret = EXIT_FAILURE;

    return ret;
}