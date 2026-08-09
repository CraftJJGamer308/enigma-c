#include "utility.h"
#include <ctype.h>

void clean_string(char* str) {
    int i, j = 0;

    for (i = 0; str[i] != '\0'; i++) {
        if (isalpha((unsigned char)str[i]))
            str[j++] = toupper((unsigned char)str[i]);
    }

    str[j] = '\0';
}

inline int mod_26(int b) {
    return (b % 26 + 26) % 26;
}