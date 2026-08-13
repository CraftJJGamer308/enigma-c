#ifndef WALZE_H
#define WALZE_H

#include <stdint.h>

// letter_t: interner Datentyp für Buchstaben. A - Z <=> 0 - 25
typedef uint8_t letter_t;

// Walze-Konfigurationstyp
typedef struct {
    char* name;
    char lut[27];
    letter_t kerbe1, kerbe2;
} Walze_conf;

// Walze Datentyp
typedef struct {
    letter_t lut[26];
    letter_t lut_inv[26];
    letter_t pos;
    letter_t ring;
    letter_t kerbe1, kerbe2;
} Walze;

// Enigma Datentyp
typedef struct {
    Walze sb;
    Walze w3;
    Walze w2;
    Walze w1;
    Walze grw;
    Walze ukw;
} Enigma;

// Standard-Walzen
extern const Walze_conf w_I;
extern const Walze_conf w_II;
extern const Walze_conf w_III;
extern const Walze_conf w_IV;
extern const Walze_conf w_V;
extern const Walze_conf w_VI;
extern const Walze_conf w_VII;
extern const Walze_conf w_VIII;

// Umkehrwalzen
extern const Walze_conf w_Bruno;
extern const Walze_conf w_Caesar;

// Griechenwalzen
extern const Walze_conf w_Beta;
extern const Walze_conf w_Gamma;

// Enigma-Initialisierung
void enigma_init(
    Enigma* e,
    Walze_conf* sb,
    const Walze_conf* w3, const Walze_conf* w2, const Walze_conf* w1, 
    const Walze_conf* grw,
    const Walze_conf* ukw,
    char ring_w3, char ring_w2, char ring_w1, char ring_grw,
    char pos_w3,  char pos_w2,  char pos_w1,  char pos_grw
);

void enigma_print_conf(Enigma* e);
char enigma_encrypt(Enigma* e, char in);
int enigma_init_from_str(Enigma* e, char* config, char* sb);
#endif