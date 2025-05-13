#ifndef macro_h
#define macro_h
#include <stdio.h>
#include <ctype.h>

#define MAX_MACRO_LENGTH 1024
#define MAX_LINE_LENGTH 80
/*A data structure for saving macros*/
struct Macro {
    char name[MAX_LINE_LENGTH];
    char lines[MAX_LINE_LENGTH][MAX_MACRO_LENGTH];
    int lineCount;
    struct Macro* next;
};
/* build a new macro struct with the name of the macro, the line in the macro, and the number of the lines*/
struct Macro* createMacro(char* name, FILE *stream);
/*Adds a new macro node to the list*/
void addMacro(struct Macro** head, struct Macro* newMacro);
/*isMacro function check if a line is a macro, and returns the appropriate macro otherwise returns NULL*/
struct Macro* isMacro(const char* line, struct Macro* head);
/*The main function of a macro is to open the files and write the relevant lines on them after deploying the macros*/
int macros(char* name_input_file);
/* Function to read a string from the file, that remove spaces and tabs*/
char *myfgets(char *str, int size, FILE *stream);
/*Function to remove spaces and tabs from the beginning and end of the line*/
void removeSpacesAndTabsMacro(char *word);
/*The function remove space from the end of the line*/
void removeSpaceInTheEnd(char *line);
/*Function to check if is a legal macro*/
int isALegalMacro(char *macroName);
/*the macro not found in the file*/
int macroDontExist(struct Macro* head, struct Macro* newNode);


#endif
