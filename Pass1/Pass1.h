#ifndef PASS1_PASS1_H
#define PASS1_PASS1_H

#include "../HashTable/HashTable.h"
#include <stdbool.h>

bool is_instruction(char *word);
void fillSymTab(HashTable *table ,FILE *inputFile);
int findSymTabLen(FILE *inputFile);

#endif //PASS1_PASS1_H



