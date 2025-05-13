#include "assembler.h"
#ifndef LABEL_H
#define LABEL_H
/*The function recieved the head of the label list and the new list and insert it to the end of the list*/
void listOfLabelStruct(struct labelStruct** head, struct labelStruct* newMacro);
/*function that recived the name of the label the type and the number line of the label, and create new struct of label*/
struct labelStruct* createLabelStruct(char* name,int type, int dc);
/*Function that check if the lavbel is exist in the list
 * If it already exists the function will release the structure */
int label_exist(struct labelStruct** head, struct labelStruct* newMacro);

#endif
