#ifndef UTILITY_H
#define UTILITY_H

void walze_inv(char* in, char* out) {
    for(int i = 0; i < 26; i++)   // int statt char, 0..25
        out[in[i] - 'A'] = i + 'A';
    out[26] = '\0';                // Null-Terminator explizit setzen
}

int mod_26(int b) {
    int temp = b % 26;
    return (temp < 0) ? temp + 26 : temp;
}

static inline void inc_mod_26(int* a) {
    *a = (*a == 25) ? 0 : *a + 1;
}

#endif