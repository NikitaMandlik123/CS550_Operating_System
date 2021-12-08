#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/shm.h>
#include <sys/ipc.h>
#include <errno.h>
#include <semaphore.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/wait.h>
#define SHM_SIZE 1024 /* make it a 1K shared memory segment */



struct shMemory { // creating a structure
  int data1;
  int data2;
  int sum;
  int winner;
  sem_t semaphore;
};


  struct shMemory * shM;
  int shmid;

void lock() { //created a function  for locking

    while (sem_trywait( & shM -> semaphore) < 0) {
      if (errno == EAGAIN)
        continue;
      else perror("sem_trywait failed:");
    }
    //printf("Lock Acquired\n");
  }

  void unlock() { //created a function for unlocking
    sem_post( & shM -> semaphore);
    //printf("Lock released\n");
  }

int main(int argc, char * argv[]) {
  if (argc != 3) //Check for the  correct number of arguments
  {
    printf("Wrong number of arguments");
    exit(1);
  }
  int process = atoi(argv[1]);//taking inputs from command line and converting it into Integer
  int maxNumb = atoi(argv[2]);
  pid_t pid;
  key_t key;
  

  
  if ((key = ftok("EP.txt", 'X')) < 0) { // unique key address
    perror("ftok");
    exit(1);
  }


  if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT | IPC_EXCL)) < 0) { // creating shared memory
    perror("shmget");
    exit(1);
  }

  shM = shmat(shmid, (void * ) 0, 0);//attaching shM to created shared memory
  if (shM == (struct shMemory * )(-1)) {
    perror("shmat");
    exit(1);
  }

  if (sem_init( & shM -> semaphore, 1, 1) < 0) {// initializing semaphore
    perror("sem_init:");
    exit(1);
  }



  shM -> data1 =1;// initializing data1 
  shM ->data2 =2; // initialixinf data2
  shM->winner = -1;// let the winner be -1 till the winner has not been found
  

  

  for (int i = 0; i < process; i++) {
    pid = fork();

    if (pid == -1) {
      printf("forkFailed");
	  exit(0);
    }
  
    if (pid == 0) {

      while (1) {
        lock(); //lock
        
        if(shM->winner != -1){
        	unlock();
        	exit(0);
        }
        if (shM -> data1 > maxNumb || shM -> data2 > maxNumb) {
          shM -> winner = getpid();
          unlock();
          exit(0);
        }

        shM -> sum = shM -> data1 + shM -> data2;
        if (shM -> data1 < shM -> data2)
          shM -> data1 = shM -> sum;
        else
			shM -> data2 = shM -> sum;
			printf("Sum %d\n", shM-> sum);
			unlock();
      }

    }
    }

  if (pid > 0) {
    for (int i = 0; i < process; i++) {
      wait(NULL);
    }
	
    
    if (shM -> data1 > shM -> data2) 
      printf("\nWinner process is %d and max value is %d\n",shM -> winner,shM -> data1);
    else 
      printf("\nWinner process is %d and max value is %d\n",shM -> winner,shM -> data2);
 

  }

  sem_destroy( & shM -> semaphore);

  if (shmdt(shM) == -1) {
    perror("shmdt");
    exit(1);
  }
  // delete the segment 
  if( shmctl(shmid, IPC_RMID, NULL) == -1) {
       perror("shmctl");
       exit(1);
  }

}
