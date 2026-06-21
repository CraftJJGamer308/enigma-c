#ifndef WALZE_H
#define WALZE_H
#include <stdio.h>
#include <stdint.h>
#include "utility.h"

// letter_t: interner Datentyp für Buchstaben. A - Z <=> 0 - 25
typedef uint8_t letter_t;

// Walze Datentyp
typedef struct {
    letter_t lut[26];
    letter_t lut_inv[26];
    letter_t pos;
    letter_t ring;
} Walze;

// Enigma Datentyp
typedef struct {
    Walze w3;
    Walze w2;
    Walze w1;
    Walze grw;
    Walze ukw;
} Enigma;

// Walzenkonfigurationen
extern const char w_VI         [27];
extern const char w_VII 	   [27];
extern const char w_VIII 	   [27];
extern const char w_UKW_Bruno  [27];
extern const char w_UKW_Caesar [27];
extern const char w_Beta       [27];
extern const char w_Gamma      [27];

// Enigma-Initialisierung
Enigma enigma_init(
    const char* lut_w1,  const char* lut_w2,  const char* lut_w3,
    const char* lut_grw,
    const char* lut_ukw,
    int pos_w1,  int pos_w2,  int pos_w3,  int pos_grw,
    int ring_w1, int ring_w2, int ring_w3, int ring_grw
);

void enigma_print_conf(Enigma* e);
char enigma_encrypt(Enigma* e, char in);

#endif