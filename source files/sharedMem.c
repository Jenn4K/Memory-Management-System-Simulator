#include "sharedMem.h"


int shMemCreate(key_t key, int q){

  if(key < 0){
    fprintf(stderr, "Invalid Key\n");
    exit(EXIT_FAILURE);
  }

  return shmget(key, 2*q*sizeof(struct HT_Item), IPC_CREAT | 0666);

}


int shMemDelete(int shMem_id){

   return shmctl(shMem_id, IPC_RMID, 0);

}


shMem_Seg* shMemAttach(int shMem_id){

  return shmat(shMem_id, (void *)0, 0);

}


int shMemDetach(shMem_Seg *shMem_ptr){

  return shmdt(shMem_ptr);

}
