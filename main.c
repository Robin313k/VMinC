#include <stdio.h>
#include <stdint.h>

// Define the instruction set
typedef enum {
	OP_HALT = 0,
    	OP_PUSH,     		// Push a constant onto the stack
    	OP_DEL,	 			// Delete latest constant of the stack
    	OP_ADD,      		// Pop two values, add them, push result
    	OP_SUB,	 			// Pop two calues, subtract one from the other, push result
    	OP_MULT, 	 		// Pop two values, multiply them with each other, push result
    	OP_DIV,	 			// Pop two values, divide one through the other, push result
    	OP_PRINT,    		// Print top of stack and consume it
    	OP_PEEK,	 		// Print top of stack without consuming it
    	OP_JMP,	 			// Jump to the instruction matching the given index
    	OP_JMP_IF_ZERO,		// Jump if top of stack does equal zero
    	OP_JMP_IF_NOT_ZERO,	// Jump if top of stack does not equal zero
    	OP_EQ,				// Pop top two values, push 1 if equal, push 0 if not
    	OP_NOT_EQ,			// Pop top two values, push 1 if not equal, push 0 if equal
    	OP_LT,    			// Pop top two values, push 1 if less than, else push 0
    	OP_GT,    			// Pop top two values, push 1 if greater than, else push 0
    	OP_LE,    			// Pop top two values, push 1 if greater than or equal, else push 0
    	OP_GE,    			// Pop top two values, push 1 if greater than or equal, else push 0
		OP_DUP,				// Pop top value and push it twice
		OP_SWAP,			// Swap top 2 values
		OP_OVER,			// Copy second element on top 4, 8 -> 4, 8, 4
		OP_NOP,				// Empty instruction

		OP_STORE_R,			//Pop top value and store in given register address
		OP_LOAD_R			//Pop value of given register address and load it onto the stack
} OpCode;

// Stack setup
#define STACK_SIZE 256
int stack[STACK_SIZE];
int sp = -1; // Stack pointer

// Register setup
#define REGISTER_SIZE 8
uint16_t reg[REGISTER_SIZE]; // R0-R7

// Stack operations
void push(int value) {
    	if (sp >= STACK_SIZE - 1) {
        	printf("Stack overflow!\n");
        	return;
    	}
    	stack[++sp] = value;
}

int pop() {
    	if (sp < 0) {
        	printf("Stack underflow!\n");
        	return 0;
    	}
    	return stack[sp--];
}

int peek() {
    	if (sp < 0) {
        	printf("Stack underflow on peek!\n");
        	return 0;
    	}
    	return stack[sp];
}

// The VM execution function
void run(uint8_t *program, size_t length) {
    	int pc = 0; // Program counter

    	while (1) {
        	OpCode op = program[pc++];  // Fetch

        	switch (op) {
            	case OP_HALT: {
            		return;
				}

            	case OP_PUSH: {
            		push(program[pc++]);
            		break;
				}

	    		case OP_DEL: {
	    			int a = pop();
				break;
	    		}

            	case OP_ADD: {
            		int b = pop();
            		int a = pop();
            		push(a + b);
            		break;
        		}

	    		case OP_SUB: {
					int b = pop();
					int a = pop();
					push(a - b);
					break;		
	    		}

	    		case OP_MULT: {
					int b = pop();
					int a = pop();
					push(a * b);
					break;		
	    		}

	    		case OP_DIV: {
					int b = pop();
					int a = pop();
					if (b == 0) {
						printf("You can NOT divide by 0.\nThis VM will shut down as means of a security measure!\n");
						return;
					}

					push(a / b);		
					break;
	    		}

            	case OP_PRINT: {
                	printf("%d\n", pop());
                	break;
				}

	    		case OP_PEEK: {
				printf("%d\n", peek());
				break;
				}

	    		case OP_JMP: {
           			if (pc >= length) {
        				printf("Error: JMP missing target address\n");
        				return;
    				}

    				pc = program[pc];  // Jump to absolute position
    				break;
				}

	    		case OP_JMP_IF_ZERO: {
           			if (pc >= length) {
        				printf("Error: JMP missing target address\n");
        				return;
    				}

					uint8_t target = program[pc++];
					int condition = pop();

					if (condition == 0) {
    					pc = target;
					}

					break;
				}

	    		case OP_JMP_IF_NOT_ZERO: {
           			if (pc >= length) {
        				printf("Error: JMP missing target address\n");
        				return;
    				}

				uint8_t target = program[pc++];
				int condition = pop();

					if (condition != 0) {
    					pc = target;
					}

					break;
				}

				case OP_EQ: {
					int b = pop();	    
					int a = pop();	    
					if (a == b) {
						push(1);
					} else {
						push(0);
					}

				 	break;
				}

			case OP_NOT_EQ: {
				int b = pop();	    
				int a = pop();	    
				if (a != b) {
					push(1);
				} else {
					push(0);
				}

				 break;
			}

			case OP_LT: {
				int b = pop();	    
				int a = pop();	    
				if (a < b) {
					push(1);
				} else {
					push(0);
				}

				 break;	    
			}

			case OP_GT: {
				int b = pop();	    
				int a = pop();	    
				if (a > b) {
					push(1);
				} else {
					push(0);
				}

				 break;	    
			}

			case OP_LE: {
				int b = pop();	    
				int a = pop();	    
				if (a <= b) {
					push(1);
				} else {
					push(0);
				}

				 break;	    
			}

			case OP_GE: {
				int b = pop();	    
				int a = pop();	    
				if (a >= b) {
					push(1);
				} else {
					push(0);
				}

				 break;	    
			}

			case OP_DUP: {
				int a = pop();
		   		push(a);		
		   		push(a);		
				break;
			}

			case OP_SWAP: {
				int b = pop();	      
				int a = pop();	      
				push(b);
				push(a);
				break;
			}

			case OP_OVER: {
				int b = pop();	      
				int a = pop();	      
				push(a);
				push(b);
				push(a);
				break;
			}

			case OP_NOP: { 
				//does nothing, pc just advances

				break;
			}

				case OP_STORE_R: {
					reg[program[pc++]] = pop();
					break;
				}

				case OP_LOAD_R: {
					push(reg[program[pc++]]);
					break;
				}

			default: {
                		printf("Unknown instruction: %d\n", op);
                		return;
			}
       		}
	}
}

// Main function
int main() {
    	// Example bytecode: push 2, push 3, add, print, halt
    	uint8_t program[] = {
			OP_PUSH, 8,
			OP_DUP,
			OP_DUP,
			OP_MULT,
			OP_PRINT,
			OP_DUP,
			OP_PUSH, 1,
			OP_SUB,
			OP_DUP,
			OP_JMP_IF_NOT_ZERO, 2,
			OP_HALT
	};


    run(program, sizeof(program));
	return 0;
}