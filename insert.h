#include "assembler.h"
#ifndef INSERT_H
#define INSERT_H
/*Fuction to insert the command and the operands to the list of ic the binary encode
  The function also checks the correctness of the parameters and commands */
int insertTheCommand(struct commands** head, struct MaybeALabel** headML, char* command, char* line, int numOfLine);
/*the functin insert command or operand to struct and insert the struct to the end of the list*/
void insertOpCode(struct commands** head, short opCode, char *commandName, int numOfLine);
/*The function insert the label lines to the operand*/
void insertLabelLines(struct commands** headCom, struct labelStruct** headLabel);
/*The function received the head of data list and the value to insert to the list and insert it to the end of the list*/
void insertData(struct data** head, short value);
/*update the data num of line after the first laps to IC+the number od the line in data*/
void updateDataNumLine(struct labelStruct** head, int numOfIC);
#endif
