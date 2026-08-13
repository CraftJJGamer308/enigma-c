#include "enigma.h"
#include "utility.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

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
const Walze_conf w_Bruno      = { .lut = "ENKQAUYWJICOPBLMDXZVFTHRGS" };
const Walze_conf w_Caesar     = { .lut = "RDOBJNTKVEHMLFCWZAXGYIPSUQ" };
const Walze_conf w_Beta       = { .lut = "LEYJVCNIXWPBQMDRTAKZGFUHOS" };
const Walze_conf w_Gamma      = { .lut = "FSOKANUERHMBTIYCWLQPZXVGJD" };

// Walzen-Initialisierung
static void walze_init(Walze* w, const Walze_conf* w_conf, char ring, char pos) {
    for (int i = 0; i < 26; i++) {
        w->lut[i] = to_letter(w_conf->lut[i]);
        w->lut_inv[w->lut[i]] = i; // Walzen-Invertierung
    }

    w->ring = to_letter(ring);
    w->pos =  to_letter(pos);

    w->kerbe1 = to_letter(w_conf->kerbe1);
    w->kerbe2 = to_letter(w_conf->kerbe2);
}

static inline int in_kerbe(Walze* w) {
    // Kerben-Logik
    return w->pos == w->kerbe1 || w->pos == w->kerbe2;
}
static inline void pos_inc(Walze* w) {
    w->pos = (w->pos == 25) ? 0 : (w->pos + 1);
}

// Walzen-Ausgang
static inline letter_t walze_output_vw(Walze *w, letter_t in) {
#ifdef SHOW_INTERNAL
    printf("%c -> ", to_char(in));
#endif
    int offset = w->pos - w->ring;
    return mod_26(w->lut[mod_26(in + offset)] - offset);
}
static inline letter_t walze_output_rw(Walze *w, letter_t in) {
#ifdef SHOW_INTERNAL
    printf("%c -> ", to_char(in));
#endif
    int offset = w->pos - w->ring;
    return mod_26(w->lut_inv[mod_26(in + offset)] - offset);
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
void enigma_init(
    Enigma* e,
    const Walze_conf* w3, const Walze_conf* w2, const Walze_conf* w1, 
    const Walze_conf* grw,
    const Walze_conf* ukw,
    char ring_w3, char ring_w2, char ring_w1, char ring_grw,
    char pos_w3,  char pos_w2,  char pos_w1,  char pos_grw
) {
    memset(e, 0, sizeof(Enigma)); // Init. mit Defaultwerten

    walze_init(&e->w3,  w3,  ring_w3,  pos_w3 );
    walze_init(&e->w2,  w2,  ring_w2,  pos_w2 );
    walze_init(&e->w1,  w1,  ring_w1,  pos_w1 );
    walze_init(&e->grw, grw, ring_grw, pos_grw);
    walze_init(&e->ukw, ukw, 0, 0);
}

// Gesamt-Konfiguration ausgeben
void enigma_print_conf(Enigma* e) {
    printf("Walze\tLUT\t\t\t\tRS\tPos\tKerbe1\tKerbe2\n");
    printf("----------------------------------------------------------------------\n");

    printf("W3\t");    print_conf(&e->w3);
    printf("W2\t");    print_conf(&e->w2);
    printf("W1\t");    print_conf(&e->w1);
    printf("GrW\t");   print_conf(&e->grw);
    printf("UKW\t");   print_conf(&e->ukw);
}

// Walzenstellung aktualisieren
static void update_pos(Enigma* e) {
    int kerbe_w3 = in_kerbe(&e->w3);
    int kerbe_w2 = in_kerbe(&e->w2);
    
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


/////////////// string args ///////////////

static Walze_conf* walze_conf_lookup(char* name) {
    static const struct {
        const char *name;
        const Walze_conf *walze_conf;
    } table[] = {
        { "I",      &w_I },
        { "II",     &w_II },
        { "III",    &w_III },
        { "IV",     &w_IV },
        { "V",      &w_V },
        { "VI",     &w_VI },
        { "VII",    &w_VII },
        { "VIII",   &w_VIII },
        { "Beta",   &w_Beta },
        { "Gamma",  &w_Gamma },
        { "Bruno",  &w_Bruno },
        { "Caesar", &w_Caesar },
        { NULL, NULL }
    };

    for (int i = 0; table[i].name; i++) {
        if (strcmp(table[i].name, name) == 0) {
            return (Walze_conf*) table[i].walze_conf;
        }
    }

    return NULL;
}

// parses text to enigma_init arguments.
// returns 0 if success, 1 if invalid
int enigma_init_from_str(Enigma* e, char* str) {
    char buf[128];
    strncpy(buf, str, sizeof(buf) - 1);

    char *walze    = strtok(buf, ":");
    char *grw      = strtok(NULL, ":");
    char *ukw      = strtok(NULL, ":");
    char *ring     = strtok(NULL, ":");
    char *pos      = strtok(NULL, ":");

    if (!walze || !ring || !pos || !grw || !ukw)
        return 1;
    if (strlen(ring) != 4 || strlen(pos) != 4) 
        return 1;
    for (int i = 0; i < 4; i++) {
        if (!isalpha(ring[i]) || !isalpha(pos[i]))
            return 1;
    }

    char *w1 = strtok(walze, "-");
    char *w2 = strtok(NULL, "-");
    char *w3 = strtok(NULL, "-");

    if (!w1 || !w2 || !w3) 
        return 1;

    const Walze_conf *w1_conf  = walze_conf_lookup(w1);
    const Walze_conf *w2_conf  = walze_conf_lookup(w2);
    const Walze_conf *w3_conf  = walze_conf_lookup(w3);
    const Walze_conf *grw_conf = walze_conf_lookup(grw);
    const Walze_conf *ukw_conf = walze_conf_lookup(ukw);

    if (!w1_conf || !w2_conf || !w3_conf || !grw_conf || !ukw_conf) 
        return 1;

    char ring_w3  = toupper(ring[0]);
    char ring_w2  = toupper(ring[1]);
    char ring_w1  = toupper(ring[2]);
    char ring_grw = toupper(ring[3]);

    char pos_w3  = toupper(pos[0]);
    char pos_w2  = toupper(pos[1]);
    char pos_w1  = toupper(pos[2]);
    char pos_grw = toupper(pos[3]);

    enigma_init(
        e,
        w3_conf, w2_conf, w1_conf,
        grw_conf,
        ukw_conf,
        ring_w3, ring_w2, ring_w1, ring_grw,
        pos_w3, pos_w2, pos_w1, pos_grw    
    );

    return 0;
}