#ifndef UTILITY_H
#define UTILITY_H
#include <ctype.h>

static char* clean_string(char* str) {
    int i, j = 0;

    for (i = 0; str[i] != '\0'; i++) {
        if (isalpha((unsigned char)str[i]))
            str[j++] = toupper((unsigned char)str[i]);
    }

    str[j] = '\0';
    return str;
}

static inline int mod_26(int b) {
    int temp = b % 26;
    return (temp < 0) ? temp + 26 : temp;
}
#endif