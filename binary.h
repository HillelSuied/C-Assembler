
#ifndef BINARY_H
#define BINARY_H

#include <stdio.h>
/*insert the binary code to the number*/
short CommandBinary(int number);
/*insert the target operand*/
short targetOperandBinary(short opcode, short num);
/*insert register to the binary number*/
short targetRegisterBinary(short target, short num);
/*if there is two registers the secound insert to same number*/
short sourceRegisterBinary(short opcode, short num);
/*Binary encodes the source's place on the command line*/
short sourceCommandBinary(short opcode, short num);
/*Binary encodes the oparend*/
short numberOperandBinary(short target, short num);
/*Binary encodes the extern labels*/
short binaryExtern(short opCode);
/*Binary encodes the entery labels*/
short binaryEntry(short opCode);
/*Binary encodes the number of line of the label*/
short binaryLine(short opCode, short numOfLine);


#endif

