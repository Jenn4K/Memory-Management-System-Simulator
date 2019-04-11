#pragma once


typedef struct HT_Item {
    unsigned int pageNum;
    char value;
    unsigned int offset;
    int processNum;
    struct HT_Item* next;
} HT_Item;

typedef struct {
    int size;
    int count;
    HT_Item** items;
} HashTable;


HT_Item* createHT_Item(unsigned int , char, unsigned int, int);
void deleteHT_Item(HT_Item*);

HashTable* createHashTable(int);
void deleteHashTable(HashTable*);

int Hash_Function(unsigned int, int);

int HT_Insert(HashTable* , unsigned int, char, unsigned int, int);
int HT_Search(HashTable* , unsigned int, int);
void HT_print(HashTable*);
void HT_Remove(HashTable* , HT_Item*);
int HT_RemoveAll(HashTable* , int);
