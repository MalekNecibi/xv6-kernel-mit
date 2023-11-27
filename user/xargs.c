#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

#define MAX_COMMAND_LENGTH (MAXARG * 32)    // 1024

#ifndef stdin
#define stdin 0
#endif

void clone_args(char* old_argv[], int old_argc, char* new_argv, char* argv[]);

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        printf("Error: please include at least one argument\n");
        exit(1);
    }

    char command[MAX_COMMAND_LENGTH + 1];
    int length = 0;
    char current;

    
    while (read(stdin, &current, 1) > 0) {

        // fgets
        if ('\n' == current) {
            command[length] = 0;
            length = 0;
            
            // Successfully extracted a complete line!

            if (0 == fork()) {
                // Child Process
                int new_argc = argc;
                char* new_argv[MAXARG];

                clone_args(argv+1, argc-1, command, new_argv);
                new_argv[new_argc] = 0;

                exec(new_argv[0], new_argv);    // Run the Command

                exit(0);
            }
            
            // Parent : Wait for child to complete the command
            wait((int *)0);
        
        } else {
            if (length >= MAX_COMMAND_LENGTH) {
                printf("Error: the resulting command is too long... (limit %d characters)\n", MAX_COMMAND_LENGTH);
                exit(1);

            } else {
                command[length] = current;
                length++;
            }
        }
    }

    exit(0);
}


// TODO : modified from
void clone_args(char* old_argv[], int old_argc, char* new_argv, char* argv[]) {
    int k = 0;
    for (int i = 0; i < old_argc; i++) {
        argv[k] = malloc(strlen(old_argv[i]) + 1);
        memcpy(argv[k++], old_argv[i], strlen(old_argv[i]) + 1);
    }
    argv[k] = malloc(strlen(new_argv) + 1);
    memcpy(argv[k++], new_argv, strlen(new_argv) + 1);
}

