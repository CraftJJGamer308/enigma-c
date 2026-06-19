#ifndef UTILITY_H
#define UTILITY_H

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