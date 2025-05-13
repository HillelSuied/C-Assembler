#include "assembler.h"
#include "binary.h"
#include "base64.h"
#include "maybeLabel.h"
#include "printToFile.h"
#include "Label.h"
#include "Free.h"
#include "lineFix.h"
#include "wordChecks.h"
#include "insert.h"

/*The main function receives the name of the file and it will read all the lines from the after macro file and handle them as defined*/
int assembler(char* name_input_file)
{
    char orginalLine[MAX_LINE];
    char commandArray[MAX_LINE];
    char labelArray[MAX_LINE];
    char *command, *label, *line;
    int IC = 100;/*counter for the command line*/
    int DC = 0;/*counter for the data line*/
    int L;/*counter for the number of line the commend line take*/
    int isAlabel = 0;/*flag for check if there is a label*/
    int countOfLine=1;
    FILE *file_in;
    struct StructPointers pointer = {NULL, NULL, NULL, NULL,0};
    struct labelStruct *new_label;
    char *file_name;
    /*allocatain memory for the name of the file*/
    file_name=malloc(strlen(name_input_file) + 4);
    if(file_name==NULL){
        printf("Error while allocating memory malloc failed\n");
        return 1;
    }
    /*add to the name of the file, the file extension */
    strcpy(file_name, name_input_file);
    strcat(file_name, ".am");
    /*Opening the input file for reading*/
    file_in = fopen(file_name, "r");
    if (file_in == NULL) {
        printf("Failed to open the input file.\n");
        free(file_name);
        return 1;
    }
    free(file_name);
    /*while loop that read from the am file line after line and analyzing the command in the line*/
    while (fgets(orginalLine, MAX_LINE, file_in) != NULL) {
        /*the number of the line in the assembler saved on the L variabel*/
        L = 0;
        /*if the line is a comment*/
        if(orginalLine[0]==';'){
          countOfLine++;
          continue;  
        }
        /*copy the original line to temporary char**/
        strcpy(commandArray, orginalLine);
        strcpy(labelArray, orginalLine);
        /*cut the command from the line*/
        command = strtok(commandArray, " \t");
        /*cut the label from the line*/
        label = strtok(labelArray, ":");

        if(command!=NULL && label!=NULL){
            removeSpacesAndTabs(command);
            removeSpacesAndTabs(label);
            removeNewline(command);
            removeNewline(label);
        }

        /*check if there is a label in the line*/
        if (strlen(command) - strlen(label) == 1) {
            if (isALegalLabel(label)==1){
                printf("Error in line:%d %s Invalid label name\n",countOfLine, label);
                pointer.errorFlag=1;
                isAlabel = 0;
                countOfLine++;
                continue;
            }
            isAlabel = 1;
            command = strtok(NULL, " \t");
            strtok(orginalLine, " \t");
            line = strtok(NULL, "");
        }
        else
        {
            line = orginalLine;
        }
        /*check if the command is .data*/
        if (strcmp(command, ".data") == 0) {
            char *token;
            int i = 0;

            if(isAlabel)
            {
                /*create new label*/
                new_label = createLabelStruct(label, isData, DC);
                /*check if the label is exist in the code*/
                if (label_exist(&(pointer.ptrTolabelStruct), new_label)) {
                    printf("Error in line:%d label:%s already exist data\n", countOfLine,label);
                    pointer.errorFlag=1;
                    isAlabel = 0;
                    countOfLine++;
                    continue;
                }
                else {
                    /*insert the label to the list of labels*/
                    listOfLabelStruct(&(pointer.ptrTolabelStruct), new_label);
                }
            }
            else{
                printf("Warning in line:%d there is a data without a label", countOfLine);
            }
            /*insert the numbers in data to list of data with while loop that insert number one after the other, and encode them to the binary code */
            removeSpacesAndTabs(line);
            strtok(line, " ");
            token = strtok(NULL, ",");
            while (token != NULL) {
                insertData(&(pointer.ptrToData), atoi(token));
                token = strtok(NULL, ", ");
                i++;
            }
            /*update the data list number to the number of the numbers*/
            DC = DC + i;
            isAlabel = 0;
            countOfLine++;
            continue;
        }
        /*check if the command is .string*/
        if (strcmp(command, ".string") == 0)
        {
            char *token;
            size_t length;
            int i = 0;
            /*create new label*/
            if(isAlabel)
            {
                new_label = createLabelStruct(label, isData, DC);/*make new label struct*/
                /*check if the label is exist in the list*/
                if (label_exist(&(pointer.ptrTolabelStruct), new_label)) {
                    printf("Error in line:%d label:%s already exist.\n", countOfLine,label);
                    pointer.errorFlag=1;
                    isAlabel = 0;
                    countOfLine++;
                    continue;
                }
                else
                {
                    /*insert the new label*/
                    listOfLabelStruct(&(pointer.ptrTolabelStruct), new_label);
                }
            }else{
                printf("Warning in line:%d there is a string without a label", countOfLine);
            }

            /*clear the line from space and tab and new line char from the end*/
            removeNewline(line);
            removeSpacesAndTabs(line);

            /*check if the string is not in format "---"*/
            strtok(line, "\"");   /*[\0]hello world"*/
            token= strtok(NULL, "\"");    /*hello world[\0]*/
            /*if there is no string*/
            if (token==NULL){
                printf("Error in line:%d we dont have a string\n", countOfLine);
                pointer.errorFlag=1;
                isAlabel = 0;
                countOfLine++;
                continue;
            }
            removeNewline(token);
            length = strlen(token);

            for (i = 0; i < length ; i++) {
                if (!isprint(token[i]) ) {
                    printf("Error in line:%d Invalid string:\"%s\" contains illegal characters\n", countOfLine,token);
                pointer.errorFlag=1;
                isAlabel = 0;
                countOfLine++;
                continue;
                }
            }

            /*loop that run on the length of the string and insert char by char to the list of data*/
            for(i=0; i<length; i++){
                insertData(&(pointer.ptrToData), (short)token[i]);
            }
            /*insert EOF to the list of data*/
            insertData(&(pointer.ptrToData), (short)'\0');
            i++;
            DC=DC+i;
            isAlabel = 0;
            countOfLine++;
            continue;
        }

        /*compare the command to .extern*/
        if (strcmp(command, ".extern") == 0) {
            char *token, *token2;
            char tokenArray[MAX_LABEL];
            int numberOfParam=0;
            int numberOfParamWithoutSpace=0;
            /*clear the line from space and tab and new line char from the end*/
            removeNewline(line);
            strtok(line, " ");  /*line=".extern[\0]W , L3 LOOP"*/
            token= strtok(NULL, ""); /*token="W , L3, ,LOOP, T5"*/
            /*there is no line*/
            if (token==NULL) {
                printf("Error in line:%d we dont have a extern label\n", countOfLine);
                pointer.errorFlag=1;
                isAlabel = 0;
                countOfLine++;
                continue;
            }
            strcpy(tokenArray, token);
            /*remove space from the line*/
            removeSpaces(token);  /*token="W,L3[\0]"*/
            token2=strtok(tokenArray," ,");
            /*counter for the number of label in the extern command*/
            while(token2!=NULL){
                numberOfParam++;
                token2=strtok(NULL," ,");
            }
            token=strtok(token,",");
            /*while loop that insert every label to the label list*/
            while(token!=NULL)  /*token="W[\0]L3,T5"*/
            {
                removeNewline(token);
                new_label = createLabelStruct(token, isExtern, DC);/*make new label struct*/

                if (label_exist(&(pointer.ptrTolabelStruct), new_label)) {
                    printf("Error in line:%d label %s already exist.\n", countOfLine, token);
                    pointer.errorFlag=1;
                    isAlabel = 0;
                    countOfLine++;
                    continue;
                }
                else
                {
                    /*insert the new label*/
                    listOfLabelStruct(&(pointer.ptrTolabelStruct), new_label);
                }
                numberOfParamWithoutSpace++;
                token=strtok(NULL,",");
            }
            /*if the number of "," differet from the number of the labels*/
            if (numberOfParamWithoutSpace<numberOfParam){
                printf("Error in line:%d we have parameters without coma.\n", countOfLine);
                pointer.errorFlag=1;
                isAlabel = 0;
                countOfLine++;
                continue;
            }
         isAlabel = 0;
         countOfLine++;
         continue;
        }
        /*check if the command is .string*/
        if (strcmp(command, ".entry") == 0){
            char *token, *token2;
            char tokenArray[MAX_LABEL];
            int numberOfParam=0;
            int numberOfParamWithoutSpace=0;

            strtok(line, " ");  /*line=".entry[\0]W , L3 LOOP"*/
            token= strtok(NULL, ""); /*token="W , L3, ,LOOP, T5"*/
            if (token==NULL) {
                printf("Error in line:%d we dont have a extern label.\n", countOfLine);
                pointer.errorFlag=1;
                isAlabel = 0;
                countOfLine++;
                continue;
            }
            strcpy(tokenArray, token);
            removeSpaces(token);
            token2=strtok(tokenArray," ,");
            while(token2!=NULL){
                numberOfParam++;
                token2=strtok(NULL," ,");
            }
            token=strtok(token,",");
            while(token!=NULL)
            {
                removeNewline(token);
                new_label = createLabelStruct(token, isEntry, DC);/*make new label struct*/
                /*insert the new label*/
                listOfLabelStruct(&(pointer.ptrTolabelStruct), new_label);
                numberOfParamWithoutSpace++;
                token=strtok(NULL,",");
            }
            if (numberOfParamWithoutSpace<numberOfParam){
                printf("Error in line:%d we have parameters without coma.\n", countOfLine);
                pointer.errorFlag=1;
                isAlabel = 0;
                countOfLine++;
                continue;
            }
          isAlabel = 0;
          countOfLine++;
          continue;
        }

        /*compare the command to the list of commands*/
        if ((compareToCommand(command))==1)
        {
            /*if there is a label in the line*/
            if (isAlabel==1)
            {
                /*create new label*/
                new_label=createLabelStruct(label,isCode,IC);
                if (label_exist(&(pointer.ptrTolabelStruct),new_label)){
                    printf("Error in line:%d\n defining a label that has already been defined:%s.\n",countOfLine,label);
                    pointer.errorFlag=1;
                    isAlabel = 0;
                    countOfLine++;
                    continue;
                }
                else
                {
                    /*insert the new label*/
                    listOfLabelStruct(&(pointer.ptrTolabelStruct),new_label);
                }
            }

            /*Sends the continuation of the line to the function that parses the command and the operands, the function returning the number of lines in the binary encode the line takes*/
            L= insertTheCommand(&(pointer.ptrToCommands),&(pointer.ptrToMaybeALabel), command, line, countOfLine);
            /*there is a problem in the line*/
            if(L==0)
            {
                pointer.errorFlag=1;
            }
            /*update the numbers of line of commands counter*/
            IC+=L;
        }
        /*the command is not exist*/
        else
        {
            if(isAlabel){
                printf("Warning line %d has an empty label:%s.\n",countOfLine, label);
            }else{
                printf("Error in line:%d invalid command:%s.\n",countOfLine, command);
                pointer.errorFlag=1;
            }
        }

        isAlabel = 0;
        countOfLine++;
    }
    /*The second pass*/
    if(checkIfEntDef(pointer.ptrTolabelStruct)){
        pointer.errorFlag=1;
    }
    if(checkIfItWasLabel(pointer.ptrTolabelStruct,pointer.ptrToMaybeALabel)){
        pointer.errorFlag=1;
    }

    /*if there is a problem in some line, this free all the struct and the lists*/
    if (pointer.errorFlag){
        freeLabel(pointer.ptrTolabelStruct);
        freeCommands(pointer.ptrToCommands);
        freeData(pointer.ptrToData);
        freeMaybeALabel(pointer.ptrToMaybeALabel);
        fclose(file_in);
        return 1;
    }
    /*Updates the line number of the data labels*/
    updateDataNumLine(&(pointer.ptrTolabelStruct), IC);
    insertLabelLines(&(pointer.ptrToCommands), &(pointer.ptrTolabelStruct));
    /*Print to files*/
    printCmdAndExtToObj(name_input_file, pointer.ptrToCommands,IC-100,DC);
    printDataToObject(name_input_file, pointer.ptrToData);
    printLabelToFile(name_input_file, pointer.ptrTolabelStruct);
    /*Free functions*/
    freeLabel(pointer.ptrTolabelStruct);
    freeCommands(pointer.ptrToCommands);
    freeData(pointer.ptrToData);
    freeMaybeALabel(pointer.ptrToMaybeALabel);

    fclose(file_in);

    return 0;
}

/*struct to commands names and the numbers of operands that is recieved*/
struct command{
    char *name_cmd;
    int num;
    int operand;
} commandArray[]={
        {"mov", 0, 2},
        {"cmp", 1,2},
        {"add", 2,2},
        {"sub", 3,2},
        {"not", 4,1},
        {"clr", 5,1},
        {"lea", 6,2},
        {"inc" , 7,1},
        {"dec", 8,1},
        {"jmp" , 9,1},
        {"bne", 10,1},
        {"red", 11,1},
        {"prn" , 12,1},
        {"jsr", 13,1},
        {"rts" , 14,0},
        {"stop" , 15,0}
};
/*struct for th registers*/
struct reg{
    char *name_register;
}registerArray[]={
        {"@r0"},
        {"@r1"},
        {"@r2"},
        {"@r3"},
        {"@r4"},
        {"@r5"},
        {"@r6"},
        {"@r7"}

};

/*check if the name is recister if true return the number of the register else -1*/
int isRegister(char *nameRegister)
{
    int i;
    for(i=0; i<8; i++)
    {
        /*check if is a register*/
        if (strcmp(registerArray[i].name_register, nameRegister) == 0)
            return i;
    }
    return -1;
}

/*Function that recived the name of the command and compare the name to the list of command*/
int compareToCommand(char* com)
{
    int i;
    removeNewline(com);
    for(i=0; i<16 ;i++)
    {
        if(strcmp(commandArray[i].name_cmd, com)==0)
            return 1;
    }
    return 0;
}

/*return the number of the oprerand that the command receive*/
int numOfOperand(char *name)
{
    int i;
    for(i=0; i<16 ;i++)
    {
        if(strcmp(commandArray[i].name_cmd, name)==0)
            return (commandArray[i].operand);
    }
    return 3;
}

/*return the number of the opcode*/
int opCodeNum(char *name)
{
    int i;
    for(i=0; i<16 ;i++)
    {
        /*search the name of command in the command array*/
        if(strcmp(commandArray[i].name_cmd, name)==0)
            return commandArray[i].num;/*return the number of the command*/
    }
    return 0;
}









