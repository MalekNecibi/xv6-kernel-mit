#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]) {

    int p0[2];  // child  -> parent
    int p1[2];  // parent -> child
    int input;
    int output;
    char buf[2] = {0};

    // p0[0] = parent stdin
    // p0[1] = child stdout
    // p1[0] = child stdin
    // p1[1] = parent stdout
    pipe(p0);
    pipe(p1);

    int pid = fork();
    if (pid > 0) {          // Parent Process
        // indicate parent's input and output
        input  = p0[0];
        output = p1[1];

        // send byte to child
        write(output, "A", 1);

        // receive byte back from child
        if (1 != read(input, buf, 1)) {
            printf("ERROR: failed to read bytes from parent\n");
            exit(1);
        }
        printf("%d: received pong\n", getpid());

        exit(0);


    } else if (0 == pid) {// Child Process
        // indicate child's input and output pipes
        input  = p1[0];
        output = p0[1];

        // receive byte from parent
        if (1 != read(input, buf, 1)) {
            printf("ERROR: failed to read bytes from parent\n");
            exit(1);
        }
        printf("%d: received ping\n");

        // send byte back to parent pipe
        write(output, buf, 1);

        exit(0);


    } else {
        printf("fork error\n");
        exit(1);
    }

    exit(0);
}
