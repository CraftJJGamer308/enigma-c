#include "enigma.h"
#include "utility.h"
#include <stdio.h>
#include <string.h>

int main() {
    //////// Initialisierung ////////
    Enigma e;
    char config[] = "I-IV-II:Beta:Bruno:VAAA:ANJV";
    char sb[] = "AT:BL:DF:GJ:HM:NW:OP:QY:RZ:VX";
    
    switch (enigma_init_from_str(&e, config, sb))
    {
        case 1:
            fprintf(stderr, "Ungültige Konfiguration\n");
            return 1;
            break;
        case 2:
            fprintf(stderr, "Ungültiges Steckbrett\n");
            return 2;
            break;
        default:
        break;
    }
    
    FILE *input_file = fopen("./input.txt", "rb");
    if (!input_file) {
        fprintf(stderr, "Eingabedatei konnte nicht geöffnet werden\n");
        return 1;
    }
    
    //////// Anfangswalzenkonfigurationen ausgeben ////////
    printf("\nAnfangskonfigurationen\n");
    printf("======================\n");
    enigma_print_conf(&e);
    
    printf("\nVer- und Entschlüsselung\n");
    printf("========================\n");

    char text[1024];

    #ifdef SHOW_INTERNAL
    printf("\n  Pos |   SB   W3   W2   W1   GrW  UKW  GrW  W1   W2   W3   SB  ");
    printf("\n-----------------------------------------------------------------");

    int bytes_read;
    while ((bytes_read = fread(text, 1, sizeof(text), input_file)) > 0) {
        text[bytes_read] = '\0';
        clean_string(text);
        for (char *p = text; *p; p++) {
            putchar(enigma_encrypt(&e, *p));
        }
        memset(text, 0, sizeof(text));
    }
    #else
    int char_cnt = 0;
    int bytes_read;
    while ((bytes_read = fread(text, 1, sizeof(text), input_file)) > 0) {
        text[bytes_read] = '\0';
        clean_string(text);

        for (char *p = text; *p; p++) {
            char_cnt++;
            putchar(enigma_encrypt(&e, *p));
            
            if (char_cnt % 4 == 0)
                putchar(' ');
            if (char_cnt % 40 == 0)
                putchar('\n');
        }
        memset(text, 0, sizeof(text));
    }
    #endif

    fclose(input_file);

    return 0;
}