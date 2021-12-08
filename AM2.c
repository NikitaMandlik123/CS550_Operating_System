#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
pid_t pid;

int noOfLevels;
int noOfChildren;
int a;
int b;

int counter;

if(argc != 3){
        printf("Wrong number of arguments");
        exit(-1);
    }

    noOfLevels = atoi(argv[1]); //coverting the arg[1] into integer
        if(noOfLevels <= 0){
        printf("Insert positive integer as argument");
        exit(EXIT_FAILURE);
    }

    noOfChildren = atoi(argv[2]); //converting the arg[2] into integer
        if(noOfChildren <= 0){
        printf("Insert positive integer as argument");
        exit(EXIT_FAILURE);
    }
    
fork_r(1, noOfLevels, noOfChildren); //start from level one and go until n

    return 0;
   }




void fork_r(int level, int noOfLevels, int noOfChildren){
    
	 for(level = 0 ; level <= noOfLevels ; level++){ // start with the first loop of level 1 to n
	    pid_t pid;
		int status;
		pid = fork();
		if(pid ==0){
		  for( int child =0; child <= noOfChildren ; child++){ //start with the creating child 1 to n
		      if( fork() == 0){
			    printf("DEBUG: LEVEL:%d CHILD:%d pid %d from parent %d\n", level, child, getpid(), getppid());
                        int exec_return_value = execlp(level, child, "-l", (char *) NULL);
                         if (exec_return_value == -1) {
                                 perror("execlp");
                                 exit(EXIT_FAILURE);
                        }
			  }
		  }
		}
		else{
		printf("DEBUG:Parent Process:", getpid()); //parent proceeds
              int return_status = waitpid(pid, &status, 0);
              printf("DEBUG:Return status - %d\n", return_status);
		}
	     
	 }
	 for (level=0; level < noOfLevels; level++) {
                wait(NULL);
        }

}
