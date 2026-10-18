#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HashTable/HashTable.h"
#include "Pass1/Pass1.h"
#include "Pass2/Pass2.h"

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: %s <input_name> <output_name>\n", argv[0]);
        printf("Example: %s program2.asm program2.mc\n", argv[0]);
        exit(1);
    }


    const char *folder_prefix = "./Test/";

    char final_input_path[150];
    char final_output_path[150];


    snprintf(final_input_path, sizeof(final_input_path), "%s%s", folder_prefix, argv[1]);
    snprintf(final_output_path, sizeof(final_output_path), "%s%s", folder_prefix, argv[2]);


    FILE *fp = fopen(final_input_path, "r");
    if (fp == NULL) {
        printf("Error: Cannot open file at %s\n", final_input_path);
        exit(1);
    }


    int symbol_counter = findSymTabLen(fp);
    HashTable* hash_table = create_hash_table(symbol_counter);
    rewind(fp);
    fillSymTab(hash_table, fp);


    FILE *outputFile = fopen(final_output_path, "w");
    if (outputFile == NULL) {
        printf("Error: Cannot create output file at %s\n", final_output_path);
        fclose(fp);
        exit(1);
    }


    rewind(fp);
    pass2(fp, hash_table, outputFile);

    fclose(outputFile);
    fclose(fp);

    printf("[SUCCESS] Assembled '%s' into '%s' successfully.\n", argv[1], argv[2]);
    return 0;
}