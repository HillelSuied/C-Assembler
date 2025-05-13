
#include "assembler.h"
#include "wordChecks.h"



/*The function check if the char* is a number*/
int isNumber(const char *str) {
    char *endptr;
    strtol(str, &endptr, 10);
    /* Check if the endptr points to the same location as the input string */
    /* If they are the same, no valid conversion happened */
    if (endptr == str) {
        return 0;
    }
    /* Check if there are any remaining characters that are not spaces */
    while (*endptr != '\0' && *endptr != '\n') {
        if (*endptr != ' ') {
            return 0;
        }
        endptr++;
    }
    return 1;
}

/*Function to check if is a legal label*/
int isALegalLabel(char *label){
    int errFlag;
    errFlag=0;
    /*if the label equals to register or to command*/
    if(isRegister(label)>=0 || compareToCommand(label)==1 ){
        errFlag=1;
    }
    /*if the letter is not alphbet or number*/
    while (*label != '\0') {
        if (!isalnum(*label)) {
            errFlag=1 ;
        }
        label++;
    }
    return errFlag;
}


