#include "assembler.h"
#include "Label.h"
/*function that recived the name of the label the type and the number line of the label, and create new struct of label*/
struct labelStruct *createLabelStruct(char* name,int type, int dc) {
    /*allocating space to the new macro*/
    struct labelStruct *newNode = (struct labelStruct*)malloc(sizeof(struct labelStruct));
    if (newNode != NULL) {
        newNode->number_line_of_label = dc;
        newNode->label = (char*)malloc(strlen(name) + 1);
        strcpy(newNode->label, name);
        newNode->typeOfLabel = type;
        newNode->next = NULL;
    }
    return newNode;
}
/*The function recieved the head of the label list and the new list and insert it to the end of the list*/
void listOfLabelStruct(struct labelStruct** head, struct labelStruct* newMacro) {
    /*If the list is empty*/
    if (*head == NULL) {
        *head = newMacro;
    }
        /*Otherwise, it moves to the end of the list and adds the new macro at the end*/
    else {
        struct labelStruct* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newMacro;
    }
}
/*Function that check if the lavbel is exist in the list
 * If it already exists the function will release the structure */
int label_exist(struct labelStruct** head, struct labelStruct* newMacro) {
    struct labelStruct* current = *head;

    while (current != NULL) {
        if (strcmp(current->label, newMacro->label) == 0 && current->typeOfLabel!=isEntry && newMacro->typeOfLabel!=isEntry) {
            /*The label already exists in the list*/
            freeLabel(newMacro);
            return 1;
        }
        if (strcmp(current->label, newMacro->label) == 0 && current->typeOfLabel==isEntry && newMacro->typeOfLabel==isEntry) {
            freeLabel(newMacro);
            return 1;
        }
        current = current->next;
    }

    /* The label does not exist in the list*/
    return 0;
}
