#include "maybeLabel.h"
#include "assembler.h"
/*Create new struct for maybe label*/
struct MaybeALabel *createMaybeLabel(char* name,int numberOfLine) {
    /*Allocating space to the new label struct*/
    struct MaybeALabel *newNode = (struct MaybeALabel*)malloc(sizeof(struct MaybeALabel));
    if (newNode != NULL)/*check if the struct not null*/ {
        /*insert the data of the label to the struct*/
        newNode->numberOfLine = numberOfLine;
        newNode->label = (char*)malloc(strlen(name) + 1);/*Allocating space to the name of the label*/
        strcpy(newNode->label, name);
        newNode->next = NULL;
    }
    return newNode;
}

/*Gets a maybeLabel structure and a list and checks if it doesn't already exist in the list
 * If it already exists the function will release the structure*/
int MaybeLabelDontExist(struct MaybeALabel* head, struct MaybeALabel* newNode) {
    struct MaybeALabel* current = head;
    while (current != NULL) {
        if (strcmp(current->label, newNode->label) == 0 ) {
            /*The label already exists in the list*/
            freeMaybeALabel(newNode);
            return 0;
        }

        current = current->next;
    }
    /* The label does not exist in the list*/
    return 1;
}

/*Gets a structure of maybeLabel and a list and inserts the structure at the end of the list*/
void insertMaybeALabel(struct MaybeALabel** head, struct MaybeALabel* newNode) {
    struct MaybeALabel* current = *head;
    /*If the list is empty*/
    if (*head == NULL) {
        *head = newNode;
    }
        /*Otherwise, it moves to the end of the list and adds the new macro at the end*/
    else {
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

/*Gets the maybeLabel list and the label list and checks if each maybeLabel has really been declared as a label*/
int checkIfItWasLabel(struct labelStruct* headL, struct MaybeALabel* headML){
    struct MaybeALabel* currentML = headML;
    struct labelStruct* currentL = headL;
    int isDefined=0, errFlag=0;
    /*Run on all the list of maybe label*/
    while(currentML!=NULL){
        while(currentL!=NULL){
            if(strcmp(currentML->label,currentL->label)==0 && currentL->typeOfLabel!=isEntry){
                isDefined=1;
            }
            currentL=currentL->next;
        }
        if(isDefined==0){
            printf("Error in line %d The parameter:%s is invalid and it's not defined like a label\n",currentML->numberOfLine,currentML->label);
            errFlag++;
        }
        isDefined=0;
        currentL = headL;
        currentML=currentML->next;
    }
    return errFlag;
}

/*Free the label that is in the maybe list*/
void freeMaybeALabel(struct MaybeALabel* head) {
    struct MaybeALabel* temp ;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp->label);
        free(temp);
    }
}