#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashTable.h"


HT_Item* createHT_Item(unsigned int pN, char v, unsigned int o, int processNum) {

    HT_Item* item = malloc(sizeof(HT_Item));
    item->pageNum = pN;
    item->value = v;
    item->offset = o;
    item->processNum = processNum;
    item->next = NULL;
    printf("*Item %x Created!!!\n", item->pageNum);
    return item;

}


void deleteHT_Item(HT_Item* item) {

    free(item);

}


HashTable* createHashTable(int size) {

    HashTable* ht = malloc(sizeof(HashTable));
    ht->size = size;
    ht->count = 0;
    ht->items = malloc(size*sizeof(HT_Item*));
    for(int i = 0; i < size; i++){
      ht->items[i] = NULL;
    }
    printf("Hash Table created!\n" );
    return ht;

}



void deleteHashTable(HashTable* ht) {

    for (int i = 0; i < ht->size; i++) {
        HT_Item* item = ht->items[i];
        if (item != NULL) {
            deleteHT_Item(item);
        }
    }
    free(ht->items);
    free(ht);

}



int Hash_Function(unsigned int key, int size){

  long p = 1302397;
  long a = 35759;
  long b = 128932;

      return ((a*key+b) % p) % size;
  }



int HT_Search(HashTable* ht, unsigned int key, int processNum) {

    int index = Hash_Function(key, ht->size);
    HT_Item* item = ht->items[index];
    if(item == NULL){
      return -1;
    }
    do{
      if(item->pageNum == key && item->processNum == processNum)
        return 0;
      else
        item = item->next;
    }
    while(item != NULL);

    return -1;
  }


int HT_Insert(HashTable* ht, unsigned int key, char value, unsigned int offset, int processNum) {

      int index = Hash_Function(key, ht->size);
      HT_Item* cur_item = ht->items[index];

      int search = HT_Search(ht, key, processNum);
      if(search == 0 ){
        do{
          if(cur_item->pageNum == key && cur_item->value == 'R' && value == 'W'){
            cur_item->value = 'W';
            printf("Item %x updated\n",key);
            return -1;
          }
          cur_item = cur_item->next;
        }
        while (cur_item != NULL);
        return -1;
      }

      HT_Item* item = createHT_Item(key, value, offset, processNum);

      if(cur_item == NULL){
        ht->count++;
        ht->items[index] = item;
        printf("Item %x inserted\n", key);
        return 0;
        }
        else{
          while (cur_item->next != NULL) {
            cur_item = cur_item->next;
          }
          cur_item->next = item;
          ht->count++;
          printf("Item %x inserted\n", item->pageNum);
          return 0;
        }
      return -1;
  }


void HT_print(HashTable* ht){

  HT_Item* item;
  for(int i = 0; i < ht->size; i++){
    item = ht->items[i];
    printf("Bucket no%d\n", i );
    while(item != NULL){
      printf("Item with page Number %x and value %c and offset %x\n", item->pageNum, item->value , item->offset);
      item = item->next;
    }

  }
}


void HT_Remove(HashTable* ht, HT_Item* elem){

	int index = Hash_Function(elem->pageNum, ht->size);
  HT_Item* item = ht->items[index];
  HT_Item* prev = NULL;

  while(item != NULL){
    if(item->pageNum == elem->pageNum && item->processNum == elem->processNum){
      if(prev != NULL){
        prev->next = item->next;
        deleteHT_Item(item);
        ht->count--;
        return;
      }
      else{
          ht->items[index] = item->next;
          deleteHT_Item(item);
          ht->count--;
          return;
      }
    }
    else{
      prev = item;
      item = item->next;}
  }
  printf("Deleted!\n" );

}


int HT_RemoveAll(HashTable* ht, int type){

  int writes = 0;
  HT_Item* temp;
  for (int i = 0; i < ht->size; i++) {
        HT_Item* item = ht->items[i];
         while(item != NULL) {
           if(item->processNum == type){
             if(item->value == 'W' )
               writes++;
            temp = item->next;
            HT_Remove(ht, item);
            item = temp;
          }
          else
            item = item->next;
        }
    }
  return writes;
}
