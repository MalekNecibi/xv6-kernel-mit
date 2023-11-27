#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_PIPELINE 35
#define NUM_CHARS_IN_INT sizeof(int) / sizeof(char)

union Int_Char_Array {
    int integer;
    char chars[NUM_CHARS_IN_INT];
} num;

int main(int argc, char* argv[]) {
    
    // parent process
    // pass every number 2-35 to first child process instance
    
    int p[2];
    //char num[32] = {0};  // INT_MIN == –9,223,372,036,854,775,808
    //char num[ NUM_CHARS_IN_INT + 1 ] = {0};  // store int value within char, plus terminator for safety
    
    // p[0] : child_input
    // p[1] : parent_output
    pipe(p);

    int pid = fork();
    if (pid > 0) {
        // Seed to Pipeline (Parent Process)
        
        close(p[0]);    // seed doesn't receive input
        
        for (int i = 2; i < MAX_PIPELINE; i++) {
            num.integer = i;
            write(p[1], num.chars, sizeof(int));
        }
        
        close(p[1]);

        // wait for child to complete
        pid = wait((int *) 0);
        printf("parent finished waiting for children\n", pid);
        exit(0);
    
    
    } else if (0 == pid) {
        // Primeness Pipeline (Child Processes)
        
        int input;
        int output;

        close(p[1]);    // pipeline doesn't output to seed (only receives input)
        input = dup(p[0]);
        close(p[0]);
        
        // make new pipe (current_prime->next_prime)
        
        pipe(p);
        output = p[1];
        // p[0] will be used by next process in pipeline
        
        // only create new proces after receiving first input, then never again
        int pid = fork();
        // fork create next pipeline process

        
        // receive from preceding pipeline (or generating process)
        read(input, num.chars, sizeof(int));
        
        // use first number passed in as n (e.g. 2)
        printf("prime %d\n", num.integer);
        
        
        // current_prime
            // save first number : n
                // print prime n
            // configure output pipe to next
            // for each received input number
                // if input % n != 0
                    // pass to next
            // wait for next to exit

        // next_prime
            // configure input pipe from prev
            // receive from prev
            // same as current_prime
        
        // for each piped number
        while (read(input, num.chars, sizeof(int))) {
            // if num % n == 0
                // drop
            // else (not multiple of n)
                // pipe to next process
            
            
        }

        //printf("child: exiting\n");
        exit(0);
    

    } else {
        printf("fork error\n");
        exit(1);
    }

    
    
    

    printf("main process exiting\n");
    exit(0);
}
