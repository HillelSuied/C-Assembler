
#include <stdio.h>
/*insert the binary code to the number*/
short CommandBinary(int number) {
    number = number << 5;
    return (short)number;
}

/*insert the target operand*/
short targetOperandBinary(short opcode, short num)
{
    num= num<<2;
    return (opcode|num);
}

/*insert register to the binary number*/
short targetRegisterBinary(short target, short num)
{
    num= num<<2;
    return (target|num);
}
/*if there is two registers the secound insert to same number*/
short sourceRegisterBinary(short opcode, short num)
{
    num= num<<7;
    return (opcode|num);
}
/*Binary encodes the source's place on the command line*/
short sourceCommandBinary(short opcode, short num)
{
    num= num<<9;
    return (opcode|num);
}
/*Binary encodes the oparend*/
short numberOperandBinary(short target, short num)
{
    num= num<<2;
    return (target|num);
}
/*Binary encodes the extern labels*/
short binaryExtern(short opCode)
{
    short num=1;
    return (opCode|num);
}
/*Binary encodes the entery labels*/
short binaryEntry(short opCode)
{
    short num=2;
    return (opCode|num);
}
/*Binary encodes the number of line of the label*/
short binaryLine(short opCode, short numOfLine)
{
    numOfLine= numOfLine<<2;
    return (opCode|numOfLine);
}



