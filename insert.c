#include "assembler.h"
#include "insert.h"


/*Fuction to insert the command and the operands to the list of ic the binary encode
  The function also checks the correctness of the parameters and commands */
int insertTheCommand(struct commands** head, struct MaybeALabel** headML, char* command, char* line, int numOfLine){
    int operand;
    short opCode=0, target=0, source=0;/*numbers to the save the command, the target opperand and the source operad*/
    int number,regSource, regTarget;
    struct MaybeALabel *newMaybeLabel;/**/

    operand=numOfOperand(command);  /*The number of operands corresponding to the command*/
    if (operand==3) {  /*the command not exist*/
        printf("Error in line:%d, the command:%s not exist.\n",numOfLine, command);
        return 0;
    }
        /*case number 1 the command have 0 operands*/
    else if (operand==0){
        char *restOfLine;
        strtok(line, " ");  /*remove the command*/
        restOfLine= strtok(NULL, "");
        if(restOfLine!=NULL){ /*we have unexpected parameters*/
            printf("Error in line:%d unexpected parameters:%s.\n", numOfLine, restOfLine);
            return 0;
        }
        opCode= CommandBinary(opCodeNum(command));
        insertOpCode(head, opCode, command, numOfLine);
        return 1;
    }

        /*case number 2 the command have 1 operands*/
    else if (operand==1){
        char *param1, *restOfLine;
        opCode= CommandBinary(opCodeNum(command));
        strtok(line, " ");    /*remove the command*/
        param1 = strtok(NULL, " ");
        restOfLine = strtok(NULL, "");  /*this is the rest of the line without the cmd and the parameters*/
        if (restOfLine != NULL) { /*we get too much param*/
            printf("Error in line:%d unexpected parameters:%s.\n", numOfLine, restOfLine);
            return 0;
        }
        if (param1 != NULL) {   /*we have parameters*/
            removeSpacesAndTabs(param1);
            removeNewline(param1);
            if (isNumber(param1)==1) {  /*the param1 is a number*/
                if (opCodeNum(command)!=12){  /*if is not prn*/
                    printf("Error in line:%d the parameter:%s is invalid.\n",numOfLine,param1);
                    return 0;
                }
                number = atoi(param1);
                opCode = targetOperandBinary(opCode, 1);
                target = numberOperandBinary(target, number);
                insertOpCode(head, opCode, command, numOfLine);
                insertOpCode(head, target, param1, numOfLine);
                return 2;
            } else {   /*the param1 is not a number*/
                /*check if is a register*/
                if ((regTarget = isRegister(param1)) != -1) {
                    /*this is register*/
                    /*the line in the binary code that represents the command*/
                    opCode = targetOperandBinary(opCode, 5);
                    /*the line in the binary code that represents the operand*/
                    target = targetRegisterBinary(target, regTarget);
                    /*insert the command binary code to the list*/
                    insertOpCode(head, opCode, command, numOfLine);
                    /*insert the operand(register) binary code to the list*/
                    insertOpCode(head, target, param1, numOfLine + 1);
                    return 2;
                } else {
                    /*param1 is not a number and olso not a register maybe is a label*/
                    newMaybeLabel = createMaybeLabel(param1, numOfLine);
                    if(MaybeLabelDontExist(*headML,newMaybeLabel)){
                        insertMaybeALabel(headML,newMaybeLabel);
                    }
                    opCode = targetOperandBinary(opCode, 3);
                    insertOpCode(head, opCode, command, numOfLine);
                    insertOpCode(head, target, param1, numOfLine);
                    return 2;
                }
            }
        }
        else { /* we dont have parameters */
            printf("Error in line:%d missing operands.\n", numOfLine);
            return 0;
        }
    }
        /*case number 3 the command have 2 operands*/
    else if (operand==2){
        char *firstParm, *secoundParm, *restOfLine, *strForStrCat;
        strtok(line, " ");      /*remove the command*/

        firstParm = strtok(NULL, ",");/*save the first param*/
        secoundParm = strtok(NULL, " ");/*save the secound param*/
        restOfLine=strtok(NULL, "");/*save the continued of the line, if there is continue print error*/

        /*Coding the command to the command line*/
        opCode= CommandBinary(opCodeNum(command));
        if (firstParm==NULL){
            printf("Error in line:%d missing the first parameter.\n", numOfLine);
            return 0;
        }
        if (secoundParm==NULL){
            printf("Error in line:%d missing the secound parameter\n", numOfLine);
            return 0;
        }
        if (restOfLine!=NULL){    /*we get too much param*/
            printf("Error in line:%d unexpected parameters:%s.\n", numOfLine,restOfLine);
            return 0;
        }
        if (firstParm!=NULL && secoundParm!=NULL)
        {
            /*clear the line from space and tab and new line char from the end*/
            removeSpacesAndTabs(firstParm);
            removeSpacesAndTabs(secoundParm);
            removeNewline(firstParm);
            removeNewline(secoundParm);

            /*When the first parameter is a register*/
            if (isRegister(firstParm)>=0)
            {
                if (opCodeNum(command)==6){ /*if is the lea command*/
                    printf("Error in line:%d the parameter:%s is invalid for lea command.\n",numOfLine,firstParm);
                    return 0;
                }
                regSource=isRegister(firstParm);
                /*Encodes the command line, the source operand is a register*/
                opCode=sourceCommandBinary(opCode, 5);
                /*If the target operand is also a register*/
                if((regTarget=isRegister(secoundParm))>=0)
                {
                    opCode=targetOperandBinary(opCode, 5);
                    target=targetRegisterBinary(target, regTarget);
                    target=sourceRegisterBinary(target, regSource);
                    insertOpCode(head, opCode, command, numOfLine);
                    strForStrCat=malloc(strlen(firstParm) + strlen(secoundParm) + 1);
                    if(strForStrCat==NULL){
                        printf("Error line:%d while allocating memory malloc failed.\n",numOfLine);
                        return 0;
                    }
                    strcpy(strForStrCat, firstParm);
                    insertOpCode(head, target, strcat(strForStrCat,secoundParm), numOfLine);
                    free(strForStrCat);
                    return 2;
                }
                /*case that the first param is register but the secound is number*/
                if (isNumber(secoundParm)) {
                    if(opCodeNum(command)!=1){ /*if the command isn't cmp*/
                        printf("Error in line %d the parameter:%s is invalid\n",numOfLine,secoundParm);
                        return 0;
                    }
                    number = atoi(secoundParm);
                    opCode= targetOperandBinary(opCode, 1);
                    source= sourceRegisterBinary(source, regSource);
                    target= targetOperandBinary(target, number);
                    insertOpCode(head,opCode,command,numOfLine);
                    insertOpCode(head,source,firstParm ,numOfLine);
                    insertOpCode(head,target ,secoundParm,numOfLine);
                    return 3;
                }
                else /*if the second param is maybe a label*/
                {
                    newMaybeLabel = createMaybeLabel(secoundParm, numOfLine);
                    if(MaybeLabelDontExist(*headML,newMaybeLabel)){
                        insertMaybeALabel(headML,newMaybeLabel);
                    }
                    opCode= targetOperandBinary(opCode, 3);
                    source= sourceRegisterBinary(source, regSource);
                    insertOpCode(head,opCode,command,numOfLine);
                    insertOpCode(head,source,firstParm ,numOfLine);
                    insertOpCode(head,target ,secoundParm,numOfLine);
                    return 3;
                }
            }

                /*case that the first param is not a register*/
            else if (isNumber(firstParm)) {
                if (opCodeNum(command)==6){ /*if is the lea command*/
                    printf("Error in line:%d the parameter:%s is invalid for lea command.\n",numOfLine,firstParm);
                    return 0;
                }
                number = atoi(firstParm);
                /*Command line encoding, source operand is a number*/
                opCode= sourceCommandBinary(opCode, 1);
                source= numberOperandBinary(source, number);
                /*The target operand is a register*/
                if((regTarget=isRegister(secoundParm))>=0)
                {
                    /*Encoding the operands and inserting them into the list*/
                    opCode=targetOperandBinary(opCode, 5);
                    target= targetRegisterBinary(target, regTarget);
                    insertOpCode(head,opCode,command,numOfLine);
                    insertOpCode(head, source, firstParm,numOfLine);
                    insertOpCode(head,target,secoundParm,numOfLine);
                    return 3;

                }
                else if(isNumber(secoundParm)){
                    if(opCodeNum(command)!=1){ /*if the command isn't cmp*/
                        printf("Error in line:%d the parameter:%s is invalid.\n",numOfLine,secoundParm);
                        return 0;
                    }
                    /*Encoding the operands and inserting them into the list*/
                    opCode=targetOperandBinary(opCode, 1);
                    target=numberOperandBinary(target, atoi(secoundParm));
                    insertOpCode(head,opCode,command,numOfLine);
                    insertOpCode(head, source, firstParm,numOfLine);
                    insertOpCode(head,target,secoundParm,numOfLine);
                    return 3;
                }
                else /*The target operand is maybe a label*/
                {
                    newMaybeLabel = createMaybeLabel(secoundParm, numOfLine);
                    if(MaybeLabelDontExist(*headML,newMaybeLabel)){
                        insertMaybeALabel(headML,newMaybeLabel);
                    }
                    opCode=targetOperandBinary(opCode, 3);
                    insertOpCode(head,opCode,command,numOfLine);
                    insertOpCode(head, source, firstParm,numOfLine);
                    insertOpCode(head,target,secoundParm,numOfLine);
                    return 3;
                }
            }

                /*case that the first param is not a register and also not a number*/
            else if((regTarget=isRegister(secoundParm))>=0){
                newMaybeLabel = createMaybeLabel(firstParm, numOfLine);
                if(MaybeLabelDontExist(*headML,newMaybeLabel)){
                    insertMaybeALabel(headML,newMaybeLabel);
                }
                opCode= sourceCommandBinary(opCode, 3);
                opCode= targetOperandBinary(opCode, 5);
                target= targetRegisterBinary(target,regTarget);
                insertOpCode(head,opCode,command,numOfLine);
                insertOpCode(head, source, firstParm,numOfLine);
                insertOpCode(head,target,secoundParm,numOfLine);
                return 3;

            }
            else if(isNumber(secoundParm)){
                newMaybeLabel = createMaybeLabel(firstParm, numOfLine);
                if(MaybeLabelDontExist(*headML,newMaybeLabel)){
                    insertMaybeALabel(headML,newMaybeLabel);
                }
                if(opCodeNum(command)!=1){ /*if the command isn't cmp*/
                    printf("Error in line %d the parameter:%s is invalid.\n",numOfLine,secoundParm);
                    return 0;
                }
                opCode= sourceCommandBinary(opCode, 3);
                opCode= targetOperandBinary(opCode, 1);
                target= numberOperandBinary(target,atoi(secoundParm));
                insertOpCode(head,opCode,command,numOfLine);
                insertOpCode(head, source, firstParm,numOfLine);
                insertOpCode(head,target,secoundParm,numOfLine);
                return 3;
            }
            else{ /*Both parameters may be labels*/
                newMaybeLabel = createMaybeLabel(firstParm, numOfLine);
                if(MaybeLabelDontExist(*headML,newMaybeLabel)){
                    insertMaybeALabel(headML,newMaybeLabel);
                }
                newMaybeLabel = createMaybeLabel(secoundParm, numOfLine);
                if(MaybeLabelDontExist(*headML,newMaybeLabel)){
                    insertMaybeALabel(headML,newMaybeLabel);
                }
                opCode= sourceCommandBinary(opCode, 3);
                opCode= targetOperandBinary(opCode, 3);
                insertOpCode(head,opCode,command,numOfLine);
                insertOpCode(head, source, firstParm,numOfLine);
                insertOpCode(head,target,secoundParm,numOfLine);
                return 3;
            }
        }
    }   /*end of case 2 operand==2 */
    return 0;
}

/*the functin insert command or operand to struct and insert the struct to the end of the list*/
void insertOpCode(struct commands** head, short opCode, char *commandName, int numOfLine) {
    struct commands* newNode = (struct commands*)malloc(sizeof(struct commands));
    newNode->opCode = opCode;
    newNode->commandName = (char*)malloc(strlen(commandName) + 1);
    strcpy(newNode->commandName, commandName);
    newNode->number_line_of_command = numOfLine;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        struct commands* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}
/*The function insert the label lines to the operand*/
void insertLabelLines(struct commands** headCom, struct labelStruct** headLabel)
{
    struct commands* currentCom = *headCom;
    while (currentCom != NULL)
    {
        struct labelStruct* currentLabel = *headLabel;
        while(currentLabel != NULL)
        {
            removeSpaces(currentCom->commandName);
            removeSpaces(currentLabel->label);
            removeNewline(currentCom->commandName);
            removeNewline(currentLabel->label);
            if(strcmp(currentCom->commandName, currentLabel->label)==0)
            {
                if(currentLabel->typeOfLabel==isExtern)
                {
                    currentCom->opCode=binaryExtern(currentCom->opCode);
                }
                else /*if(currentLabel->typeOfLabel==isEntry)*/
                {
                    currentCom->opCode = binaryEntry(currentCom->opCode);
                    currentCom->opCode = binaryLine(currentCom->opCode, currentLabel->number_line_of_label);
                }

            }
            currentLabel=currentLabel->next;
        }
        currentCom=currentCom->next;
    }

}

/*The function received the head of data list and the value to inser to the list' and insert it to the end of the list*/
void insertData(struct data** head, short value) {
    struct data* newNode = (struct data*)malloc(sizeof(struct data));
    newNode->value = value;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        struct data* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

/*update the data num of line after the first laps to IC+the number od the line in data*/
void updateDataNumLine(struct labelStruct** head, int numOfIC)
{
    struct labelStruct* current = *head;

    while (current != NULL) {
        if(current->typeOfLabel==isData)
            current->number_line_of_label= current->number_line_of_label+numOfIC;

        current= current->next;
    }
}