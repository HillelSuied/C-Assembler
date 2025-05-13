#include "macro.h"
#include "assembler.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    char* nameInputFile;
    int numberOfInputFile;
    int i=1;
    int errFlag=0;

    numberOfInputFile=argc-1;
    if(numberOfInputFile<1){
        printf("Error you did not insert a file\n");
        return 1;
    }

    while(numberOfInputFile){
        nameInputFile=argv[i];
        /*Calling a macro function*/
        if (macros(nameInputFile)){  /*macro not successful*/
            return 1;
        }else if (assembler(nameInputFile)){    /*err in assembler*/
            errFlag = 1;
        }
        numberOfInputFile--;
        i++;
    }

    return errFlag;
}
