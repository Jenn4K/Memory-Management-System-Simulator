#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include "HashTable.h"
#include "sem.h"
#include "sharedMem.h"

int main(int argc, char* argv[]){

  int k, n, q, max;

  if( argc < 4 || argc > 5 ){                           /*get argument from command line*/
        printf("Invalid number of arguments.\n");
        exit(0);
    }
    else{
        k = atoi(argv[1]);
        n = atoi(argv[2]);
        q = atoi(argv[3]);

        if(argc == 5)
          max = atoi(argv[4]);
        else
          max = 100000;
      }
      if(k > n/2){
        printf("K should not be greater than n/2\n" );
        return -1;
      }
      int status;
      pid_t pid;
      key_t key;
      int ShMem_ID;
      shMem_Seg *ShMem_PTR;
      int empty_1, empty_2,empty_3, mutex_1, mutex_2, mutex_3, full_1, full_2, full_3;
/////////Create key///////////////////////
      if((key = ftok("main.c", 'S')) == -1){                //producing a random key for shared memory
        printf("*** Key wasn't prodused (ftok error) ***\n");
        exit(1);
        }

////////Shared Memory////////////////////////
      ShMem_ID = shMemCreate(key,q);                                 //Creating and Attaching shared memory segment
      if(ShMem_ID < 0){
       printf("***Shared Memory Segment could not be created (shmget error (server)) ***\n");
       exit(1);
     }

     ShMem_PTR = shMemAttach(ShMem_ID);
     if(ShMem_PTR == (shMem_Seg *)(-1)){
       printf("*** Could not attach Shared Memeory Segment (shmat error (server))***\n");
       exit(1);
     }

///////////Semaphores///////////////////////
    empty_1 = semCreate((key_t)8423,1,1);
    if(empty_1 < 0){
      printf("***Semaphore error***\n");
      exit(0);
    }

    empty_2 = semCreate((key_t)5468,1,1);
          if (empty_2 < 0){
              printf("***Semaphore error***\n");
              exit(0);
      }


      mutex_1 = semCreate((key_t)4321,1,1);
          if (mutex_1 < 0){
              printf("***Semaphore error***\n");
              exit(0);
      }

      mutex_2 = semCreate((key_t)2134,1,1);
          if (mutex_2 < 0){
              printf("***Semaphore error***\n");
              exit(0);
      }


      full_1 = semCreate((key_t)1234,1,0);
      if (full_1 < 0){
          printf("***Semaphore error***\n");
          exit(0);
      }

      full_2 = semCreate((key_t)7234,1,0);
      if (full_2 < 0){
          printf("***Semaphore error***\n");
          exit(0);
      }

/////////////////////////////////////////////////////////////////////////////////////
      int count1 = 0;
      int curr1 = q;   ////to be changed
      int rep1 = 0;



      int count = 0;
      int curr = q;
      int temp = max/q;
      int reminder = max%q;
      int rep = 0;

      int i;
      for(i=0; i<3; i++){
      pid = fork();
      if(pid==0){
////////////////////////**P1**///////////////////////////////////////////
        if(i == 0){
          int i = 0;
          FILE * fp;
          char * line = NULL;
          size_t len = 0;
          ssize_t read;
          fp = fopen("bzip.trace", "r");

          while(rep<max){

            sem_P(empty_1,0);
            sem_P(mutex_1,0);

            while (count < curr && (read = getline(&line, &len, fp)) != -1 ) {

              char value;
              unsigned int key, pageNum, offset;

              sscanf(line, "%x %c", &key, &value);

              pageNum = key/4096;
              offset = key%4096;
              ShMem_PTR[count].pageNum = pageNum;
              ShMem_PTR[count].value = value;
              ShMem_PTR[count].offset = offset;

              i++;
              count++;
            }
            sem_V(mutex_1,0);
            sem_V(full_1,0);

            if(rep < temp*q || rep == max-1){
              rep += q;
              curr = q;
            }
            else{
              rep = max-1;
              curr = reminder;
            }
            count = 0;
          }
          printf("EXITING PROCESS P1\n" );
          free(line);
          fclose(fp);
          exit(0);

        }
///////////////////////**P2**/////////////////////////////////////
        else if(i == 1){

          int j =0;
          FILE * sfp;
          char * sline = NULL;
          size_t slen = 0;
          ssize_t sread;
          sfp = fopen("gcc.trace", "r");


          while(rep1<max){

            sem_P(empty_2,0);
            sem_P(mutex_2,0);

            while (count1 < curr1 && (sread = getline(&sline, &slen, sfp)) != -1) {

              char value;
              unsigned int key, pageNum, offset;

              sscanf(sline, "%x %c", &key, &value);
              pageNum = key/4096;
              offset = key%4096;
              ShMem_PTR[q+count1].pageNum = pageNum;
              ShMem_PTR[q+count1].value = value;
              ShMem_PTR[q+count1].offset = offset;

              j++;
              count1++;
            }
          sem_V(mutex_2,0);
          sem_V(full_2,0);
          if(rep1 < temp*q || rep1 == max-1){
            rep1 += q;
            curr1 = q;
          }
          else{
            rep1 = max-1;
            curr1 = reminder;
          }
          count1 = 0;
        }
        printf("EXITING PROCESS P2\n" );
        free(sline);
        fclose(sfp);
        exit(0);

        }
        else{
///////////////////**MM**////////////////////////////////////////
          int tok1 = 0, tok2 = 0;
          int x = 0, tok_counter = 0;
          HashTable* HT = createHashTable(n);
          int curr2 =q;
          int page2;
          int page;
          int reads = 0, writes = 0, maxFrames = 0;
          int p1 = 0, p2 = 0;
          while(x < max){

            sem_P(full_1,0);
            sem_P(mutex_1,0);


            for(int y = 0; y < curr2 ; y++){

            if(HT_Search(HT, ShMem_PTR[y].pageNum, 1) == -1){
              tok1 ++;
            }

            if(tok1 <= k){
            page = HT_Insert(HT, ShMem_PTR[y].pageNum, ShMem_PTR[y].value, ShMem_PTR[y].offset, 1);
            if(HT->count > maxFrames)
              maxFrames = HT->count;
              if (page==0){
                p1++;
                reads++;
                tok_counter++;}
            }
            else{
                printf("FLUSH\n" );
                writes += HT_RemoveAll(HT,1);
                tok1 = 0;
                page = HT_Insert(HT, ShMem_PTR[y].pageNum, ShMem_PTR[y].value, ShMem_PTR[y].offset, 1);
                if(HT->count > maxFrames)
                  maxFrames = HT->count;
                p1++;
                tok1++;
                if(page==0){
                  reads++;
                tok_counter++;}
              }
            }
            sem_V(mutex_1,0);
            sem_V(empty_1,0);

            sem_P(full_2,0);
            sem_P(mutex_2,0);

          for(int z = 0; z < curr2 ; z++){
            if(HT_Search(HT, ShMem_PTR[z+q].pageNum, 2) == -1){
              tok2 ++;
            }
            if(tok2 <= k){
            page2 = HT_Insert(HT, ShMem_PTR[q+z].pageNum, ShMem_PTR[q+z].value, ShMem_PTR[q+z].offset, 2);
            if(HT->count > maxFrames)
              maxFrames = HT->count;
            if (page2==0){
              p2++;
              reads++;
              tok_counter++;}
          }
          else{
                printf("FLUSH\n" );
                writes += HT_RemoveAll(HT, 2);
                tok2 = 0;
                page2 = HT_Insert(HT, ShMem_PTR[z+q].pageNum, ShMem_PTR[z+q].value, ShMem_PTR[z+q].offset, 2);
                if(HT->count > maxFrames)
                  maxFrames = HT->count;
                tok2++;
                if (page2==0){
                  p2++;
                  reads++;
                  tok_counter++;}
                }
        }
            sem_V(mutex_2,0);
            sem_V(empty_2,0);

            if(x < temp*q || x == max-1){
              x += q;
              curr2 = q;
            }
            else{
              x = max-1;
              curr2 = reminder;
            }
          }
          printf("EXITING MM\n" );
          HT_RemoveAll(HT, 1);
          HT_RemoveAll(HT, 2);
          deleteHashTable(HT);
          printf("~~MAX NUMBER OF FRAMES: %d\n", maxFrames );
          printf("~~TOTAL ENTRIES EXAMINED %d\n", 2*x );
          printf("~~TOTAL PAGE FAULTS %d\n", tok_counter );
          printf("~~TOTAL READS %d\n~~TOTAL WRITES %d\n", reads, writes );
          exit(0);
          }
      }
            }
/////////////////////////////////////////////////////////////////////////////
for (;;) {  // Remove the zombie process, and get the pid and return code

pid = wait(&status);

if (pid < 0 ){
if (errno == ECHILD) {
  break;
    }
else {
  perror("Could not wait");
    }
  }
}

/////////////////DELETIONS/////////////////////////////////
if (shMemDetach(ShMem_PTR) == -1) {                  //Detaching and Deleting the first shared memory segment
     printf(" ***Shared Memory Segment could not be detached(shmdt failed)***\n");
     exit(1);
 }
 if (shMemDelete(ShMem_ID) == -1) {
     printf(" ***Shared Memory Segment could not be deleted(shmctl(IPC_RMID) failed)***\n");
     exit(1);
   }

  semDelete(full_1);
  semDelete(full_2);
  semDelete(empty_1);
  semDelete(empty_2);
  semDelete(mutex_1);
  semDelete(mutex_2);

  return 0;
  }
