#include "Pass2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include "../Pass1/Pass1.h"

void initializeInstructions(HashTable *table) {
    insert(table, "add", 0);
    insert(table, "sub", 1);
    insert(table, "slt", 2);
    insert(table, "or", 3);
    insert(table, "nand", 4);
    insert(table, "addi", 5);
    insert(table, "slti", 6);
    insert(table, "ori", 7);
    insert(table, "lui", 8);
    insert(table, "lw", 9);
    insert(table, "sw", 10);
    insert(table, "beq", 11);
    insert(table, "jalr", 12);
    insert(table, "j", 13);
    insert(table, "halt", 14);
    insert(table, ".fill", 15);
    insert(table, ".space", 16);
}

void pass2(FILE *inputFile , HashTable *symbolTable , FILE *outputFile) {
    HashTable *instructions = create_hash_table(17);
    initializeInstructions(instructions);
    char line[73];
    int currentPC = 0;
    const char *delimiters = " \t,\n\r";

    while (fgets(line, sizeof(line), inputFile) != NULL) {
        char temp[73];
        strcpy(temp, line);
        char machinCodeLine[10];
        char *token = strtok(temp, delimiters);
        if (token == NULL) {
            continue;
        }
        if (!is_instruction(token)) {
            token = strtok(NULL, delimiters);
        }

        int opcode = search(instructions, token);
        if (opcode >= 0 && opcode <= 4) { // R type instruction
            char opcodeHex[2];
            sprintf(opcodeHex, "%X", opcode);
            machinCodeLine[0] = '0';
            machinCodeLine[1] = opcodeHex[0];
            token = strtok(NULL, delimiters);
            int rdInt = strtol(token, NULL, 10);
            char rdHex[2];
            sprintf(rdHex, "%X", rdInt);
            machinCodeLine[4] = rdHex[0];
            token = strtok(NULL, delimiters);
            int rsInt = strtol(token, NULL, 10);
            char rsHex[2];
            sprintf(rsHex, "%X", rsInt);
            machinCodeLine[2] = rsHex[0];
            token = strtok(NULL, delimiters);
            int rtInt = strtol(token, NULL, 10);
            char rtHex[2];
            sprintf(rtHex, "%X", rtInt);
            machinCodeLine[3] = rtHex[0];
            for (int i = 5; i < 8; i++) {
                machinCodeLine[i] = '0';
            }

        } else if (opcode == 8) { // lui instruction
            char opcodeHex[2];
            sprintf(opcodeHex, "%X", opcode);
            machinCodeLine[0] = '0';
            machinCodeLine[1] = opcodeHex[0];
            machinCodeLine[2] = '0';// do not have RS in this instruction
            token = strtok(NULL, delimiters);
            int rtInt = strtol(token, NULL, 10);
            char rtHex[2];
            sprintf(rtHex, "%X", rtInt);
            token = strtok(NULL, delimiters);   // immediate
            machinCodeLine[3] = rtHex[0];
            int immediateValue;
            if ((token[0] <= '9' && token[0] >= '0') || token[0] == '-') {
                immediateValue = strtol(token, NULL, 10);
            } else {
                immediateValue = search(symbolTable, token);
                if (immediateValue == -1) {
                    printf("No such symbol defined\n");
                    exit(1);
                }
            }
            char immHex[4];
            int2hex16(immHex, immediateValue);
            machinCodeLine[4] = immHex[0];
            machinCodeLine[5] = immHex[1];
            machinCodeLine[6] = immHex[2];
            machinCodeLine[7] = immHex[3];

        } else if (opcode == 12) {// jalr instruction
            char opcodeHex[2];
            sprintf(opcodeHex, "%X", opcode);
            machinCodeLine[0] = '0';
            machinCodeLine[1] = opcodeHex[0];
            token = strtok(NULL, delimiters);
            int rtInt = strtol(token, NULL, 10);
            char rtHex[2];
            sprintf(rtHex, "%X", rtInt);
            machinCodeLine[3] = rtHex[0];
            token = strtok(NULL, delimiters);
            int rsInt = strtol(token, NULL, 10);
            char rsHex[2];
            sprintf(rsHex, "%X", rsInt);
            machinCodeLine[2] = rsHex[0];
            for (int i = 4; i < 8; i++) {
                machinCodeLine[i] = '0';
            }

        } else if (opcode == 14) {
            char opcodeHex[2];
            sprintf(opcodeHex, "%X", opcode);
            machinCodeLine[0] = '0';
            machinCodeLine[1] = opcodeHex[0];
            for (int i = 2; i < 8; i++) {
                machinCodeLine[i] = '0';
            }

        } else if (opcode == 15) {
            token = strtok(NULL, delimiters);
            int immediateValue;
            if ((token[0] <= '9' && token[0] >= '0') || token[0] == '-') {
                immediateValue = strtol(token, NULL, 10);
            } else {
                immediateValue = search(symbolTable, token);
                if (immediateValue == -1) {
                    printf("No such symbol defined\n");
                    exit(1);
                }
            }
            char numberHex[4];
            int2hex16(numberHex, immediateValue);
            char extendedValue[10];
            extend16To32(extendedValue, numberHex);
            extendedValue[8] = '\n';
            extendedValue[9] = '\0';
            long int lineToFile = hex2int(extendedValue);
            fprintf(outputFile, "%ld\n", lineToFile);
            currentPC++;
            continue;

        } else if (opcode == 16) {
            token = strtok(NULL, delimiters);
            int immediateValue;
            if ((token[0] <= '9' && token[0] >= '0') || token[0] == '-') {
                immediateValue = strtol(token, NULL, 10);
            } else {
                immediateValue = search(symbolTable, token);
                if (immediateValue == -1) {
                    printf("No such symbol defined\n");
                    exit(1);
                }
            }
            for (int i = 0; i < immediateValue; i++) {
                fprintf(outputFile, "0\n");
                currentPC++;
            }
            continue;

        } else if (opcode >= 5 && opcode <= 7) {
            char opcodeHex[2];
            sprintf(opcodeHex, "%X", opcode);
            machinCodeLine[0] = '0';
            machinCodeLine[1] = opcodeHex[0];

            token = strtok(NULL, delimiters);
            int rtInt = strtol(token, NULL, 10);
            char rtHex[2];
            sprintf(rtHex, "%X", rtInt);
            machinCodeLine[3] = rtHex[0];

            token = strtok(NULL, delimiters);
            int rsInt = strtol(token, NULL, 10);
            char rsHex[2];
            sprintf(rsHex, "%X", rsInt);
            machinCodeLine[2] = rsHex[0];

            token = strtok(NULL, delimiters);
            int immediateValue;
            if ((token[0] <= '9' && token[0] >= '0') || token[0] == '-') {
                immediateValue = strtol(token, NULL, 10);
            } else {
                immediateValue = search(symbolTable, token);
                if (immediateValue == -1) {
                    printf("No such symbol defined\n");
                    exit(1);
                }
            }
            char immHex[4];
            int2hex16(immHex, immediateValue);
            machinCodeLine[4] = immHex[0];
            machinCodeLine[5] = immHex[1];
            machinCodeLine[6] = immHex[2];
            machinCodeLine[7] = immHex[3];

        } else if (opcode == 9 || opcode == 10) {
            char opcodeHex[2];
            sprintf(opcodeHex, "%X", opcode);
            machinCodeLine[0] = '0';
            machinCodeLine[1] = opcodeHex[0];

            token = strtok(NULL, delimiters);
            int rtInt = strtol(token, NULL, 10);
            char rtHex[2];
            sprintf(rtHex, "%X", rtInt);
            machinCodeLine[3] = rtHex[0];

            token = strtok(NULL, delimiters);
            int rsInt = strtol(token, NULL, 10);
            char rsHex[2];
            sprintf(rsHex, "%X", rsInt);
            machinCodeLine[2] = rsHex[0];

            token = strtok(NULL, delimiters);
            int offsetValue;
            if ((token[0] <= '9' && token[0] >= '0') || token[0] == '-') {
                offsetValue = strtol(token, NULL, 10);
            } else {
                offsetValue = search(symbolTable, token);
                if (offsetValue == -1) {
                    printf("No such symbol defined\n");
                    exit(1);
                }
            }
            char offsetHex[4];
            int2hex16(offsetHex, offsetValue);
            machinCodeLine[4] = offsetHex[0];
            machinCodeLine[5] = offsetHex[1];
            machinCodeLine[6] = offsetHex[2];
            machinCodeLine[7] = offsetHex[3];

        } else if (opcode == 11) {
            char opcodeHex[2];
            sprintf(opcodeHex, "%X", opcode);
            machinCodeLine[0] = '0';
            machinCodeLine[1] = opcodeHex[0];

            token = strtok(NULL, delimiters);
            int rtInt = strtol(token, NULL, 10);
            char rtHex[2];
            sprintf(rtHex, "%X", rtInt);
            machinCodeLine[3] = rtHex[0];

            token = strtok(NULL, delimiters);
            int rsInt = strtol(token, NULL, 10);
            char rsHex[2];
            sprintf(rsHex, "%X", rsInt);
            machinCodeLine[2] = rsHex[0];

            token = strtok(NULL, delimiters);
            int offsetValue;
            if ((token[0] <= '9' && token[0] >= '0') || token[0] == '-') {
                offsetValue = strtol(token, NULL, 10);
            } else {
                int labelAddress = search(symbolTable, token);
                if (labelAddress == -1) {
                    printf("No such symbol defined\n");
                    exit(1);
                }
                offsetValue = labelAddress - currentPC - 1;
            }
            char offsetHex[4];
            int2hex16(offsetHex, offsetValue);
            machinCodeLine[4] = offsetHex[0];
            machinCodeLine[5] = offsetHex[1];
            machinCodeLine[6] = offsetHex[2];
            machinCodeLine[7] = offsetHex[3];

        } else if (opcode == 13) {
            char opcodeHex[2];
            sprintf(opcodeHex, "%X", opcode);
            machinCodeLine[0] = '0';
            machinCodeLine[1] = opcodeHex[0];
            machinCodeLine[2] = '0';
            machinCodeLine[3] = '0';

            token = strtok(NULL, delimiters);
            int targetAddress;
            if ((token[0] <= '9' && token[0] >= '0') || token[0] == '-') {
                targetAddress = strtol(token, NULL, 10);
            } else {
                targetAddress = search(symbolTable, token);
                if (targetAddress == -1) {
                    printf("No such symbol defined\n");
                    exit(1);
                }
            }
            char targetHex[4];
            int2hex16(targetHex, targetAddress);
            machinCodeLine[4] = targetHex[0];
            machinCodeLine[5] = targetHex[1];
            machinCodeLine[6] = targetHex[2];
            machinCodeLine[7] = targetHex[3];
        }

        machinCodeLine[8] = '\n';
        machinCodeLine[9] = '\0';
        long int lineToFile = hex2int(machinCodeLine);
        fprintf(outputFile, "%ld\n", lineToFile);
        currentPC++;
    }
}

void int2hex16(char *lower, int a) {
    unsigned short val = (unsigned short) (a & 0xFFFF);
    sprintf(lower, "%04X", val);
}

long int hex2int(char *hex) {
    unsigned long result = strtoul(hex, NULL, 16);
    return (int32_t)result;
}

void extend16To32(char *extendedValue, char *hex16) {
    for (int i = 0; i < 4; i++) {
        extendedValue[i] = hex16[0];
    }
    for (int i = 4; i < 8; i++) {
        extendedValue[i] = hex16[i - 4];
    }
}
