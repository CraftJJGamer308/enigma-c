#ifndef WALZE_H
#define WALZE_H
#include <stdio.h>
#include "utility.h"

// Walze Datentyp
typedef struct {
    char lut[27];
    char lut_inv[27];
    int pos;
    int ring;
} Walze;

// Walzenkonfigurationen
#define w_VI         "JPGVOUMFYQBENHZRDKASXLICTW"
#define w_VII 	     "NZJHGRCXMYSWBOUFAIVLPEKQDT"
#define w_VIII 	     "FKQHTLXOCBJSPDZRAMEWNIUYGV" 
#define w_UKW_Bruno  "ENKQAUYWJICOPBLMDXZVFTHRGS"
#define w_UKW_Caesar "RDOBJNTKVEHMLFCWZAXGYIPSUQ"
#define w_Beta       "LEYJVCNIXWPBQMDRTAKZGFUHOS"
#define w_Gamma      "FSOKANUERHMBTIYCWLQPZXVGJD"

// Notch-Logik (VI/VII/VIII: Der Übertrag geschieht beim Übergang von Z auf A und von M auf N)
static inline int in_notch(Walze* a) {
    return (a->pos+'A' == 'Z' || a->pos+'A' == 'M');
}

void update_pos(Walze* w1, Walze* w2, Walze* w3) {
    // printf(": %c %c %c\n", w1->pos+'A', w2->pos+'A', w3->pos+'A');
    
    int notch_w3 = in_notch(w3);
    int notch_w2 = in_notch(w2);

    inc_mod_26(&(w3->pos));
    if (notch_w3 || notch_w2)
        inc_mod_26(&(w2->pos));
    if (notch_w2)
        inc_mod_26(&(w1->pos));
}

char walze_output_vw(Walze w, char in) {
    return mod_26(w.lut[mod_26(in + w.pos - w.ring)] - 'A' - w.pos + w.ring);
}
char walze_output_rw(Walze w, char in) {
    return mod_26(w.lut_inv[mod_26(in + w.pos - w.ring)] - 'A' - w.pos + w.ring);
}

char walze_output_ukw(Walze w, char in) {
    return w.lut[in] - 'A';
}


#endif