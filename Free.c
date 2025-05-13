#include "assembler.h"
#include "Free.h"

/*Function to free all the struct of data*/
void freeData(struct data* head) {
    struct data* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
/*Function to free all the struct of labels*/
void freeLabel(struct labelStruct* head) {
    struct labelStruct* temp ;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp->label);
        free(temp);
    }
}
/*Function to free all the struct of commands, and the command name*/
void freeCommands(struct commands* head) {
    struct commands* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp->commandName);
        free(temp);
    }
}