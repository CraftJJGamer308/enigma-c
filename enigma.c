#include <string.h>
#include "enigma.h"
#include "utility.h"

const char w_VI         [27] = "JPGVOUMFYQBENHZRDKASXLICTW";
const char w_VII 	    [27] = "NZJHGRCXMYSWBOUFAIVLPEKQDT";
const char w_VIII 	    [27] = "FKQHTLXOCBJSPDZRAMEWNIUYGV";
const char w_UKW_Bruno  [27] = "ENKQAUYWJICOPBLMDXZVFTHRGS";
const char w_UKW_Caesar [27] = "RDOBJNTKVEHMLFCWZAXGYIPSUQ";
const char w_Beta       [27] = "LEYJVCNIXWPBQMDRTAKZGFUHOS";
const char w_Gamma      [27] = "FSOKANUERHMBTIYCWLQPZXVGJD";

///////////// WALZEN /////////////

// Walzen-Initialisierung
static void walze_inv(letter_t* in, letter_t* out) {
    for(int i = 0; i < 26; i++)
        out[in[i]] = i;
}
static Walze walze_init(const char* lut, int pos, int ring) {
    Walze w;
    for (int i = 0; i < 26; i++) 
        w.lut[i] = (letter_t) lut[i] - 'A';

    walze_inv(w.lut, w.lut_inv);
    w.pos = mod_26(pos);
    w.ring = mod_26(ring);

    return w;
}

// Walzenstellung aktualisieren
static inline int in_notch(Walze* w) {
    // Notch-Logik (VI/VII/VIII: Der Übertrag geschieht beim Übergang von Z auf A und von M auf N)
    return (w->pos+'A' == 'Z' || w->pos+'A' == 'M');
}
static inline void pos_inc(Walze* w) {
    w->pos = (w->pos == 25) ? 0 : w->pos + 1;
}
static void update_pos(Enigma* e) {
    int notch_w3 = in_notch(&e->w3);
    int notch_w2 = in_notch(&e->w2);
    
    pos_inc(&e->w3);
    if (notch_w3 || notch_w2)
        pos_inc(&e->w2);
    if (notch_w2)
        pos_inc(&e->w1);
}

// Walzen-Ausgang
static inline letter_t walze_output_vw(Walze *w, letter_t in) {
    return mod_26(w->lut[mod_26(in + w->pos - w->ring)] - w->pos + w->ring);
}
static inline letter_t walze_output_rw(Walze *w, letter_t in) {
    return mod_26(w->lut_inv[mod_26(in + w->pos - w->ring)] - w->pos + w->ring);
}
static inline letter_t walze_output_ukw(Walze *w, letter_t in) {
    return w->lut[in];
}

// Walzenkonfiguration ausgeben
static void print_conf(Walze* w, int ukw) {
    letter_t* p = w->lut;
    for (int i = 0; i < 26; i++)
        putchar((char) *p++ + 'A');

    if (!ukw) {
        printf(", Pos: %c", (char) w->pos + 'A');
        printf(", RS: %c\n", (char) w->ring + 'A');
    } else {
        printf("\n");
    }
}

///////////// ENIGMA /////////////

// Enigma-Initialisierung
Enigma enigma_init(
    const char* lut_w1,  const char* lut_w2,  const char* lut_w3,
    const char* lut_grw,
    const char* lut_ukw,
    int pos_w1,  int pos_w2,  int pos_w3,  int pos_grw,
    int ring_w1, int ring_w2, int ring_w3, int ring_grw
) {
    Enigma e;
    e.w1     = walze_init(lut_w1,  pos_w1,  ring_w1);
    e.w2     = walze_init(lut_w2,  pos_w2,  ring_w2);
    e.w3     = walze_init(lut_w3,  pos_w3,  ring_w3);
    e.grw    = walze_init(lut_grw, pos_grw, ring_grw);
    e.ukw    = walze_init(lut_ukw, 0, 0);
    return e;
}

// Gesamt-Konfiguration ausgeben
void enigma_print_conf(Enigma* e) {
    printf("W1:\t");    print_conf(&e->w1, 0);
    printf("W2:\t");    print_conf(&e->w2, 0);
    printf("W3:\t");    print_conf(&e->w3, 0);
    printf("GrW:\t");   print_conf(&e->grw, 0);
    printf("UKW:\t");   print_conf(&e->ukw, 1);
}

// Verschlüsselung
char enigma_encrypt(Enigma* e, char in) {
    update_pos(e);

    letter_t x = (letter_t) in - 'A';

    // Vorwärtspfad
    x = walze_output_vw(&e->w3, x);
    x = walze_output_vw(&e->w2, x);
    x = walze_output_vw(&e->w1, x);
    x = walze_output_vw(&e->grw, x);

    // UKW
    x = walze_output_ukw(&e->ukw, x);

    // Rückwärtspfad
    x = walze_output_rw(&e->grw, x);
    x = walze_output_rw(&e->w1, x);
    x = walze_output_rw(&e->w2, x);
    x = walze_output_rw(&e->w3, x);

    return (char) x + 'A';
}