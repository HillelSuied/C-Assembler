#include <stdlib.h>
#include <string.h>
#include "macro.h"
#include "assembler.h"

/* build a new macro struct with the name of the macro, the line in the macro, and the number of the lines*/
struct Macro* createMacro(char* name, FILE* inputFile) {
    /*Allocating space to the new macro*/
    struct Macro* newNode = (struct Macro*)malloc(sizeof(struct Macro));
    if (newNode != NULL) {
        char line[MAX_LINE_LENGTH];
        strcpy(newNode->name, name);
        newNode->lineCount = 0;
        newNode->next = NULL;
        /*A loop that stores line by line in the macro*/
        while (myfgets(line, MAX_LINE_LENGTH, inputFile) != NULL) {
            if(line[0]=='\0' || line[0]=='\n'|| (line[1]=='\n'&&(line[0]==' '||line[0]=='\t')))
                continue;
            /*If the macro is over*/
            if (strncmp(line, "endmcro",7) == 0) {
                /* Found end of the macro, break the loop*/
                break;
            }
            if (newNode->lineCount < MAX_MACRO_LENGTH) {
                /* Insert the line in the macro's lines array*/
                strcpy(newNode->lines[newNode->lineCount], line);
                newNode->lineCount++;
            }
        }
    }
    return newNode;
}

/*Adds a new macro node to the list*/
void addMacro(struct Macro** head, struct Macro* newMacro) {
    /*If the list is empty*/
    if (*head == NULL) {
        *head = newMacro;
    }
        /*Otherwise, it moves to the end of the list and adds the new macro at the end*/
    else {
        struct Macro* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newMacro;
    }
}

/*isMacro function check if a line is a macro, and returns the appropriate macro otherwise returns NULL*/
struct Macro* isMacro(const char* line, struct Macro* head) {
    /* Implement the logic to check if the line is a macro*/
    struct Macro* currentMacro = head;
    while (currentMacro != NULL) {
        if (strcmp(line, currentMacro->name) == 0) {
            return currentMacro;
        }
        currentMacro = currentMacro->next;
    }
    return NULL;
}
/*     i need to pass an argumant*/
/*The main function of a macro is to open the files and write the relevant lines on them after deploying the macros*/
int macros(char* name_input_file) {
    FILE *inputFile, *outputFile;
    char line[MAX_LINE_LENGTH], macroName[MAX_LINE_LENGTH];
    struct Macro* head = NULL;
    struct Macro* deleteM;
    struct Macro* pMacro = NULL;
    char *outputFileName;
    char *inputFileName;

    inputFileName=malloc(strlen(name_input_file) + 4);
    if(inputFileName==NULL){
        printf("Error while allocating memory malloc failed\n");
        return 1;
    }
    strcpy(inputFileName, name_input_file);        /* Copy the filename */
    strcat(inputFileName, ".as");

    /*Opening the input file for reading*/
    inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        printf("Error opening input file check that you have a file named:%s.as\n",name_input_file);
        free(inputFileName);
        return 1;
    }
    free(inputFileName);

    /*Opening the output file for writing*/
    outputFileName=malloc(strlen(name_input_file) + 4);
    if(outputFileName==NULL){
        printf("Error while allocating memory malloc failed\n");
        return 1;
    }
    strcpy(outputFileName, name_input_file);
    strcat(outputFileName, ".am");
    outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        printf("Error creating output file.\n");
        free(outputFileName);
        return 1;
    }
    free(outputFileName);

    /*A loop that receives lines from the file*/
    while (myfgets(line, MAX_LINE_LENGTH, inputFile) != NULL) {
        char *fristWorld, *nameOfMacro, *restOfLine;
        struct Macro* newMacro;

        if(line[0]=='\0' || line[0]=='\n'|| (line[1]=='\n'&&(line[0]==' '||line[0]=='\t'))){
            continue;
        }

        /*Checks if the row is a macro, if so it builds a struct macro and add it to the list*/
        strcpy(macroName,line);
        fristWorld= strtok(macroName," \t");
        if (strcmp(fristWorld, "mcro") == 0){
            if (isALegalMacro(fristWorld)==1){
                printf("Error is an illegal macro name:%s.\n",fristWorld);
                fclose(inputFile);
                fclose(outputFile);
                return 1;
            }
            nameOfMacro = strtok(NULL," \t");
            restOfLine = strtok(NULL,"");
            if(restOfLine!=NULL){
                printf("Error invalid macro start definition Unnecessary parameters:%s.\n",restOfLine);
                fclose(inputFile);
                fclose(outputFile);
                return 1;
            }
            newMacro = createMacro(nameOfMacro, inputFile);
            if(macroDontExist(head, newMacro)){
                addMacro(&head, newMacro);
            }else{
                printf("Error The macro:%s is already defined.\n",nameOfMacro);
                fclose(inputFile);
                fclose(outputFile);
                return 1;
            }
        }
        else if (strcmp(fristWorld, "endmcro") == 0){
            restOfLine = strtok(NULL,"");
            if(restOfLine!=NULL){
                printf("Error invalid macro end definition Unnecessary parameters:%s.\n",restOfLine);
                fclose(inputFile);
                fclose(outputFile);
                return 1;
            }
        }

            /*Checks whether the received line is the name of a macro*/
        else if((pMacro=isMacro(line, head))!= NULL)
        {
            /*Prints the lines of the relevant macro to the file*/
            int i;
            for (i = 0; i < pMacro->lineCount; i++)
                fprintf(outputFile, "%s", pMacro->lines[i]);
        }
            /*If it is a line not related to a macro it will print it to a file*/
        else {
            fprintf(outputFile, "%s", line);
        }
    }
    /*Close the files*/
    fclose(inputFile);
    fclose(outputFile);

    /* Free the memory allocated for the macros*/
    while (head != NULL) {
        deleteM = head;
        head = head->next;
        free(deleteM);
    }

    return 0;
}
/* Function to read a string from the file, that remove spaces and tabs*/
char *myfgets(char *str, int size, FILE *stream) {
    int c;/* Variable to store each character read*/
    char *ptr = str;/* Pointer to track the current position in the string*/
    int num_chars = size - 1;/* Maximum number of characters to read (excluding null terminator)*/
    int flag=0;
    int start=0;

    while (num_chars > 0 && (c = fgetc(stream)) != EOF) {
        if((c==' '&&start==1) || (c==' '&&start==0) || (c=='\t'&&start==1) || (c=='\t'&&start==0))
            start=1;
        else
            start=2;
        if ((flag==1 && c==' ')||(flag==1 && c=='\t')  || start==1)
            continue;

        if(c==' '||c=='\t')
            flag=1;
        else
            flag=0;
        *ptr++ = (char)c;/* Store the character in the string and move the pointer*/
        num_chars--;

        if (c == '\n')  /* If a newline character is encountered, stop reading*/
            break;
    }

    if (ptr == str || ferror(stream))
        return NULL;            /* Return NULL if an error occurred or no characters were read*/

    *ptr = '\0';                /* Add the null terminator at the end of the string*/

    removeSpaceInTheEnd(str);
    return str;                 /* Return the pointer to the string*/
}

/*A function that deletes spaces and tabs*/
void removeSpacesAndTabsMacro(char *word) {
    /* Remove leading spaces and tabs */
    char *start = word;
    char *end;
    while (isspace((unsigned char)*start)) {
        start++;
    }
    /* Remove trailing spaces and tabs */
    end = word + strlen(word) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }
    *(end + 1) = '\0';
    /* Move the trimmed word to the beginning of the input buffer */
    if (start != word) {
        memmove(word, start, end - start + 2);
    }
}
/*The function remove space from the end of the line*/
void removeSpaceInTheEnd(char *line) {
    int index;
    int len = strlen(line);
    if (len == 0) {
        return;  /* Empty line, nothing to do*/
    }

    index = len - 2;
    if(index >= 0 && isspace((unsigned char)line[index])) {
        line[index] = '\n';
        line[index + 1] = '\0';
    }
}

/*Function to check if is a legal macro*/
int isALegalMacro(char *macroName){
        int errFlag;
        errFlag=0;
        /*if the macro name equals to register or to command*/
        if(isRegister(macroName)>=0 || compareToCommand(macroName)==1 ){
            errFlag=1;
        }
        /*if the letter is not alphbet or number*/
        while (*macroName != '\0') {
            if (!isalnum(*macroName)) {
                errFlag=1 ;
            }
            macroName++;
        }
        return errFlag;
}
/*the macro not found in the file*/
int macroDontExist(struct Macro* head, struct Macro* newNode) {
    struct Macro* current = head;
    while (current != NULL) {
        if (strcmp(current->name, newNode->name) == 0 ) {
            /*The label already exists in the list*/
            free(newNode);
            return 0;
        }
        current = current->next;
    }
    /* The label does not exist in the list*/
    return 1;
}
