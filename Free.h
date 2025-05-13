

#ifndef FREE_H
#define FREE_H
/*Function to free all the struct of data*/
void freeData(struct data* head);
/*Function to free all the struct of labels*/
void freeLabel(struct labelStruct* head);
/*Function to free all the struct of commands, and the command name*/
void freeCommands(struct commands* head);
#endif
