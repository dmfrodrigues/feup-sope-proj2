#include "utils.h"

#include <stdlib.h>
#include <stdbool.h>

int random_range(int lower, int upper) {
    static bool first_time = true;
    if(first_time){
        first_time = false;
        srand(0);
    }

    long random_ms = lower + (rand() % (upper - lower + 1));
    return random_ms;
}
