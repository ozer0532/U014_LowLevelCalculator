#include <stdio.h>
#include <stdlib.h>

#define AND_GATE(regA, bitA, regB, bitB) ((regA.bitA == 1 && regB.bitB == 1) ? 1 : 0)
#define OR_GATE(regA, bitA, regB, bitB) ((regA.bitA == 1 || regB.bitB == 1) ? 1 : 0)
#define XOR_GATE(regA, bitA, regB, bitB) ((regA.bitA != regB.bitB) ? 1 : 0)
#define NOT_GATE(regI, bitI) ((regI.bitI == 0) ? 1 : 0)

// Tests if regI is equal to the sequence of bits
#define IS(regI, b8, b7, b6, b5, b4, b3, b2, b1) ((regI.bit8 == b8) && (regI.bit7 == b7) && (regI.bit6 == b6) && (regI.bit5 == b5) && (regI.bit4 == b4) && (regI.bit3 == b3) && (regI.bit2 == b2) && (regI.bit1 == b1))
// Tests if regI is > 0
#define NOT_EMPTY(regI) ((regI.bit8 == 1) || (regI.bit7 == 1) || (regI.bit6 == 1) || (regI.bit5 == 1) || (regI.bit4 == 1) || (regI.bit3 == 1) || (regI.bit2 == 1) || (regI.bit1 == 1))
// Sets regI to the sequence of bits
#define SET(regI, b8, b7, b6, b5, b4, b3, b2, b1) regI.bit8 = b8; regI.bit7 = b7; regI.bit6 = b6; regI.bit5 = b5; regI.bit4 = b4; regI.bit3 = b3; regI.bit2 = b2; regI.bit1 = b1;
// Sets regI to 1
#define FILL(regI) regI.bit8 = 1; regI.bit7 = 1; regI.bit6 = 1; regI.bit5 = 1; regI.bit4 = 1; regI.bit3 = 1; regI.bit2 = 1; regI.bit1 = 1;
// Sets regI to 0
#define CLEAR(regI) regI.bit8 = 0; regI.bit7 = 0; regI.bit6 = 0; regI.bit5 = 0; regI.bit4 = 0; regI.bit3 = 0; regI.bit2 = 0; regI.bit1 = 0;
// Copy the register from regI to regO
#define COPY(regI, regO) regO.bit8 = regI.bit8; regO.bit7 = regI.bit7; regO.bit6 = regI.bit6; regO.bit5 = regI.bit5; regO.bit4 = regI.bit4; regO.bit3 = regI.bit3; regO.bit2 = regI.bit2; regO.bit1 = regI.bit1;

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

reg reg1;   // input
reg reg2;   // prev/out
reg reg3;   // op
reg reg4;   // keep

// Ops list
// + = 1
// - = 2
// * = 3
// / = 4
// % = 5
// ^ = 6
// r = 7

void debugPrint(reg regI) {
    printf("%d:%d:%d:%d:%d:%d:%d:%d\n", regI.bit8, regI.bit7, regI.bit6, regI.bit5, regI.bit4, regI.bit3, regI.bit2, regI.bit1);
}

// Reads numeric input into reg1
void parseInput(char c) {
    // -- Multiply reg1 by 10 (0b1010) --
    // Multiply by 10
    if (reg1.bit8 == 1) {
        FILL(reg1);
        return;
    }
    reg1.bit8 = reg1.bit7;
    reg1.bit7 = reg1.bit6;
    reg1.bit6 = reg1.bit5;
    reg1.bit5 = reg1.bit4;
    reg1.bit4 = reg1.bit3;
    reg1.bit3 = reg1.bit2;
    reg1.bit2 = reg1.bit1;
    reg1.bit1 = 0;

    // Multiply by 1000

    // Bit 8 & 7
    if (reg1.bit8 == 1) {
        FILL(reg1);
        return;
    }
    if (reg1.bit7 == 1) {
        FILL(reg1);
        return;
    }

    // Bit 6
    if (AND_GATE(reg1, bit6, reg1, bit8) == 1) {    // overflow
        FILL(reg1);
    } else {
        reg1.bit8 = (reg1.bit8 == 1) ? 1 : reg1.bit6;
    }

    // Bit 5
    if (AND_GATE(reg1, bit5, reg1, bit7) == 1) {    // carry to 8
        reg1.bit7 = 0;
        if (reg1.bit8 == 1) {                       // overflow
            FILL(reg1);
        } else {
            reg1.bit8 = 1;
        }
    } else {
        reg1.bit7 = (reg1.bit7 == 1) ? 1 : reg1.bit5;
    }

    // Bit 4
    if (AND_GATE(reg1, bit4, reg1, bit6) == 1) {    // carry to 7
        reg1.bit6 = 0;
        if (reg1.bit7 == 1) {                       // carry to 8
            reg1.bit7 = 0;
            if (reg1.bit8 == 1) {                   // overflow
                FILL(reg1);
            } else {
                reg1.bit8 = 1;
            }
        } else {
            reg1.bit7 = 1;
        }
    } else {
        reg1.bit6 = (reg1.bit6 == 1) ? 1 : reg1.bit4;
    }

    // Bit 3
    if (AND_GATE(reg1, bit3, reg1, bit5) == 1) {    // carry to 6
        reg1.bit5 = 0;
        if (reg1.bit6 == 1) {                       // carry to 7
            reg1.bit6 = 0;
            if (reg1.bit7 == 1) {                   // carry to 8
                reg1.bit7 = 0;
                if (reg1.bit8 == 1) {               // overflow
                    FILL(reg1);
                } else {
                    reg1.bit8 = 1;
                }
            } else {
                reg1.bit7 = 1;
            }
        } else {
            reg1.bit6 = 1;
        }
    } else {
        reg1.bit5 = (reg1.bit5 == 1) ? 1 : reg1.bit3;
    }

    // Bit 2
    if (AND_GATE(reg1, bit2, reg1, bit4) == 1) {    // carry to 5
        reg1.bit4 = 0;
        if (reg1.bit5 == 1) {                       // carry to 6
            reg1.bit5 = 0;
            if (reg1.bit6 == 1) {                   // carry to 7
                reg1.bit6 = 0;
                if (reg1.bit7 == 1) {               // carry to 8
                    reg1.bit7 = 0;
                    if (reg1.bit8 == 1) {           // overflow
                        FILL(reg1);
                    } else {
                        reg1.bit8 = 1;
                    }
                } else {
                    reg1.bit7 = 1;
                }
            } else {
                reg1.bit6 = 1;
            }
        } else {
            reg1.bit5 = 1;
        }
    } else {
        reg1.bit4 = (reg1.bit4 == 1) ? 1 : reg1.bit2;
    }

    // Bit 1 is 0 because of x10b

    // -- Add character value --
    // Bit 4
    if (((c << 28) >> 31) == -1) {
        if (reg1.bit4 == 1) {                           // carry to 5
            reg1.bit4 = 0;
            if (reg1.bit5 == 1) {                       // carry to 6
                reg1.bit5 = 0;
                if (reg1.bit6 == 1) {                   // carry to 7
                    reg1.bit6 = 0;
                    if (reg1.bit7 == 1) {               // carry to 8
                        reg1.bit7 = 0;
                        if (reg1.bit8 == 1) {           // overflow
                            FILL(reg1);
                        } else {
                            reg1.bit8 = 1;
                        }
                    } else {
                        reg1.bit7 = 1;
                    }
                } else {
                    reg1.bit6 = 1;
                }
            } else {
                reg1.bit5 = 1;
            }
        } else {
            reg1.bit4 = 1;
        }
    }

    // Bit 3
    if (((c << 29) >> 31) == -1) {
        if (reg1.bit3 == 1) {                           // carry to 4
            reg1.bit3 = 0;
            if (reg1.bit4 == 1) {                       // carry to 5
                reg1.bit4 = 0;
                if (reg1.bit5 == 1) {                   // carry to 6
                    reg1.bit5 = 0;
                    if (reg1.bit6 == 1) {               // carry to 7
                        reg1.bit6 = 0;
                        if (reg1.bit7 == 1) {           // carry to 8
                            reg1.bit7 = 0;
                            if (reg1.bit8 == 1) {       // overflow
                                FILL(reg1);
                            } else {
                                reg1.bit8 = 1;
                            }
                        } else {
                            reg1.bit7 = 1;
                        }
                    } else {
                        reg1.bit6 = 1;
                    }
                } else {
                    reg1.bit5 = 1;
                }
            } else {
                reg1.bit4 = 1;
            }
        } else {
            reg1.bit3 = 1;
        }
    }

    // Bit 2
    if (((c << 30) >> 31) == -1) {
        if (reg1.bit2 == 1) {                           // carry to 3
            reg1.bit2 = 0;
            if (reg1.bit3 == 1) {                       // carry to 4
                reg1.bit3 = 0;
                if (reg1.bit4 == 1) {                   // carry to 5
                    reg1.bit4 = 0;
                    if (reg1.bit5 == 1) {               // carry to 6
                        reg1.bit5 = 0;
                        if (reg1.bit6 == 1) {           // carry to 7
                            reg1.bit6 = 0;
                            if (reg1.bit7 == 1) {       // carry to 8
                                reg1.bit7 = 0;
                                if (reg1.bit8 == 1) {   // overflow
                                    FILL(reg1);
                                } else {
                                    reg1.bit8 = 1;
                                }
                            } else {
                                reg1.bit7 = 1;
                            }
                        } else {
                            reg1.bit6 = 1;
                        }
                    } else {
                        reg1.bit5 = 1;
                    }
                } else {
                    reg1.bit4 = 1;
                }
            } else {
                reg1.bit3 = 1;
            }
        } else {
            reg1.bit2 = 1;
        }
    }
    
    // Bit 1
    if (((c << 31) >> 31) == -1) {
        if (reg1.bit1 == 1) {                               // carry to 2
            reg1.bit1 = 0;
            if (reg1.bit2 == 1) {                           // carry to 3
                reg1.bit2 = 0;
                if (reg1.bit3 == 1) {                       // carry to 4
                    reg1.bit3 = 0;
                    if (reg1.bit4 == 1) {                   // carry to 5
                        reg1.bit4 = 0;
                        if (reg1.bit5 == 1) {               // carry to 6
                            reg1.bit5 = 0;
                            if (reg1.bit6 == 1) {           // carry to 7
                                reg1.bit6 = 0;
                                if (reg1.bit7 == 1) {       // carry to 8
                                    reg1.bit7 = 0;
                                    if (reg1.bit8 == 1) {   // overflow
                                        FILL(reg1);
                                    } else {
                                        reg1.bit8 = 1;
                                    }
                                } else {
                                    reg1.bit7 = 1;
                                }
                            } else {
                                reg1.bit6 = 1;
                            }
                        } else {
                            reg1.bit5 = 1;
                        }
                    } else {
                        reg1.bit4 = 1;
                    }
                } else {
                    reg1.bit3 = 1;
                }
            } else {
                reg1.bit2 = 1;
            }
        } else {
            reg1.bit1 = 1;
        }
    }
}

// Prints register 2 as numbers
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

#pragma region add

// Addition functions
void addCarry8() {
    if (reg2.bit8 == 1) {
        FILL(reg2);         // OVERFLOW
    } else {
        reg2.bit8 = 1;
    }
}

void addCarry7() {
    if (reg2.bit7 == 1) {
        reg2.bit7 = 0;
        addCarry8();
    } else {
        reg2.bit7 = 1;
    }
}

void addCarry6() {
    if (reg2.bit6 == 1) {
        reg2.bit6 = 0;
        addCarry7();
    } else {
        reg2.bit6 = 1;
    }
}

void addCarry5() {
    if (reg2.bit5 == 1) {
        reg2.bit5 = 0;
        addCarry6();
    } else {
        reg2.bit5 = 1;
    }
}

void addCarry4() {
    if (reg2.bit4 == 1) {
        reg2.bit4 = 0;
        addCarry5();
    } else {
        reg2.bit4 = 1;
    }
}

void addCarry3() {
    if (reg2.bit3 == 1) {
        reg2.bit3 = 0;
        addCarry4();
    } else {
        reg2.bit3 = 1;
    }
}

void addCarry2() {
    if (reg2.bit2 == 1) {
        reg2.bit2 = 0;
        addCarry3();
    } else {
        reg2.bit2 = 1;
    }
}

void add() {
    // Add bit 8
    if (reg1.bit8 == 1 && reg2.bit8 == 1) {
        FILL(reg2);                             // OVERFLOW
    } else {
        reg2.bit8 = (reg2.bit8 == 1) ? 1 : reg1.bit8;
    }

    // Add bit 7
    if (reg1.bit7 == 1 && reg2.bit7 == 1) {
        reg2.bit7 = 0;
        addCarry8();                            // CARRY
    } else {
        reg2.bit7 = (reg2.bit7 == 1) ? 1 : reg1.bit7;
    }

    // Add bit 6
    if (reg1.bit6 == 1 && reg2.bit6 == 1) {
        reg2.bit6 = 0;
        addCarry7();                            // CARRY
    } else {
        reg2.bit6 = (reg2.bit6 == 1) ? 1 : reg1.bit6;
    }

    // Add bit 5
    if (reg1.bit5 == 1 && reg2.bit5 == 1) {
        reg2.bit5 = 0;
        addCarry6();                            // CARRY
    } else {
        reg2.bit5 = (reg2.bit5 == 1) ? 1 : reg1.bit5;
    }

    // Add bit 4
    if (reg1.bit4 == 1 && reg2.bit4 == 1) {
        reg2.bit4 = 0;
        addCarry5();                            // CARRY
    } else {
        reg2.bit4 = (reg2.bit4 == 1) ? 1 : reg1.bit4;
    }

    // Add bit 3
    if (reg1.bit3 == 1 && reg2.bit3 == 1) {
        reg2.bit3 = 0;
        addCarry4();                            // CARRY
    } else {
        reg2.bit3 = (reg2.bit3 == 1) ? 1 : reg1.bit3;
    }
    
    // Add bit 2
    if (reg1.bit2 == 1 && reg2.bit2 == 1) {
        reg2.bit2 = 0;
        addCarry3();                            // CARRY
    } else {
        reg2.bit2 = (reg2.bit2 == 1) ? 1 : reg1.bit2;
    }

    // Add bit 1
    if (reg1.bit1 == 1 && reg2.bit1 == 1) {
        reg2.bit1 = 0;
        addCarry2();                            // CARRY
    } else {
        reg2.bit1 = (reg2.bit1 == 1) ? 1 : reg1.bit1;
    }
}

#pragma endregion add

#pragma region sub

void subTake8() {
    if (reg2.bit8 == 0) {
        CLEAR(reg2);                            // UNDERFLOW
    } else {
        reg2.bit8 = 0;
    }
}

void subTake7() {
    if (reg2.bit7 == 0) {
        reg2.bit7 = 1;
        subTake8();                             // TAKE
    } else {
        reg2.bit7 = 0;
    }
}

void subTake6() {
    if (reg2.bit6 == 0) {
        reg2.bit6 = 1;
        subTake7();                             // TAKE
    } else {
        reg2.bit6 = 0;
    }
}

void subTake5() {
    if (reg2.bit5 == 0) {
        reg2.bit5 = 1;
        subTake6();                             // TAKE
    } else {
        reg2.bit5 = 0;
    }
}

void subTake4() {
    if (reg2.bit4 == 0) {
        reg2.bit4 = 1;
        subTake5();                             // TAKE
    } else {
        reg2.bit4 = 0;
    }
}

void subTake3() {
    if (reg2.bit3 == 0) {
        reg2.bit3 = 1;
        subTake4(reg2);                         // TAKE
    } else {
        reg2.bit3 = 0;
    }
}

void subTake2() {
    if (reg2.bit2 == 0) {
        reg2.bit2 = 1;
        subTake3(reg2);                         // TAKE
    } else {
        reg2.bit2 = 0;
    }
}

void sub() {
    // Subtract bit 8
    if (reg1.bit8 == 1 && reg2.bit8 == 0) {
        CLEAR(reg2);                             // UNDERFLOW
    } else {
        reg2.bit8 = (reg1.bit8 == 1) ? 0 : reg2.bit8;
    }

    // Subtract bit 7
    if (reg1.bit7 == 1 && reg2.bit7 == 0) {
        reg2.bit7 = 1;
        subTake8();                             // TAKE
    } else {
        reg2.bit7 = (reg1.bit7 == 1) ? 0 : reg2.bit7;
    }
    
    // Subtract bit 6
    if (reg1.bit6 == 1 && reg2.bit6 == 0) {
        reg2.bit6 = 1;
        subTake7();                             // TAKE
    } else {
        reg2.bit6 = (reg1.bit6 == 1) ? 0 : reg2.bit6;
    }
    
    // Subtract bit 5
    if (reg1.bit5 == 1 && reg2.bit5 == 0) {
        reg2.bit5 = 1;
        subTake6();                             // TAKE
    } else {
        reg2.bit5 = (reg1.bit5 == 1) ? 0 : reg2.bit5;
    }

    // Subtract bit 4
    if (reg1.bit4 == 1 && reg2.bit4 == 0) {
        reg2.bit4 = 1;
        subTake5();                             // TAKE
    } else {
        reg2.bit4 = (reg1.bit4 == 1) ? 0 : reg2.bit4;
    }

    debugPrint(reg2);

    // Subtract bit 3
    if (reg1.bit3 == 1 && reg2.bit3 == 0) {
        reg2.bit3 = 1;
        subTake4();                             // TAKE
    } else {
        reg2.bit3 = (reg1.bit3 == 1) ? 0 : reg2.bit3;
    }

    debugPrint(reg2);

    // Subtract bit 2
    if (reg1.bit2 == 1 && reg2.bit2 == 0) {
        reg2.bit2 = 1;
        subTake3();                             // TAKE
    } else {
        reg2.bit2 = (reg1.bit2 == 1) ? 0 : reg2.bit2;
    }

    debugPrint(reg2);

    // Subtract bit 1
    if (reg1.bit1 == 1 && reg2.bit1 == 0) {
        reg2.bit1 = 1;
        subTake2();                             // TAKE
    } else {
        reg2.bit1 = (reg1.bit1 == 1) ? 0 : reg2.bit1;
    }

    debugPrint(reg2);
}

#pragma endregion sub

// Checks the operator and does the calculation
void resolveOperation() {
    if (IS(reg3,0,0,0,0,0,0,0,1)) {         // +
        add();
        CLEAR(reg3);
    } else if (IS(reg3,0,0,0,0,0,0,1,0)) {  // -
        sub();
        CLEAR(reg3);
    }
}

void commonResolveCheck() {
    if (NOT_EMPTY(reg3)) {
        resolveOperation();
        CLEAR(reg1);
    } else {
        COPY(reg1, reg2);
        CLEAR(reg1);
    }
}

int main(int argc, char* argv[]){
	
	FILE *file = fopen(argv[1], "r");
	char c;

	if (file == NULL) {
		printf("File not found!\n");
		exit(1);
	}

	while ((c = fgetc(file)) != EOF) {
        if (c == '+') {
            // ADDITION
            // Move input to prev
            commonResolveCheck();
            SET(reg3, 0, 0, 0, 0, 0, 0, 0, 1);
        } else if (c == '-') {
            // SUBTRACTION
            commonResolveCheck();
            SET(reg3, 0, 0, 0, 0, 0, 0, 1, 0);
        } else if (c == '*') {
            // MULTIPLICATION
        } else if (c == '/') {
            // DIVISION
        } else if (c == '%') {
            // MODULO
        } else if (c == '^') {
            // POWER
        } else if (c == 'r') {
            // SQUARE ROOT
        } else if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            // WHITESPACE (IGNORE)
        } else {
            // NUMBER
            parseInput(c);
        }
	}
    if (NOT_EMPTY(reg3)) {
        resolveOperation();
    } else {
        COPY(reg1, reg2);
    }
    printReg2();
	
    return 0;
}