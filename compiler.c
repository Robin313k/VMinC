#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_CODE 1024
#define MAX_LABELS 100

uint8_t bytecode[1024];
int pc = 0;

typedef struct {
    const char* name;
    uint8_t code;
} Instruction;

Instruction instructions[] = {
    {"HALT", 0},
    {"PUSH", 1},
    {"DEL", 2},
    {"ADD", 3},
    {"SUB", 4},
    {"MULT", 5},
    {"DIV", 6},
    {"PRINT", 7},
    {"PEEK", 8},
    {"JMP", 9},
    {"JMP_IF_ZERO", 10},
    {"JMP_IF_NOT_ZERO", 11},
    {"EQ", 12},
    {"NOT_EQ", 13},
    {"LT", 14},
    {"GT", 15},
    {"LE", 16},
    {"GE", 17},
    {"DUP", 18},
    {"SWAP", 19},
    {"OVER", 20},
    {"NOP", 21},
    {"STORE", 22},
    {"LOAD", 23}
};


int get_opcode(const char* name) {
    for (int i = 0; i < (sizeof(instructions)/sizeof(instructions[0])); i++) {
        if (strcmp(instructions[i].name, name) == 0) {
            return instructions[i].code;
        }
    }
    return -1;
}


int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s input.asm output.obj\n", argv[0]);
        return 1;
    }

    const char* input_file = argv[1];
    const char* output_file = argv[2];

    FILE* file = fopen(input_file, "r");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    char line[128];
    char* token;

    while (fgets(line, sizeof(line), file)) {
        token = strtok(line, " \t\n");
        while (token != NULL) {
            int opcode = get_opcode(token);
            if (opcode != -1) {
                printf("Opcode: %d\n", opcode);
                bytecode[pc] = opcode;
            } else {
                printf("Value or unknown instruction: %s\n", token);
                bytecode[pc] = (uint8_t)atoi(token);
            }

            token = strtok(NULL, " \t\n");
            pc = pc + 1;
        }
    }

    fclose(file);

    
    FILE* out = fopen(output_file, "wb");
    int bytecode_length = sizeof(bytecode)/sizeof(bytecode[0]);
    fwrite(bytecode, 1, bytecode_length, out);
    fclose(out);

    uint8_t bytecode[] = {1, 4, 18, 7, 0};  // Example bytecode
    size_t length = sizeof(bytecode) / sizeof(bytecode[0]);
    return 0;    
}
