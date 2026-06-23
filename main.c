#include <stdio.h>
#include "enigma.h"

int main() {
    char text[] = "Es lebe das Craft JJ Imperium!";
    clean_string(text);

    Enigma e = enigma_init(
        w_VI, w_VII, w_VIII,
        w_Beta,
        w_UKW_Bruno,
        0, 0, 0, 0,   // pos:  W1, W2, W3, GrW
        0, 0, 0, 0    // ring: W1, W2, W3, GrW
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