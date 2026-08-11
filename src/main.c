#include "enigma.h"
#include "utility.h"
#include <stdio.h>

int main() {
    //////// Initialisierung ////////
    Enigma e;
    
    enigma_init(
        &e,
        &w_VI, &w_VII, &w_VIII, // W3, W2, W1
        &w_Beta,                // GrW
        &w_Bruno,               // UKW
        'C', 'D', 'B', 'A',     // ring:       W3, W2, W1, GrW
        'E', 'Q', 'F', 'A'      // init. pos:  W3, W2, W1, GrW
    );
    
    //////// Anfangswalzenkonfigurationen ausgeben ////////
    printf("\nAnfangskonfigurationen\n");
    printf("======================\n");
    enigma_print_conf(&e);
    
    //////// Text-Eingabe ////////
    char text[1024]; // Test: VTVGUBFTJVLRUCMPEAAWABQA
    printf("\nText-Eingabe\n");
    printf("============\n");
    
    printf("Text eingeben:\t");
    fgets(text, 1024, stdin);
    clean_string(text);

    //////// Ergebnis-Ausgabe ////////
    printf("\nVer- und Entschluesselung\n");
    printf("=========================\n");

    printf("Eingabe:\t%s\n", text);
    printf("Ausgabe:\t");

#ifdef SHOW_INTERNAL
    printf("\n  Pos |   W3   W2   W1   GrW  UKW  GrW  W1   W2   W3  ");
    printf("\n------------------------------------------------------");
#endif

    for (char *p = text; *p; p++) {
        putchar(enigma_encrypt(&e, *p));
    }
    putchar('\n');

    return 0;
}