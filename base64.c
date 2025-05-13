#include <stdio.h>

/*convert the binary number to base 64*/
char convertToBase64(short value) {
    /*array of the base 64 char*/
    char base64[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    /*return the char in the array*/
    return base64[value];
}

/*The function prints the BASE64 encoding to the object file*/
void printBase64(short binaryNumber, FILE *stream) {
    short mask = 63; /* Mask for 6 bits*/
    char word[3]; /* To store the Base64 characters*/

    word[0] = convertToBase64((binaryNumber >> 6) & mask); /* First 6 bits*/
    word[1] = convertToBase64(binaryNumber & mask); /* Last 6 bits*/
    word[2] = '\0';
    fprintf(stream,"%s\n", word); /* Print the Base64 representation */
}

