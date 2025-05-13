#include "assembler.h"

#ifndef PRINTTOFILE_H
#define PRINTTOFILE_H
/*Function to print to the object file, which contains the machine code*/
int printCmdAndExtToObj(char *fileName, struct commands* head, int IC ,int DC);
/*The function print to ob file all the data in the code*/
int printDataToObject(char *fileName, struct data* head);
/*The function print to ent file all the entry label in the code*/
int printLabelToFile(char *fileName, struct labelStruct* head);
/*check if the are is extern*/
int AREextern(short num);
/*Gets the list of labels and checks if each label that was declared as an entry was really defined at some point in the file*/
int checkIfEntDef(struct labelStruct* head);
#endif
