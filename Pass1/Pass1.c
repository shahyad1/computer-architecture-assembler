#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Pass1.h"

#include <ctype.h>

const char *INSTRUCTIONS[] = {
        "add", "sub", "slt", "or", "nand", "addi", "slti", "ori", "lui",
        "lw", "sw", "beq", "j", "jalr", "halt", ".fill", ".space"
};

bool is_instruction(char *word) {
    for (int i = 0; i < 17; i++) {
        if (strcmp(word, INSTRUCTIONS[i]) == 0) {
            return true;
        }
    }
    return false;
}

void fillSymTab(HashTable *table, FILE *inputFile) {

    char line[73];
    int address = 0;
    int line_number = 0;
    const char *delimiters = " \t\n\r";

    while (fgets(line, sizeof(line), inputFile) != NULL) {

        line_number++;
        char temp[73];
        strcpy(temp, line);
        char *token = strtok(temp, delimiters);
        if (token == NULL) {
            continue;
        }
        char *label = NULL;
        if (!is_instruction(token)) {
            label = token;
            token = strtok(NULL, delimiters);


            if (search(table, label) != -1) {
                printf("Error in line %d column 1 : label '%s' is already defined.\n", line_number,
                       label);
                exit(1);
            } else {
                insert(table, label, address);
            }
        }

        if (strcmp(token, ".space") == 0) {
            char *field0 = strtok(NULL, delimiters);
            int holder = strtol(field0, NULL, 10);
            address += holder;
        } else {
            address += 1;
        }
    }
}


int findSymTabLen(FILE *inputFile) {

    char line[73];
    int symbol_counter = 0;
    const char *delimiters = " \t\n\r";

    while (fgets(line, sizeof(line), inputFile) != NULL) {

        char temp[73];
        strcpy(temp, line);
        char *token = strtok(temp, delimiters);
        if (token == NULL) {
            continue;
        }
        if (!is_instruction(token)) {
            symbol_counter ++;
        }
    }
    return symbol_counter;
}
