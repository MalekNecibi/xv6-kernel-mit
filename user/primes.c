#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_CANDIDATE 35

//#include "kernel/param.h"
//#define NOFILE       16  // open files per process
#define MAX_FD_MASK     0x000000FF    //  TODO : auto-compute from NOFILE

#define MAX_FILE_DESCIPTOR 15
enum pipe_ops{ Read, Write };

void _close(int* fd);
void _pipe(int p[2]);

int main(int argc, char *argv[]) {
    int parent = -1;
    int child[2] = {-1,-1};

    _pipe(child);

    // Process: Seed to Pipeline
    if (0 == fork()) {
        for (int i = 2; i <= MAX_CANDIDATE; i++) {
            write(child[Write], &i, sizeof(int));
        }
        _close(&child[Write]);

        exit(0);
    }

    _close(&child[Write]);
    parent = child[Read];
    child[Read] = -1;
    

    int prime; 
    while (read(parent, &prime, sizeof(int))) {
        printf("prime %d\n", prime); 
        
        int candidate;
        _pipe(child);

        
        // Process: Syphon out invalid candidates
        if (0 == fork()) {
            while (read(parent, &candidate, sizeof(int))) {
                if (0 != candidate % prime) {
                    write(child[Write], &candidate, sizeof(int));
                }
            }

            _close(&child[Write]);
            _close(&parent);
            exit(0);
        }
        
        // process complete, prep for next in pipeline
        _close(&parent);
        _close(&child[Write]);
        parent = child[Read];
        child[Read] = -1;   // no longer refers to the child
    
    }

    wait((int *) 0);
    exit(0);
}

void _close(int* fd) {
    if (!fd || *fd < 0) {
        printf("Warning (_close): Double closing a file descriptor\n");
        return;
    }
    close(*fd);
    // indicate that the file descriptor is no longer valid
    *fd = (-1);
}

void _pipe(int p[2]) {
    if (!p) {
        printf("Error (_pipe): Please provide a location to store the new pipe\n");
        exit(0);
    }
    if (p[0] >= 0 || p[1] >= 0) {
        printf("Warning (_pipe): potentialy overwriting existing file descriptor(s) [%d,%d]\n", p[0], p[1]);
    }

    pipe(p);
    if (p[0] < 0 || p[1] < 0) {
        printf("Error (_pipe): failed to allocate new file descriptors\n");
        exit(0);
    }
}
