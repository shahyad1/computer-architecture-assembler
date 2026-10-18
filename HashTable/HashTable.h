#ifndef PASS1_HASHTABLE_H
#define PASS1_HASHTABLE_H
#include <stddef.h>

typedef struct {
    int size;
    char **keys;
    int *values;
} HashTable;

HashTable* create_hash_table(int size);
void insert(HashTable *table, char key[], int value);
int search(HashTable *table, char key[]);
int calcindex(HashTable *table, int index , char key[]);
int hash(char key[] , int size);
int delete_value(HashTable* table,  char key[]);


#endif //PASS1_HASHTABLE_H