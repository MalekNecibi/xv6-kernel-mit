#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_PIPELINE 35
enum pipe_ops{ Read, Write };

int main(int argc, char *argv[]) {
	int parent;
    int child[2];

    pipe(child);

    // Process: Seed to Pipeline
	if (0 == fork()) {
		for (int i = 2; i <= MAX_PIPELINE; i++) {
			write(child[Write], &i, sizeof(int));
		}
		close(child[Write]);

		exit(0);
	}

	close( child[Write] );
	parent = child[Read];
	

	int prime; 
    while (read(parent, &prime, sizeof(int))) {
		printf("prime %d\n", prime); 
		
		int child[2];
		int candidate;

		pipe(child);
//        printf("new pipe : %d,%d\n", child[Read], child[Write]);
        
        // Process: Syphon out invalid candidates
		if (0 == fork()) {
			while (read(parent, &candidate, sizeof(int))) {
				if (0 != candidate % prime) {
					write(child[Write], &candidate, sizeof(int));
				}
			}

			close(child[Write]);
			close(parent);
			exit(0);
		}
        
        // process complete, prep for next in pipeline
		close(parent);
		close(child[Write]);
		parent = child[Read];
	
    }
    wait((int *) 0);
	exit(0);
}

