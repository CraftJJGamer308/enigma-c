#include "enigma.h"
#include "utility.h"

///////////// UMRECHNUNG /////////////

inline static letter_t to_letter(char c) {
    return (letter_t) c - 'A';
}
inline static char to_char(letter_t l) {
    return (char) l + 'A';
}

///////////// WALZEN /////////////

const Walze_conf w_I          = { .lut = "EKMFLGDQVZNTOWYHXUSPAIBRCJ", .kerbe1 = 'Q' };
const Walze_conf w_II         = { .lut = "AJDKSIRUXBLHWTMCQGZNPYFVOE", .kerbe1 = 'E' };
const Walze_conf w_III        = { .lut = "BDFHJLCPRTXVZNYEIWGAKMUSQO", .kerbe1 = 'V' };
const Walze_conf w_IV         = { .lut = "ESOVPZJAYQUIRHXLNFTGKDCMWB", .kerbe1 = 'J' };
const Walze_conf w_V          = { .lut = "VZBRGITYUPSDNHLXAWMJQOFECK", .kerbe1 = 'Z' };
const Walze_conf w_VI         = { .lut = "JPGVOUMFYQBENHZRDKASXLICTW", .kerbe1 = 'Z', .kerbe2 = 'M' };
const Walze_conf w_VII 	      = { .lut = "NZJHGRCXMYSWBOUFAIVLPEKQDT", .kerbe1 = 'Z', .kerbe2 = 'M' };
const Walze_conf w_VIII 	  = { .lut = "FKQHTLXOCBJSPDZRAMEWNIUYGV", .kerbe1 = 'Z', .kerbe2 = 'M' };
const Walze_conf w_UKW_Bruno  = { .lut = "ENKQAUYWJICOPBLMDXZVFTHRGS" };
const Walze_conf w_UKW_Caesar = { .lut = "RDOBJNTKVEHMLFCWZAXGYIPSUQ" };
const Walze_conf w_Beta       = { .lut = "LEYJVCNIXWPBQMDRTAKZGFUHOS" };
const Walze_conf w_Gamma      = { .lut = "FSOKANUERHMBTIYCWLQPZXVGJD" };

// Walzen-Initialisierung
static void walze_inv(letter_t* in, letter_t* out) {
    for(int i = 0; i < 26; i++)
        out[in[i]] = i;
}
static Walze walze_init(const Walze_conf* w_conf, char ring, char pos) {
    Walze w;
    for (int i = 0; i < 26; i++) 
        w.lut[i] = to_letter(w_conf->lut[i]);

    walze_inv(w.lut, w.lut_inv);

    w.ring = to_letter(ring);
    w.pos =  to_letter(pos);

    w.kerbe1 = to_letter(w_conf->kerbe1);
    w.kerbe2 = to_letter(w_conf->kerbe2);
    return w;
}

static inline bool in_kerbe(Walze* w) {
    // Kerben-Logik (VI/VII/VIII: Der Übertrag geschieht beim Übergang von Z auf A und von M auf N)
    return w->pos == w->kerbe1 || w->pos == w->kerbe2;
}
static inline void pos_inc(Walze* w) {
    w->pos = (w->pos == 25) ? 0 : w->pos + 1;
}

// Walzen-Ausgang
static inline letter_t walze_output_vw(Walze *w, letter_t in) {
#ifdef SHOW_INTERNAL
    printf("%c -> ", to_char(in));
#endif
    return mod_26(w->lut[mod_26(in + w->pos - w->ring)] - w->pos + w->ring);
}
static inline letter_t walze_output_rw(Walze *w, letter_t in) {
#ifdef SHOW_INTERNAL
    printf("%c -> ", to_char(in));
#endif
    return mod_26(w->lut_inv[mod_26(in + w->pos - w->ring)] - w->pos + w->ring);
}
static inline letter_t walze_output_ukw(Walze *w, letter_t in) {
#ifdef SHOW_INTERNAL
    printf("%c -> ", to_char(in));
#endif
    return w->lut[in];
}

// Walzenkonfiguration ausgeben
static void print_conf(Walze* w) {
    letter_t* p = w->lut;
    for (int i = 0; i < 26; i++) {
        putchar(to_char(*p));
        p++;
    }
  
    printf("\t%c\t", to_char(w->ring));
    printf("%c\t",   to_char(w->pos));
    printf("%c\t",   to_char(w->kerbe1));
    printf("%c\n",   to_char(w->kerbe2));
}

///////////// ENIGMA /////////////

// Enigma-Initialisierung
Enigma enigma_init(
    const Walze_conf* w1,  const Walze_conf* w2,  const Walze_conf* w3,
    const Walze_conf* grw,
    const Walze_conf* ukw,
    char ring_w1, char ring_w2, char ring_w3, char ring_grw,
    char pos_w1,  char pos_w2,  char pos_w3,  char pos_grw
) {
    Enigma e;
    e.w1     = walze_init(w1,  ring_w1,  pos_w1 );
    e.w2     = walze_init(w2,  ring_w2,  pos_w2 );
    e.w3     = walze_init(w3,  ring_w3,  pos_w3 );
    e.grw    = walze_init(grw, ring_grw, pos_grw);
    e.ukw    = walze_init(ukw, 0, 0);
    return e;
}

// Gesamt-Konfiguration ausgeben
void enigma_print_conf(Enigma* e) {
    printf("Walze\tLUT\t\t\t\tRS\tPos\tKerbe1\tKerbe2\n");
    printf("----------------------------------------------------------------------\n");

    printf("W1\t");    print_conf(&e->w1);
    printf("W2\t");    print_conf(&e->w2);
    printf("W3\t");    print_conf(&e->w3);
    printf("GrW\t");   print_conf(&e->grw);
    printf("UKW\t");   print_conf(&e->ukw);
}

// Walzenstellung aktualisieren
static void update_pos(Enigma* e) {
    bool kerbe_w3 = in_kerbe(&e->w3);
    bool kerbe_w2 = in_kerbe(&e->w2);
    
    pos_inc(&e->w3);
    if (kerbe_w3 || kerbe_w2)
        pos_inc(&e->w2);
    if (kerbe_w2)
        pos_inc(&e->w1);
}

// Verschlüsselung
char enigma_encrypt(Enigma* e, char in) {
    update_pos(e);

    letter_t x = to_letter(in);

#ifdef SHOW_INTERNAL
    printf("\n%c %c %c | ", 
        to_char(e->w1.pos), 
        to_char(e->w2.pos), 
        to_char(e->w3.pos)
    );
#endif

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

    return to_char(x);
}