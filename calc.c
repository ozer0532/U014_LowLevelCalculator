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

#define IS_EQUAL(regA, regB) ((regA.bit8 == regB.bit8) && (regA.bit7 == regB.bit7) && (regA.bit6 == regB.bit6) && (regA.bit5 == regB.bit5) && (regA.bit4 == regB.bit4) && (regA.bit3 == regB.bit3) && (regA.bit2 == regB.bit2) && (regA.bit1 == regB.bit))
#define LESS_THAN(regA, regB) ((regA.bit8 != regB.bit8) ? (regA.bit8 == 0) : ((regA.bit7 != regB.bit7) ? (regA.bit7 == 0) : ((regA.bit6 != regB.bit6) ? (regA.bit6 == 0) : ((regA.bit5 != regB.bit5) ? (regA.bit5 == 0) : ((regA.bit4 != regB.bit4) ? (regA.bit4 == 0) : ((regA.bit3 != regB.bit3) ? (regA.bit3 == 0) : ((regA.bit2 != regB.bit2) ? (regA.bit2 == 0) : ((regA.bit1 != regB.bit1) ? (regA.bit2 == 0) : 0))))))))

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
reg reg3;   // op/loop
reg reg4;   // keep

// Ops list
// + = 1
// - = 2
// * = 3
// / = 4
// % = 5
// ^ = 16
// r = 32

void debugPrint(reg regI) {
    printf("%d:%d:%d:%d:%d:%d:%d:%d\n", regI.bit8, regI.bit7, regI.bit6, regI.bit5, regI.bit4, regI.bit3, regI.bit2, regI.bit1);
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

#pragma region inc

void incCarry8() {
    reg3.bit7 = 0;
    if (reg3.bit8 == 1) {
        FILL(reg3);         // OVERFLOW
    } else {
        reg3.bit8 = 1;
    }
}

void incCarry7() {
    reg3.bit6 = 0;
    if (reg3.bit7 == 1) {
        incCarry8();
    } else {
        reg3.bit7 = 1;
    }
}

void incCarry6() {
    reg3.bit5 = 0;
    if (reg3.bit6 == 1) {
        incCarry7();
    } else {
        reg3.bit6 = 1;
    }
}

void incCarry5() {
    reg3.bit4 = 0;
    if (reg3.bit5 == 1) {
        incCarry6();
    } else {
        reg3.bit5 = 1;
    }
}

void incCarry4() {
    reg3.bit3 = 0;
    if (reg3.bit4 == 1) {
        incCarry5();
    } else {
        reg3.bit4 = 1;
    }
}

void incCarry3() {
    reg3.bit2 = 0;
    if (reg3.bit3 == 1) {
        incCarry4();
    } else {
        reg3.bit3 = 1;
    }
}

void incCarry2() {
    reg3.bit1 = 0;
    if (reg3.bit2 == 1) {
        incCarry3();
    } else {
        reg3.bit2 = 1;
    }
}

void inc() {
    if (reg3.bit1 == 1) {
        incCarry2();
    } else {
        reg3.bit1 = 1;
    }
}

#pragma endregion inc

#pragma region dec

void decTake8() {
    reg3.bit7 = 1;
    if (reg3.bit8 == 0) {
        CLEAR(reg2);                            // UNDERFLOW
    } else {
        reg3.bit8 = 0;
    }
}

void decTake7() {
    reg3.bit6 = 1;
    if (reg3.bit7 == 0) {
        decTake8();                             // TAKE
    } else {
        reg3.bit7 = 0;
    }
}

void decTake6() {
    reg3.bit5 = 1;
    if (reg3.bit6 == 0) {
        decTake7();                             // TAKE
    } else {
        reg3.bit6 = 0;
    }
}

void decTake5() {
    reg3.bit4 = 1;
    if (reg3.bit5 == 0) {
        decTake6();                             // TAKE
    } else {
        reg3.bit5 = 0;
    }
}

void decTake4() {
    reg3.bit3 = 1;
    if (reg3.bit4 == 0) {
        decTake5();                             // TAKE
    } else {
        reg3.bit4 = 0;
    }
}

void decTake3() {
    reg3.bit2 = 1;
    if (reg3.bit3 == 0) {
        decTake4();                         // TAKE
    } else {
        reg3.bit3 = 0;
    }
}

void decTake2() {
    reg3.bit1 = 1;
    if (reg3.bit2 == 0) {
        decTake3();                         // TAKE
    } else {
        reg3.bit2 = 0;
    }
}

void dec() {
    if (reg3.bit1 == 0) {
        decTake2();                             // TAKE
    } else {
        reg3.bit1 = 0;
    }
}

#pragma endregion

#pragma region add

void addCarry8() {
    reg2.bit7 = 0;
    if (reg2.bit8 == 1) {
        FILL(reg2);         // OVERFLOW
        FILL(reg1);
    } else {
        reg2.bit8 = 1;
    }
}

void addCarry7() {
    reg2.bit6 = 0;
    if (reg2.bit7 == 1) {
        addCarry8();
    } else {
        reg2.bit7 = 1;
    }
}

void addCarry6() {
    reg2.bit5 = 0;
    if (reg2.bit6 == 1) {
        addCarry7();
    } else {
        reg2.bit6 = 1;
    }
}

void addCarry5() {
    reg2.bit4 = 0;
    if (reg2.bit5 == 1) {
        addCarry6();
    } else {
        reg2.bit5 = 1;
    }
}

void addCarry4() {
    reg2.bit3 = 0;
    if (reg2.bit4 == 1) {
        addCarry5();
    } else {
        reg2.bit4 = 1;
    }
}

void addCarry3() {
    reg2.bit2 = 0;
    if (reg2.bit3 == 1) {
        addCarry4();
    } else {
        reg2.bit3 = 1;
    }
}

void addCarry2() {
    reg2.bit1 = 0;
    if (reg2.bit2 == 1) {
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
        addCarry8();                            // CARRY
    } else {
        reg2.bit7 = (reg2.bit7 == 1) ? 1 : reg1.bit7;
    }

    // Add bit 6
    if (reg1.bit6 == 1 && reg2.bit6 == 1) {
        addCarry7();                            // CARRY
    } else {
        reg2.bit6 = (reg2.bit6 == 1) ? 1 : reg1.bit6;
    }

    // Add bit 5
    if (reg1.bit5 == 1 && reg2.bit5 == 1) {
        addCarry6();                            // CARRY
    } else {
        reg2.bit5 = (reg2.bit5 == 1) ? 1 : reg1.bit5;
    }

    // Add bit 4
    if (reg1.bit4 == 1 && reg2.bit4 == 1) {
        addCarry5();                            // CARRY
    } else {
        reg2.bit4 = (reg2.bit4 == 1) ? 1 : reg1.bit4;
    }

    // Add bit 3
    if (reg1.bit3 == 1 && reg2.bit3 == 1) {
        addCarry4();                            // CARRY
    } else {
        reg2.bit3 = (reg2.bit3 == 1) ? 1 : reg1.bit3;
    }
    
    // Add bit 2
    if (reg1.bit2 == 1 && reg2.bit2 == 1) {
        addCarry3();                            // CARRY
    } else {
        reg2.bit2 = (reg2.bit2 == 1) ? 1 : reg1.bit2;
    }

    // Add bit 1
    if (reg1.bit1 == 1 && reg2.bit1 == 1) {
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
        CLEAR(reg1);
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

    // Subtract bit 3
    if (reg1.bit3 == 1 && reg2.bit3 == 0) {
        reg2.bit3 = 1;
        subTake4();                             // TAKE
    } else {
        reg2.bit3 = (reg1.bit3 == 1) ? 0 : reg2.bit3;
    }

    // Subtract bit 2
    if (reg1.bit2 == 1 && reg2.bit2 == 0) {
        reg2.bit2 = 1;
        subTake3();                             // TAKE
    } else {
        reg2.bit2 = (reg1.bit2 == 1) ? 0 : reg2.bit2;
    }

    // Subtract bit 1
    if (reg1.bit1 == 1 && reg2.bit1 == 0) {
        reg2.bit1 = 1;
        subTake2();                             // TAKE
    } else {
        reg2.bit1 = (reg1.bit1 == 1) ? 0 : reg2.bit1;
    }
}

#pragma endregion sub

#pragma region mul

void mul() {
    // Init loop counter
    COPY(reg2, reg3);
    CLEAR(reg2);
    if (IS(reg3, 0, 0, 0, 0, 0, 0, 0, 0)) {
        return;
    }
    mul_loop:
    add();
    dec(&reg3);
    if (!IS(reg3, 0, 0, 0, 0, 0, 0, 0, 0)) {
        goto mul_loop;
    }
}

#pragma endregion mul

#pragma region div

void dvd() {
    // Init loop counter
    CLEAR(reg3);
    div_loop:
    sub();
    if (!(IS(reg2, 0, 0, 0, 0, 0, 0, 0, 0) && IS(reg1, 0, 0, 0, 0, 0, 0, 0, 0))) {
        inc(&reg3);
        goto div_loop;
    }
    COPY(reg3, reg2);
}

#pragma endregion div

#pragma region mod

void mod() {
    // Init loop counter
    COPY(reg2, reg3);
    mod_loop:
    sub();
    if (!(IS(reg2, 0, 0, 0, 0, 0, 0, 0, 0) && IS(reg1, 0, 0, 0, 0, 0, 0, 0, 0))) {
        COPY(reg2, reg3);
        goto mod_loop;
    }
    COPY(reg3, reg2);
}

#pragma endregion mod

#pragma region pow

void powadd() {
    // Add bit 8
    if (reg2.bit8 == 1) {
        FILL(reg2);                             // OVERFLOW
    }

    // Add bit 7
    if (reg2.bit7 == 1) {
        addCarry8();                            // CARRY
    }

    // Add bit 6
    if (reg2.bit6 == 1) {
        addCarry7();                            // CARRY
    }

    // Add bit 5
    if (reg2.bit5 == 1) {
        addCarry6();                            // CARRY
    }

    // Add bit 4
    if (reg2.bit4 == 1) {
        addCarry5();                            // CARRY
    }

    // Add bit 3
    if (reg2.bit3 == 1) {
        addCarry4();                            // CARRY
    }
    
    // Add bit 2
    if (reg2.bit2 == 1) {
        addCarry3();                            // CARRY
    }

    // Add bit 1
    if (reg2.bit1 == 1) {
        addCarry2();                            // CARRY
    }
}

void powMul() {
    // Init loop counter
    // Copy reg1 to lower half of reg3 (number of adds)
    reg3.bit4 = reg3.bit8;
    reg3.bit3 = reg3.bit7;
    reg3.bit2 = reg3.bit6;
    reg3.bit1 = reg3.bit5;
    CLEAR(reg2);

    powmul_loop:
    add();
    dec(&reg3);
    if (!((reg3.bit4 == 0) && (reg3.bit3 == 0) && (reg3.bit2 == 0) && (reg3.bit1 == 0))) {
        goto powmul_loop;
    }
}

void pwr() {
    if (IS(reg1,0,0,0,0,0,0,0,0)) { // a ^ 0
        SET(reg2,0,0,0,0,0,0,0,1);
        return;
    }
    if (IS(reg1,0,0,0,0,0,0,0,1)) {
        return;                     // Reg2 as is (1 ^ n)
    }
    if (IS(reg2,0,0,0,0,0,0,0,0)) {
        return;                     // Reg2 is 0 (0 ^ n)
    }
    if ((reg1.bit8 == 1) || (reg1.bit7 == 1) || (reg1.bit6 == 1) || (reg1.bit5 == 1) || (reg1.bit4 == 1)) { // Guaranteed overflow (max a ^ 8 i.e. 2 ^ 8)
        FILL(reg2);
        return;
    }
    if ((reg2.bit8 == 1) || (reg2.bit7 == 1) || (reg2.bit6 == 1) || (reg2.bit5 == 1)) { // Guaranteed overflow (max 16 ^ n i.e. 16 ^ 2)
        FILL(reg2);
        return;
    }

    // Init pow loop
    // Copy lower half of reg1 to upper half of reg3 (a)
    reg3.bit8 = reg2.bit4;
    reg3.bit7 = reg2.bit3;
    reg3.bit6 = reg2.bit2;
    reg3.bit5 = reg2.bit1;

    // Jump to corresponding pow
    if (IS(reg1,0,0,0,0,0,1,1,1)) {
        goto pow7;
    }
    if (IS(reg1,0,0,0,0,0,1,1,0)) {
        goto pow6;
    }
    if (IS(reg1,0,0,0,0,0,1,0,1)) {
        goto pow5;
    }
    if (IS(reg1,0,0,0,0,0,1,0,0)) {
        goto pow4;
    }
    if (IS(reg1,0,0,0,0,0,0,1,1)) {
        goto pow3;
    }
    else {
        goto pow2;
    }

    // Pow 7
    pow7:
    COPY(reg2, reg1);
    powMul();

    pow6:
    // Pow 6
    COPY(reg2, reg1);
    powMul();

    pow5:
    // Pow 5
    COPY(reg2, reg1);
    powMul();

    pow4:
    // Pow 4
    COPY(reg2, reg1);
    powMul();

    pow3:
    // Pow 3
    COPY(reg2, reg1);
    powMul();

    pow2:
    // Pow 2
    COPY(reg2, reg1);
    powMul();
}

#pragma endregion pow

#pragma region sqt

void sqt() {
    SET(reg3,0,0,0,0,0,1,0,0);  // 4
    if (LESS_THAN(reg1, reg3)) {
        SET(reg1,0,0,0,0,0,0,0,1);      // 1
        return;
    }

    SET(reg3,0,0,0,0,1,0,0,1);  // 9
    if (LESS_THAN(reg1, reg3)) {
        SET(reg1,0,0,0,0,0,0,1,0);      // 2
        return;
    }

    SET(reg3,0,0,0,1,0,0,0,0);  // 16
    if (LESS_THAN(reg1, reg3)) {
        SET(reg1,0,0,0,0,0,0,1,1);      // 3
        return;
    }

    SET(reg3,0,0,0,1,1,0,0,1);  // 25
    if (LESS_THAN(reg1, reg3)) {
        SET(reg1,0,0,0,0,0,1,0,0);      // 4
        return;
    }

    SET(reg3,0,0,1,0,0,1,0,0);  // 36
    if (LESS_THAN(reg1, reg3)) {
        SET(reg1,0,0,0,0,0,1,0,1);      // 5
        return;
    }

    SET(reg3,0,0,1,1,0,0,0,1);  // 49
    if (LESS_THAN(reg1, reg3)) {
        SET(reg1,0,0,0,0,0,1,1,0);      // 6
        return;
    }

    SET(reg3,0,1,0,0,0,0,0,0);  // 64
    if (LESS_THAN(reg1, reg3)) {
        SET(reg1,0,0,0,0,0,1,1,1);      // 7
        return;
    }

    SET(reg3,0,1,0,1,0,0,0,1);  // 81
    if (LESS_THAN(reg1, reg3)) {
        SET(reg1,0,0,0,0,1,0,0,0);      // 8
        return;
    }

    SET(reg3,0,1,1,0,0,1,0,0);  // 100
    if (LESS_THAN(reg1, reg3)) {
        SET(reg1,0,0,0,0,1,0,0,1);      // 9
        return;
    }

    SET(reg3,0,1,1,1,1,0,0,1);  // 121
    if (LESS_THAN(reg1, reg3)) {
        SET(reg1,0,0,0,0,1,0,1,0);      // 10
        return;
    }

    SET(reg3,1,0,0,1,0,0,0,0);  // 144
    if (LESS_THAN(reg1, reg3)) {
        SET(reg1,0,0,0,0,1,0,1,1);      // 11
        return;
    }

    SET(reg3,1,0,1,0,1,0,0,1);  // 169
    if (LESS_THAN(reg1, reg3)) {
        SET(reg1,0,0,0,0,1,1,0,0);      // 12
        return;
    }

    SET(reg3,1,1,0,0,0,1,0,0);  // 196
    if (LESS_THAN(reg1, reg3)) {
        SET(reg1,0,0,0,0,1,1,0,1);      // 13
        return;
    }

    SET(reg3,1,1,1,0,0,0,0,1);  // 225
    if (LESS_THAN(reg1, reg3)) {
        SET(reg1,0,0,0,0,1,1,1,0);      // 14
        return;
    }

    SET(reg1,0,0,0,0,1,1,1,1);
}

#pragma endregion sqt

// Checks the operator and does the calculation
void resolveOperation() {
    if (IS(reg3,0,0,0,0,0,0,0,1)) {         // +
        add();
        CLEAR(reg3);
    } else if (IS(reg3,0,0,0,0,0,0,1,0)) {  // -
        sub();
        CLEAR(reg3);
    } else if (IS(reg3,0,0,0,0,0,0,1,1)) {  // *
        mul();
        CLEAR(reg3);
    } else if (IS(reg3,0,0,0,0,0,1,0,0)) {  // /
        dvd();
        CLEAR(reg3);
    } else if (IS(reg3,0,0,0,0,0,1,0,1)) {  // %
        mod();
        CLEAR(reg3);
    } else if (IS(reg3,0,0,0,1,0,0,0,0)) {  // ^
        pwr();
        CLEAR(reg3);
    } else if (IS(reg3,0,0,0,1,0,0,0,1)) {  // +^
        pwr();
        COPY(reg2, reg1);
        COPY(reg4, reg2);
        SET(reg3,0,0,0,0,0,0,0,1);
    } else if (IS(reg3,0,0,0,1,0,0,1,0)) {  // -^
        pwr();
        COPY(reg2, reg1);
        COPY(reg4, reg2);
        SET(reg3,0,0,0,0,0,0,1,0);
    } else if (IS(reg3,0,0,0,1,0,0,1,1)) {  // *^
        pwr();
        COPY(reg2, reg1);
        COPY(reg4, reg2);
        SET(reg3,0,0,0,0,0,0,1,1);
    } else if (IS(reg3,0,0,0,1,0,1,0,0)) {  // /^
        pwr();
        COPY(reg2, reg1);
        COPY(reg4, reg2);
        SET(reg3,0,0,0,0,0,1,0,0);
    } else if (IS(reg3,0,0,0,1,0,1,0,1)) {  // %^
        pwr();
        COPY(reg2, reg1);
        COPY(reg4, reg2);
        SET(reg3,0,0,0,0,0,1,0,1);
    } else if (IS(reg3,0,0,1,0,0,0,0,0)) {  // r
        sqt();
        COPY(reg1, reg2);
        CLEAR(reg3);
    } else if (IS(reg3,0,0,1,0,0,0,0,1)) {  // +r
        printf("test\n");
        sqt();
        SET(reg3,0,0,0,0,0,0,0,1);
        resolveOperation();
    } else if (IS(reg3,0,0,1,0,0,0,1,0)) {  // -r
        sqt();
        SET(reg3,0,0,0,0,0,0,1,0);
        resolveOperation();
    } else if (IS(reg3,0,0,1,0,0,0,1,1)) {  // *r
        sqt();
        SET(reg3,0,0,0,0,0,0,1,1);
        resolveOperation();
    } else if (IS(reg3,0,0,1,0,0,1,0,0)) {  // /r
        sqt();
        SET(reg3,0,0,0,0,0,1,0,0);
        resolveOperation();
    } else if (IS(reg3,0,0,1,0,0,1,0,1)) {  // %r
        sqt();
        SET(reg3,0,0,0,0,0,1,0,1);
        resolveOperation();
    } else if (IS(reg3,0,0,1,1,0,0,0,0)) {  // ^r
        sqt();
        SET(reg3,0,0,0,1,0,0,0,0);
        resolveOperation();
    } else if (IS(reg3,0,0,1,1,0,0,0,1)) {  // +^r
        sqt();
        SET(reg3,0,0,0,1,0,0,0,1);
        resolveOperation();
    } else if (IS(reg3,0,0,1,1,0,0,1,0)) {  // -^r
        sqt();
        SET(reg3,0,0,0,1,0,0,1,0);
        resolveOperation();
    } else if (IS(reg3,0,0,1,1,0,0,1,1)) {  // *^r
        sqt();
        SET(reg3,0,0,0,1,0,0,1,1);
        resolveOperation();
    } else if (IS(reg3,0,0,1,1,0,1,0,0)) {  // /^r
        sqt();
        SET(reg3,0,0,0,1,0,1,0,0);
        resolveOperation();
    } else if (IS(reg3,0,0,1,1,0,1,0,1)) {  // %^r
        sqt();
        SET(reg3,0,0,0,1,0,1,0,1);
        resolveOperation();
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

// Checks the operator and does the calculation
void powerResolveOperation() {
    if (IS(reg3,0,0,0,0,0,0,0,1)) {         // +
        add();
        CLEAR(reg3);
    } else if (IS(reg3,0,0,0,0,0,0,1,0)) {  // -
        sub();
        CLEAR(reg3);
    } else if (IS(reg3,0,0,0,0,0,0,1,1)) {  // *
        mul();
        CLEAR(reg3);
    } else if (IS(reg3,0,0,0,0,0,1,0,0)) {  // /
        dvd();
        CLEAR(reg3);
    } else if (IS(reg3,0,0,0,0,0,1,0,1)) {  // %
        mod();
        CLEAR(reg3);
    } else if (IS(reg3,0,0,0,1,0,0,0,0)) {  // ^
        pwr();
        CLEAR(reg3);
    } else if (IS(reg3,0,0,0,1,0,0,0,1)) {  // +^
        pwr();
        COPY(reg2, reg1);
        COPY(reg4, reg2);
        SET(reg3,0,0,0,0,0,0,0,1);
    } else if (IS(reg3,0,0,0,1,0,0,1,0)) {  // -^
        pwr();
        COPY(reg2, reg1);
        COPY(reg4, reg2);
        SET(reg3,0,0,0,0,0,0,1,0);
    } else if (IS(reg3,0,0,0,1,0,0,1,1)) {  // *^
        pwr();
        COPY(reg2, reg1);
        COPY(reg4, reg2);
        SET(reg3,0,0,0,0,0,0,1,1);
    } else if (IS(reg3,0,0,0,1,0,1,0,0)) {  // /^
        pwr();
        COPY(reg2, reg1);
        COPY(reg4, reg2);
        SET(reg3,0,0,0,0,0,1,0,0);
    } else if (IS(reg3,0,0,0,1,0,1,0,1)) {  // %^
        pwr();
        COPY(reg2, reg1);
        COPY(reg4, reg2);
        SET(reg3,0,0,0,0,0,1,0,1);
    } else if (IS(reg3,0,0,1,0,0,0,0,0)) {  // r
        sqt();
        COPY(reg1, reg2);
        CLEAR(reg3);
    } else if (IS(reg3,0,0,1,0,0,0,0,1)) {  // +r
        printf("test\n");
        sqt();
        SET(reg3,0,0,0,0,0,0,0,1);
    } else if (IS(reg3,0,0,1,0,0,0,1,0)) {  // -r
        sqt();
        SET(reg3,0,0,0,0,0,0,1,0);
    } else if (IS(reg3,0,0,1,0,0,0,1,1)) {  // *r
        sqt();
        SET(reg3,0,0,0,0,0,0,1,1);
    } else if (IS(reg3,0,0,1,0,0,1,0,0)) {  // /r
        sqt();
        SET(reg3,0,0,0,0,0,1,0,0);
    } else if (IS(reg3,0,0,1,0,0,1,0,1)) {  // %r
        sqt();
        SET(reg3,0,0,0,0,0,1,0,1);
    } else if (IS(reg3,0,0,1,1,0,0,0,0)) {  // ^r
        sqt();
        SET(reg3,0,0,0,1,0,0,0,0);
    } else if (IS(reg3,0,0,1,1,0,0,0,1)) {  // +^r
        sqt();
        SET(reg3,0,0,0,1,0,0,0,1);
    } else if (IS(reg3,0,0,1,1,0,0,1,0)) {  // -^r
        sqt();
        SET(reg3,0,0,0,1,0,0,1,0);
    } else if (IS(reg3,0,0,1,1,0,0,1,1)) {  // *^r
        sqt();
        SET(reg3,0,0,0,1,0,0,1,1);
    } else if (IS(reg3,0,0,1,1,0,1,0,0)) {  // /^r
        sqt();
        SET(reg3,0,0,0,1,0,1,0,0);
    } else if (IS(reg3,0,0,1,1,0,1,0,1)) {  // %^r
        sqt();
        SET(reg3,0,0,0,1,0,1,0,1);
    }
}

void powerResolveCheck() {
    if ((reg3.bit5 == 1) || (reg3.bit6 == 1)) {
        powerResolveOperation();
        COPY(reg2, reg4);
        COPY(reg1, reg2);
        CLEAR(reg1);
    } else {
        COPY(reg2, reg4);
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
            commonResolveCheck();
            SET(reg3, 0, 0, 0, 0, 0, 0, 1, 1);
        } else if (c == '/') {
            // DIVISION
            commonResolveCheck();
            SET(reg3, 0, 0, 0, 0, 0, 1, 0, 0);
        } else if (c == '%') {
            // MODULO
            commonResolveCheck();
            SET(reg3, 0, 0, 0, 0, 0, 1, 0, 1);
        } else if (c == '^') {
            // POWER
            powerResolveCheck();
            reg3.bit5 = 1;
        } else if (c == 'r') {
            // SQUARE ROOT
            reg3.bit6 = 1;
        } else if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            // WHITESPACE (IGNORE)
        } else {
            // NUMBER
            #pragma region input
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
            #pragma endregion
        }
	}
    if (NOT_EMPTY(reg3)) {
        resolveOperation();
        if (NOT_EMPTY(reg3)) {      // resolve supers
            resolveOperation();
        } else if (IS(reg2,0,0,0,0,0,0,0,0)) {
            COPY(reg1, reg2);
        }
    } else {
        COPY(reg1, reg2);
    }
    printReg2();
	
    return 0;
}