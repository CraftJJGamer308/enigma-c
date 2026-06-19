#include <stdio.h>
#include <string.h>
#include "walze.h"
#include "utility.h"

int main() {
    char text[] = "Es lebe das Craft JJ Imperium!";
    clean_string(text);

    Walze w1 = {.pos = 0, .ring = 0};
    Walze w2 = {.pos = 0, .ring = 0};
    Walze w3 = {.pos = 0, .ring = 0};
    Walze w_beta = {0};
    Walze ukw = {0};

    strncpy(w1.lut, w_VI, 26);
    strncpy(w2.lut, w_VII, 26);
    strncpy(w3.lut, w_VIII, 26);
    strncpy(w_beta.lut, w_Beta, 26);
    strncpy(ukw.lut, w_UKW_Bruno, 26);

    walze_inv(w1.lut, w1.lut_inv);
    walze_inv(w2.lut, w2.lut_inv);
    walze_inv(w3.lut, w3.lut_inv);
    walze_inv(w_beta.lut, w_beta.lut_inv);

    // Anfangswalzenkonfigurationen ausgeben
    printf("\nAnfangskonfigurationen\n");
    printf("======================\n");

    printf("W1:\t"); print_conf(w1, 0);
    printf("W2:\t"); print_conf(w2, 0);
    printf("W3:\t"); print_conf(w3, 0);
    printf("GrW:\t"); print_conf(w_beta, 0);
    printf("UKW:\t"); print_conf(ukw, 1);

    // Ergebnisse ausgeben
    printf("\nVer- und Entschluesselung\n");
    printf("=========================\n");

    printf("Eingabe:\t%s\n", text);
    printf("Ausgabe:\t");

    char out;
    for (int i = 0; i < strlen(text); i++) {
        update_pos(&w1, &w2, &w3);
        out = text[i] - 'A';

        // Vorwärtspfad
        out = walze_output_vw(w3, out);
        out = walze_output_vw(w2, out);
        out = walze_output_vw(w1, out);
        out = walze_output_vw(w_beta, out);

        // UKW
        out = walze_output_ukw(ukw, out);

        // Rückwärtspfad
        out = walze_output_rw(w_beta, out);
        out = walze_output_rw(w1, out);
        out = walze_output_rw(w2, out);
        out = walze_output_rw(w3, out);

        // Ausgang: Index nach ASCII
        printf("%c", out + 'A');
    }

    return 0;
}