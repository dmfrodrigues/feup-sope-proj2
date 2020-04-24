#include "acutest.h"
#include "server_args.h"

#include <limits.h>
#include <sys/time.h>

#define MAX_THREADS 1000000

char **deconstify_argv(int argc, const char **argv_const) {
    char **argv = calloc(argc, sizeof(char *));
    for (int i = 0; i < argc; ++i) {
        argv[i] = calloc(strlen(argv_const[i]) + 1, sizeof(char));
        strcpy(argv[i], argv_const[i]);
    }
    return argv;
}
void free_argv(int argc, char **argv) {
    for (int i = 0; i < argc; ++i) {
        free(argv[i]);
        argv[i] = NULL;
    }
    free(argv);
}

int get_seconds_since_epoch(double *t){
    struct timeval tv;
    if(gettimeofday(&tv, NULL)) return EXIT_FAILURE;
    *t = tv.tv_sec + ((double)tv.tv_usec)/1000000.0;
    return EXIT_SUCCESS;
}

void test_server_args_ctor(void) {
    {
        int argc = 1;
        const char *argv_const[2] = {"prog", NULL};
        char **argv = deconstify_argv(argc, argv_const);

        server_args_t args;
        TEST_CHECK(server_args_ctor(&args, argc, argv, MAX_THREADS) == EXIT_FAILURE);
        TEST_CHECK(server_args_dtor(&args) == EXIT_SUCCESS);

        free_argv(argc, argv);
    }
    {
        int argc = 2;
        const char *argv_const[3] = {"prog", "-t=2", NULL};
        char **argv = deconstify_argv(argc, argv_const);

        server_args_t args;
        TEST_CHECK(server_args_ctor(&args, argc, argv, MAX_THREADS) == EXIT_FAILURE);
        TEST_CHECK(server_args_dtor(&args) == EXIT_SUCCESS);

        free_argv(argc, argv);
    }
    {
        int argc = 2;
        const char *argv_const[3] = {"prog", "fifopath", NULL};
        char **argv = deconstify_argv(argc, argv_const);

        server_args_t args;
        TEST_CHECK(server_args_ctor(&args, argc, argv, MAX_THREADS) == EXIT_FAILURE);
        TEST_CHECK(server_args_dtor(&args) == EXIT_SUCCESS);

        free_argv(argc, argv);
    }
    {
        int argc = 3;
        const char *argv_const[4] = {"prog", "-t", "2", NULL};
        char **argv = deconstify_argv(argc, argv_const);

        server_args_t args;
        TEST_CHECK(server_args_ctor(&args, argc, argv, MAX_THREADS) == EXIT_FAILURE);
        TEST_CHECK(server_args_dtor(&args) == EXIT_SUCCESS);

        free_argv(argc, argv);
    }
    {
        int argc = 3;
        const char *argv_const[4] = {"prog", "-t=2", "fifopath", NULL};
        char **argv = deconstify_argv(argc, argv_const);

        server_args_t args;
        TEST_CHECK(server_args_ctor(&args, argc, argv, MAX_THREADS) == EXIT_FAILURE);
        TEST_CHECK(server_args_dtor(&args) == EXIT_SUCCESS);

        free_argv(argc, argv);
    }
    {
        int argc = 5;
        const char *argv_const[6] = {"prog", "-t2",      "-l3",
                                     "-n4",  "fifopath", NULL};
        char **argv = deconstify_argv(argc, argv_const);

        server_args_t args;
        TEST_CHECK(server_args_ctor(&args, argc, argv, MAX_THREADS) == EXIT_SUCCESS);
        TEST_CHECK(args.nsecs == 2);
        TEST_CHECK(args.nplaces == 3);
        TEST_CHECK(args.nthreads == 4);
        TEST_CHECK(strcmp(args.fifoname, "fifopath") == 0);
        TEST_CHECK(server_args_dtor(&args) == EXIT_SUCCESS);

        free_argv(argc, argv);
    }
    {
        int argc = 8;
        const char *argv_const[9] = {"prog", "-t", "2",        "-l", "3",
                                     "-n",   "4",  "fifopath", NULL};
        char **argv = deconstify_argv(argc, argv_const);

        server_args_t args;
        TEST_CHECK(server_args_ctor(&args, argc, argv, MAX_THREADS) == EXIT_SUCCESS);
        TEST_CHECK(args.nsecs == 2);
        TEST_CHECK(strcmp(args.fifoname, "fifopath") == 0);
        TEST_CHECK(args.nplaces == 3);
        TEST_CHECK(args.nthreads == 4);
        TEST_CHECK(server_args_dtor(&args) == EXIT_SUCCESS);

        free_argv(argc, argv);
    }
    {
        int argc = 4;
        const char *argv_const[5] = {"prog", "-t", "5", "fifopath", NULL};
        char **argv = deconstify_argv(argc, argv_const);

        server_args_t args;
        TEST_CHECK(server_args_ctor(&args, argc, argv, MAX_THREADS) == EXIT_SUCCESS);
        TEST_CHECK(args.nsecs == 5);
        TEST_CHECK(args.nplaces == INT_MAX);
        TEST_CHECK(args.nthreads == MAX_THREADS);
        TEST_CHECK(strcmp(args.fifoname, "fifopath") == 0);
        TEST_CHECK(server_args_dtor(&args) == EXIT_SUCCESS);

        free_argv(argc, argv);
    }
}

void test_server_exit_without_readers(void) {
    {
        double start; get_seconds_since_epoch(&start);
        int ret = system("./server -t 2 /tmp/fifo");
        double finish; get_seconds_since_epoch(&finish);
        TEST_CHECK(ret == 0);
        double elapsed = finish-start;
        if (!TEST_CHECK(2.00 <= elapsed) ||
            !TEST_CHECK(elapsed < 2.01)){
            TEST_MSG("Elapsed time: %f (should be around 2.0)", elapsed);
        }
    }
}

TEST_LIST = {
    {"server_args_ctor"             , test_server_args_ctor             },
    {"server_exit_without_readers"  , test_server_exit_without_readers  },
    {NULL, NULL}
};
