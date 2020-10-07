#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h> 


void  ChildProcess(int []);
void  ParentProcess(int []);

int ShmID;
int *ShmPTR;
pid_t pid;
int status;

int  main(int  argc, char *argv[])
{
     


     ShmID = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
     ShmPTR = (int *) shmat(ShmID, NULL, 0);

     ShmPTR[0] = 0; // representative of the bank account
     ShmPTR[1] = 0; // representative of turn

     pid = fork();
  
     if(pid == 0) { // child process
       
       ChildProcess(ShmPTR);
       
     } else { // parent process
       
       ParentProcess(ShmPTR);
     }
  
  
  
     exit(0);
}


void  ChildProcess(int *SharedMem)
{
     time_t t;
     srandom((unsigned) time(&t));
     int request;
     sleep(random() % 5);
     
       int j;
       for(j = 0; j < 25; j++) {

         while(ShmPTR[1] != 1); // no-op
  
         request = random() % 50; // generate random balance request
         printf("Poor Student needs $%d\n", request);

         if(request <= ShmPTR[0]) {
           ShmPTR[0] = ShmPTR[0] - request;
           printf("Poor Student: Withdraws $%d / Balance = $%d\n", request, ShmPTR[0]);
         } else if (request > ShmPTR[0]) {
           printf("Poor Student: Not Enough Cash ($%d)\n", ShmPTR[0] );
         }

         ShmPTR[1] = 0;

       }
       
       exit(0);
}

void  ParentProcess(int *SharedMem)
{
     time_t t;
     srandom((unsigned) time(&t));
     int gift;
     

     int i;
     for(i = 0; i < 25; i++) {
       
       sleep(random() % 5);

       while(ShmPTR[1] != 0); // no-op

       if(ShmPTR[0] <= 100) {

         gift = random() % 100; // generate random balance gift
         if(gift % 2 == 0) {

           ShmPTR[0] = ShmPTR[0] + gift;
           printf("Dear old Dad: Deposits $%d / Balance = $%d\n", gift, ShmPTR[0]);
         } else if(gift % 2 == 1) {

           printf("Dear old Dad: Doesn't have any money to give\n");
         }
       
     } else {
       
         printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", ShmPTR[0]);

     }
     ShmPTR[1] = 1;
       
     }
    
     wait(&status);

}