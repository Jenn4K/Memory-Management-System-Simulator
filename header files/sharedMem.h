#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include "HashTable.h"
#include <sys/resource.h>


typedef struct shMem{

  unsigned int pageNum;
  char value;                        
  unsigned int offset;
}shMem_Seg;


int shMemCreate(key_t,int);     /*function to create shared memory*/

int shMemDelete(int);       /*function to delete shared memory*/


shMem_Seg* shMemAttach(int);        /*function to get a pointer to the segment*/

int shMemDetach(shMem_Seg *);    /*function to detach shared memory*/
