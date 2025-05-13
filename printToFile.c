#include "printToFile.h"
#include "assembler.h"
#include "base64.h"
/*Function to print to the object file, which contains the machine code*/
int printCmdAndExtToObj(char *fileName, struct commands* head, int IC ,int DC)
{
    FILE *outputObjectFile;
    FILE *outputExtFile;
    char *nameObjFile, *nameExtFile;
    int flagObj=0, flagExt=0, countLine=100;
    struct commands* current = head;

    /*alloction memory for the name of the file*/
    nameObjFile=malloc(strlen(fileName) + 4);
    nameExtFile=malloc(strlen(fileName) + 4);
    /*run on the list of command and print the command and the opperand in the encode rule*/
    while (current != NULL)
    {
        /*this is the first line to print*/
        if (flagObj==0)
        {
            strcpy(nameObjFile, fileName);
            strcat(nameObjFile, ".ob");
            outputObjectFile = fopen(nameObjFile, "w"); /* Open the file for writing*/

            if (outputObjectFile == NULL) {
                printf("Error opening file\n");
                free(nameObjFile);
                free(nameExtFile);
                return 1;
            }
            /*print the number of the ic and dc in the start of the ob file*/
            fprintf(outputObjectFile, "%d %d\n",IC,DC);
            printBase64(current->opCode, outputObjectFile);
            flagObj=1;
        }
            /*if this isnt the first print to the file*/
        else{
            printBase64(current->opCode, outputObjectFile);
        }
        /*if the label is extern, print to the ext file*/
        if(AREextern(current->opCode))
        {
            /*if is the first extern need to open the ext file*/
            if(flagExt==0)
            {
                strcpy(nameExtFile, fileName);
                strcat(nameExtFile, ".ex");
                outputExtFile = fopen(nameExtFile, "w"); /* Open the extern file for writing*/
                if (outputExtFile == NULL) {
                    printf("Error opening file\n");
                    free(nameExtFile);
                    free(nameObjFile);
                    return 1;
                }
                fprintf(outputExtFile , "%s %d\n", current->commandName , countLine);
                flagExt=1;
            }
                /*this isnt the first extern*/
            else
                fprintf(outputExtFile , "%s %d\n", current->commandName , countLine);
        }
        countLine++;
        current=current->next;
    }
    fclose(outputObjectFile);
    if(flagExt==1)
    {
        fclose(outputExtFile);
    }
    free(nameObjFile);
    free(nameExtFile);
    return 0;
}
/*The function print to ob file all the data in the code*/
int printDataToObject(char *fileName, struct data* head)
{
    FILE *outputObjectFile;
    char *nameObjFile;
    int flagObj=0;
    struct data* current = head;

    nameObjFile=malloc(strlen(fileName) + 4);
    while (current != NULL)
    {
        /*if is the first data its open the file to append*/
        if (flagObj==0)
        {
            strcpy(nameObjFile, fileName);
            strcat(nameObjFile, ".ob");
            outputObjectFile = fopen(nameObjFile, "a"); /* Open the file for writing*/
            free(nameObjFile);
            if (outputObjectFile == NULL) {
                printf("Error opening file\n");
                return 1;
            }
            printBase64(current->value, outputObjectFile);
            flagObj=1;
        }
            /*this is not the first data it only prit it to the file*/
        else
            printBase64(current->value, outputObjectFile);
        current=current->next;
    }

    fclose(outputObjectFile);

    return 0;
}
/*The function print to ent file all the entry label in the code*/
int printLabelToFile(char *fileName, struct labelStruct* head)
{
    FILE *outputEntryFile;
    char *nameEntFile;
    int flagEnt=0;  /*alreadyIn=0 */
    struct labelStruct* current = head;
    struct labelStruct* temp;

    nameEntFile=malloc(strlen(fileName) + 5);
    while (current != NULL)
    {
        temp=current->next;
        while (temp != NULL){
            if(strcmp(current->label,temp->label)==0 ){
                if (current->typeOfLabel==isEntry || temp->typeOfLabel==isEntry){
                    if (flagEnt==0)
                    {
                        strcpy(nameEntFile, fileName);
                        strcat(nameEntFile, ".ent");
                        outputEntryFile = fopen(nameEntFile, "w"); /* Open the file for writing*/
                        if (outputEntryFile == NULL) {
                            printf("Error opening file\n");
                            free(nameEntFile); /* Free the allocated memory for nameEntFile */
                            return 1;
                        }
                        if(current->typeOfLabel==isEntry){
                            fprintf(outputEntryFile, "%s %hd\n", temp->label, temp->number_line_of_label);
                            flagEnt=1;
                        }else if(temp->typeOfLabel==isEntry){
                            fprintf(outputEntryFile, "%s %hd\n", current->label, current->number_line_of_label);
                            flagEnt=1;
                        }
                    }
                    else if(current->typeOfLabel==isEntry){
                        fprintf(outputEntryFile, "%s %hd\n", temp->label, temp->number_line_of_label);
                    }else if(temp->typeOfLabel==isEntry){
                        fprintf(outputEntryFile, "%s %hd\n", current->label, current->number_line_of_label);
                    }

                }

            }
            temp=temp->next;
        }
        current=current->next;
    }
    if (flagEnt==1){
        fclose(outputEntryFile);
    }
    free(nameEntFile); /* Free the allocated memory for nameEntFile */
    return 0;
}

/*Gets the list of labels and checks if each label that was declared as an entry was really defined at some point in the file*/
int checkIfEntDef(struct labelStruct* head)
{
    int errFlag=0, alreadyFind=0;
    struct labelStruct* current = head;
    struct labelStruct* temp;

    while (current != NULL)
    {
        if(current->typeOfLabel==isEntry){
            temp=head;
            while (temp != NULL){
                if(strcmp(current->label,temp->label)==0 && temp->typeOfLabel!=isEntry){
                    alreadyFind=1;
                }
                temp=temp->next;
            }
            if(alreadyFind==0){
                printf("Error at line %d The entry label was not defined in the file.\n",current->number_line_of_label);
                errFlag++;
            }
        }
        alreadyFind=0;
        current=current->next;
    }
    return errFlag;
}

/*check if the are is extern*/
int AREextern(short num)
{
    short ext;
    ext= num&1;
    if (ext==1){
        return 1;
    }
    else
        return 0;
}