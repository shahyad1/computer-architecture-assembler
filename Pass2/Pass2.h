#ifndef PASS1_PASS2_H
#define PASS1_PASS2_H
#include <stdio.h>
#include "./../HashTable/HashTable.h"

void initializeInstructions(HashTable *table);
void pass2(FILE *inputFile , HashTable *symbolTable , FILE *outputFile);
void int2hex16(char *lower, int a);
void extend16To32(char *extendedValue , char *hex16);
int long hex2int(char *hex);


#endif //PASS1_PASS2_H