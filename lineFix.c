
#include "assembler.h"
#include "lineFix.h"

/*The function remove space and tab*/
void removeSpacesAndTabs(char *word) {
    /* Remove leading spaces and tabs */
    char *end;
    char *start = word;
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
/*The function remove new line char from the end of str*/
void removeNewline(char *str) {
    size_t length = strlen(str);
    if (length > 0 && str[length - 1] == '\n') {
        str[length - 1] = '\0';
    }
}
/*The function remove all space in the line*/
void removeSpaces(char *str) {
    int len = strlen(str);
    int i, j = 0;
    for (i = 0; i < len; i++) {
        if (str[i] != ' ') {
            str[j] = str[i];
            j++;
        }
    }
    str[j] = '\0';
}