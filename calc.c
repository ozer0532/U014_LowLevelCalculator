#include <stdio.h>
#include <stdlib.h>

#define AND_GATE(regA, bitA, regB, bitB) ((regA.bitA == 1 && regB.bitB == 1) ? 1 : 0)
#define OR_GATE(regA, bitA, regB, bitB) ((regA.bitA == 1 || regB.bitB == 1) ? 1 : 0)
#define XOR_GATE(regA, bitA, regB, bitB) ((regA.bitA != regB.bitB) ? 1 : 0)
#define NOT_GATE(regI, bitI) ((regI.bitI == 0) ? 1 : 0)

#define IS(regI, bit8, bit7, bit6, bit5, bit4, bit3, bit2, bit1) ((regI.bit8 == bit8) && (regI.bit7 == bit7) && (regI.bit6 == bit6) && (regI.bit5 == bit5) && (regI.bit4 == bit4) && (regI.bit3 == bit3) && (regI.bit2 == bit2) && (regI.bit1 == bit1))
#define NOT_EMPTY(regI) ((regI.bit8 == 1) || (regI.bit7 == 1) || (regI.bit6 == 1) || (regI.bit5 == 1) || (regI.bit4 == 1) || (regI.bit3 == 1) || (regI.bit2 == 1) || (regI.bit1 == 1))
#define FILL(regI) regI.bit8 = 1; regI.bit7 = 1; regI.bit6 = 1; regI.bit5 = 1; regI.bit4 = 1; regI.bit3 = 1; regI.bit2 = 1; regI.bit1 = 1;
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

reg reg1;
reg reg2;
reg reg3;
reg reg4;

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

int main(int argc, char* argv[]){
	
	FILE *file = fopen(argv[1], "r");
	char c;

	if (file == NULL) {
		printf("File not found!\n");
		exit(1);
	}

	while ((c = fgetc(file)) != EOF) {
        parseInput(c);
        COPY(reg1, reg2);
	}
    printReg2();
	
    return 0;
}