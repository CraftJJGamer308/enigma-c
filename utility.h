#ifndef UTILITY_H
#define UTILITY_H
#include <ctype.h>

char* clean_string(char* str) {
    int i, j = 0;

    for (i = 0; str[i] != '\0'; i++) {
        if (isalpha((unsigned char)str[i]))
            str[j++] = toupper((unsigned char)str[i]);
    }

    str[j] = '\0';
    return str;
}

void walze_inv(char* in, char* out) {
    for(int i = 0; i < 26; i++)
        out[in[i] - 'A'] = i + 'A';
    out[26] = '\0';
}

int mod_26(int b) {
    int temp = b % 26;
    return (temp < 0) ? temp + 26 : temp;
}
#endif