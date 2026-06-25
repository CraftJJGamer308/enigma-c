#include <stdio.h>
#include "enigma.h"

int main() {
    //////// Initialisierung ////////
    Enigma e = enigma_init(
        &w_VIII, &w_VII, &w_VI,    // W1, W2, W3
        &w_Beta,                // GrW
        &w_UKW_Bruno,           // UKW
        'B', 'D', 'C', 'A',     // ring:       W1, W2, W3, GrW
        'F', 'Q', 'E', 'A'      // init. pos:  W1, W2, W3, GrW
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

    return 0;
}