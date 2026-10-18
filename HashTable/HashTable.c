#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

char *DELETED = "*";

void trim_end(char *str) {
    if (str == NULL) return;
    int i = strlen(str) - 1;

    while (i >= 0 && isspace((unsigned char) str[i])) {
        i--;
    }
    if (i != strlen(str) - 1) {
        str[i + 1] = '\0';
    }
}

HashTable *create_hash_table(int size) {
    HashTable *table = (HashTable *) malloc(sizeof(HashTable));
    if (!table) return NULL;

    table->size = size;

    table->keys = (char **) malloc(size * sizeof(char *));

    table->values = (int *) malloc(size * sizeof(int));

    for (int i = 0; i < size; i++) {
        table->keys[i] = NULL;

        table->values[i] = -1;
    }

    return table;
}

int hash(char key[], int size) {
    int holder = 0;

    for (int i = 0; key[i] != '\0'; i++) {
        holder += key[i];
    }
    return holder % size;
}


int calc_index(HashTable *table, int i, char key[]) {
    return (hash(key, table->size) + i) % table->size;
}

void insert(HashTable *table, char key[], int value) {
    trim_end(key);
    for (int i = 0; i < table->size; i++) {
        int index = calc_index(table, i, key);

        if (table->keys[index] == NULL || table->keys[index] == DELETED) {
            table->keys[index] = strdup(key);
            table->values[index] = value;
            return;
        }

        if (strcmp(table->keys[index], key) == 0) {
            table->values[index] = value;
            return;
        }
    }
    printf("Error: Hashtable overflow\n");
}

int search(HashTable *table, char key[]) {
    trim_end(key);
    for (int i = 0; i < table->size; i++) {
        int index = calc_index(table, i, key);

        if (table->keys[index] == NULL) {
            return -1;
        }

        if (table->keys[index] != DELETED) {
            if (strcmp(table->keys[index], key) == 0) {
                return table->values[index];
            }
        }
    }
    return -1;
}

int delete_value(HashTable *table, char key[]) {
    for (int i = 0; i < table->size; i++) {
        int index = calc_index(table, i, key);

        if (table->keys[index] == NULL) {
            return -1;
        }

        if (table->keys[index] != DELETED) {
            if (strcmp(table->keys[index], key) == 0) {
                free(table->keys[index]);
                table->keys[index] = DELETED;
                return 1;
            }
        }
    }
    return -1;
}