#include "assembler.h"

#ifndef MAYBELABEL_H
#define MAYBELABEL_H
/*Create new struct for maybe label*/
struct MaybeALabel *createMaybeLabel(char* name,int numberOfLine);
/*Gets a maybeLabel structure and a list and checks if it doesn't already exist in the list
 * If it already exists the function will release the structure*/
int MaybeLabelDontExist(struct MaybeALabel* head, struct MaybeALabel* newNode);
/*Gets a structure of maybeLabel and a list and inserts the structure at the end of the list*/
void insertMaybeALabel(struct MaybeALabel** head, struct MaybeALabel* newNode);
/*Gets the maybeLabel list and the label list and checks if each maybeLabel has really been declared as a label*/
int checkIfItWasLabel(struct labelStruct* headL, struct MaybeALabel* headML);
/*Free the label that is in the maybe list*/
void freeMaybeALabel(struct MaybeALabel* head);

#endif
