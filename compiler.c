#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_CODE 1024
#define MAX_LABELS 100
#define REGISTER_COUNT 3

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
    	{"CLEAR", 3},
    	{"ADD", 4},
    	{"SUB", 5},
    	{"MULT", 6},
    	{"DIV", 7},
    	{"PRINT", 8},
    	{"ASCII", 9},
    	{"PEEK", 10},
    	{"JMP", 11},
    	{"JMP_IF_ZERO", 12},
    	{"JMP_IF_NOT_ZERO", 13},
    	{"EQ", 14},
    	{"NOT_EQ", 15},
    	{"LT", 16},
    	{"GT", 17},
    	{"LE", 18},
    	{"GE", 19},
    	{"DUP", 20},
    	{"SWAP", 21},
    	{"OVER", 22},
    	{"NOP", 23},
    	{"STORE", 24},
	{"LOAD", 25}
};

Instruction registers[] = {
	{"R0", 0},
	{"R1", 1},
	{"R2", 2},
	{"R3", 3},
	{"R4", 4},
	{"R5", 5},
	{"R6", 6},
	{"R7", 7}
};

int get_opcode(const char* name) {
	for (int i = 0; i < (sizeof(instructions)/sizeof(instructions[0])); i++) {
        	if (strcmp(instructions[i].name, name) == 0) {
			return instructions[i].code;
        	}
    	}
	return -1;
}

int check_for_register(const char* name) {
	for (int i = 0; i < (sizeof(registers)/sizeof(registers[0])); i++) {
		if (strcmp(registers[i].name, name) == 0) {
			return registers[i].code;
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
			int regcode = check_for_register(token);
			if (opcode != -1) {
                		printf("Opcode: %d\n", opcode);
                		bytecode[pc] = opcode;
            		} else if (regcode != -1) {
				bytecode[pc] = regcode;
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
