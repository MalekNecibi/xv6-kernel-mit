#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]) {

    if (argc <= 1) {
        printf("ERROR: please include how many ticks : `sleep 10`\n");

    } else {
        int num_ticks = atoi(argv[1]);
        sleep(num_ticks);
    }

    exit(0);
}
