#include <stdio.h>
#include <stdlib.h>

#define AND_GATE(regA, bitA, regB, bitB) ((regA.bitA == 1 && regB.bitB == 1) ? 1 : 0)
#define OR_GATE(regA, bitA, regB, bitB) ((regA.bitA == 1 || regB.bitB == 1) ? 1 : 0)
#define XOR_GATE(regA, bitA, regB, bitB) ((regA.bitA != regB.bitB) ? 1 : 0)
#define NOT_GATE(regI, bitI) ((regI.bitI == 0) ? 1 : 0)

// assume bit ordering 87654321
typedef struct {
	unsigned int bit1 : 1;
	unsigned int bit2 : 1;
	unsigned int bit3 : 1;
	unsigned int bit4 : 1;
	unsigned int bit5 : 1;
	unsigned int bit6 : 1;
	unsigned int bit7 : 1;
	unsigned int bit8 : 1;
} reg;

reg reg1;
reg reg2;
reg reg3;
reg reg4;

int main(int argc, char* argv[]){
	
	FILE *file = fopen(argv[1], "r");
	char c;

	if (file == NULL) {
		printf("File not found!\n");
		exit(1);
	}

	while ((c = fgetc(file)) != EOF) {

	}
	
    return 0;
}

// Reads input into reg1
void parseInput(char c) {
    
}

void printReg2() {
    int output = 0;
    if (reg2.bit8 == 1) {
        output |= 0b10000000;
    }
    if (reg2.bit7 == 1) {
        output |= 0b01000000;
    }
    if (reg2.bit6 == 1) {
        output |= 0b00100000;
    }
    if (reg2.bit5 == 1) {
        output |= 0b00010000;
    }
    if (reg2.bit4 == 1) {
        output |= 0b00001000;
    }
    if (reg2.bit3 == 1) {
        output |= 0b00000100;
    }
    if (reg2.bit2 == 1) {
        output |= 0b00000010;
    }
    if (reg2.bit1 == 1) {
        output |= 0b00000001;
    }

    printf("%d", output);
}

void clearReg1() {
    reg1.bit8 = 0;
    reg1.bit7 = 0;
    reg1.bit6 = 0;
    reg1.bit5 = 0;
    reg1.bit4 = 0;
    reg1.bit3 = 0;
    reg1.bit2 = 0;
    reg1.bit1 = 0;
}

void clearReg2() {
    reg2.bit8 = 0;
    reg2.bit7 = 0;
    reg2.bit6 = 0;
    reg2.bit5 = 0;
    reg2.bit4 = 0;
    reg2.bit3 = 0;
    reg2.bit2 = 0;
    reg2.bit1 = 0;
}

void clearReg3() {
    reg3.bit8 = 0;
    reg3.bit7 = 0;
    reg3.bit6 = 0;
    reg3.bit5 = 0;
    reg3.bit4 = 0;
    reg3.bit3 = 0;
    reg3.bit2 = 0;
    reg3.bit1 = 0;
}

void clearReg4() {
    reg4.bit8 = 0;
    reg4.bit7 = 0;
    reg4.bit6 = 0;
    reg4.bit5 = 0;
    reg4.bit4 = 0;
    reg4.bit3 = 0;
    reg4.bit2 = 0;
    reg4.bit1 = 0;
}