#ifndef STRUCT_H
#define STRUCT_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LINE 80
#define MAX_COMMAND 5
#define MAX_LABEL 31
#define isData 0
#define isExtern 1
#define isCode 2
#define isEntry 3

/*struct of pointers to the lists of the structs*/
struct StructPointers {
    struct labelStruct *ptrTolabelStruct;/*pointer to the head of the label list*/
    struct commands *ptrToCommands;/*pointer to the head of the command list*/
    struct data *ptrToData;/*pointer to the head of the data list*/
    struct MaybeALabel *ptrToMaybeALabel;/*pointer to the head of the maybe label list*/
    int errorFlag;
};
/*struct for the labels*/
struct labelStruct{
    char *label;/*the name of the label*/
    short number_line_of_label;/*the number of the line of the label*/
    int typeOfLabel;/*the type of the label extern or entry*/
    struct labelStruct *next;/*pointer to the next struct*/
};
/*struct for the maybe label*/
struct MaybeALabel{
    char *label;/*the name of the label*/
    short numberOfLine;/*the number of the line of the label*/
    int typeOfLabel;/*the type of the label extern or entry*/
    struct MaybeALabel *next;/*pointer to the next struct*/
};
/*struct for the commands line*/
struct commands{
    char *commandName;/*the name of the command or the operand name*/
    int number_line_of_command;/*the number of the line of the command or the opperand*/
    short opCode;/*the opcode in binary of the command or the operand */
    struct commands *next;/*pointer to the next struct*/
};
/*struuct for the data lines*/
struct data{
    short value;/*the opcode in binary of the data value*/
    struct data* next;/*pointer to the next struct*/
};
/*The main function receives the name of the file and it will read all the lines from the after macro file and handle them as defined*/
int assembler(char* name_input_file);
/*check if the name is register if true return the number of the register else -1*/
int isRegister(char *nameRegister);
/*Function that recived the name of the command and compare the name to the list of command*/
int compareToCommand(char* com);
/*return the number of the oprerand that the opcode receive*/
int numOfOperand(char *name);
/*return the number of the opcode*/
int opCodeNum(char *name);



#endif
