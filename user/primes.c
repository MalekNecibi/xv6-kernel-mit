#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_PIPELINE 35
#define NUM_CHARS_IN_INT sizeof(int) / sizeof(char)

union Int_Char_Array {
    int integer;
    char chars[NUM_CHARS_IN_INT];
};

enum pipe_sides{Read, Write};

void swap(int *p0, int *p1);

int main(int argc, char* argv[]) {
    int parent[2];  // [ read, write ]
    int child[2];   // [ read, write ]
    
    union Int_Char_Array num;

    
    pipe(parent);
    pipe(child);

    
    close(parent[Read]);
    
    // Seed to Pipeline
    for (int i = 2; i <= MAX_PIPELINE; i++) {
        num.integer = i;
        write(child[Write], num.chars, sizeof(int));
    }
    close(child[Write]);    // no need to write from seed again
    
    swap(parent, child);

    
    int pid;
    // Pipeline
    while (0 != read(parent[Read], num.chars, sizeof(int))) {
        int prime = num.integer;
        printf("%d: prime %d\n", getpid(), prime);
        
        pipe(child);
         
        pid = fork();
        if (pid > 0) {
            // Parent Process
            
            while(0 != read(parent[Read], num.chars, sizeof(int))) {
                if (0 != num.integer % prime) {
                    write(child[Write], num.chars, sizeof(int));
                }
            }
            
            close(child[Write]);
            close(parent[Read]);
                
            printf("%d*", getpid());
            wait((int *) 0);
            printf("|");
            exit(0);

            
        
        } else if (0 == pid) {
            // Spawned next process in pipeline
            swap(parent, child);
            continue;
        }
    }
    
    //close(parent[Read]);

    printf("done\n");
                

    exit(0);
}

void swap(int *p0, int *p1) {
    int a, b;

    a = p0[0];
    b = p0[1];

    p0[0] = p1[0];
    p0[1] = p1[1];

    p1[0] = a;
    p1[1] = b;
    
    return;
}
