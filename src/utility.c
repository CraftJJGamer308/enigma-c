#include "utility.h"
#include <ctype.h>

void clean_string(char* str) {
    char *p, *q;
    for (p = q = str; *p; p++) 
        if (isalpha(*p))
            *(q++) = toupper(*p);

    *q = '\0';
}

inline int mod_26(int b) {
    int temp = b % 26;
    return (temp < 0) ? (temp + 26) : temp;
}