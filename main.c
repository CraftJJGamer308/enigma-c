#include <stdio.h>
#include "enigma.h"

int main() {
    char text[] = "VTVGUBFTJVLRUCMPEAAWABQA";
    clean_string(text);

    Enigma e = enigma_init(
        &w_VIII, &w_VII, &w_VI,    // W1, W2, W3
        &w_Beta,                // GrW
        &w_UKW_Bruno,           // UKW
        'B', 'D', 'C', 'A',     // ring:       W1, W2, W3, GrW
        'F', 'Q', 'E', 'A'      // init. pos:  W1, W2, W3, GrW
    );

    // Anfangswalzenkonfigurationen ausgeben
    printf("\nAnfangskonfigurationen\n");
    printf("======================\n");
    enigma_print_conf(&e);

    // Ergebnisse ausgeben
    printf("\nVer- und Entschluesselung\n");
    printf("=========================\n");

    printf("Eingabe:\t%s\n", text);
    printf("Ausgabe:\t");

    for (char *p = text; *p; p++) {
        putchar(enigma_encrypt(&e, *p));
    }

    return 0;
}