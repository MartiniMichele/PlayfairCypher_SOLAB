#include <stdio.h>
#include "FileHandler.h"
#include "Playfair.h"

int main( )
{
    int n_files;
    char *choice;
    char keyfile [251];
    char outputdir [251];
    char message_file [251];

    if ((choice = calloc(1, sizeof(char))) == NULL) {
        printf("Cannot allocate choice, exiting. . .\n");
        exit(EXIT_FAILURE);
    }

    printf("Usare i comandi sotto elencati per scegliere l'operazione desiderata\n");
    printf("Bisogna inserire il path dei rispettivi file\n");
    printf("N.B. e' fondamentale avere i file keyfile e message.txt per codificare e decodificare il messaggio\n");

    printf("1------------>CODIFICA\n");
    printf("2------------>DECODIFICA\n");
    scanf("%s", choice);


        if (strcmp(choice,"1") == 0) {

            strcpy(keyfile, "");
            printf("Inserire path keyfile.\n");
            scanf("%s", keyfile);

            strcpy(outputdir, "");
            printf("Inserire path outputdir.\n");
            scanf("%s", outputdir);

            printf("Inserire il numero dei file da codificare.\n");
            scanf("%d", &n_files);


            for (int i = 0; i < n_files; i++) {

                strcpy(message_file, "");
                printf("Inserire path del file.\n");
                scanf("%s", message_file);

                encode(keyfile, outputdir, i, message_file);
            }

            printf("\nOperazione eseguita correttamente.\n");
        }

        else if (strcmp(choice, "2") == 0) {

            strcpy(keyfile, "");
            printf("Inserire path keyfile.\n");
            scanf("%s", keyfile);

            strcpy(outputdir, "");
            printf("Inserire path outputdir.\n");
            scanf("%s", outputdir);


            printf("Inserire il numero dei file da decodificare.\n");
            scanf("%d", &n_files);


            for (int i = 1; i <= n_files; i++) {
                strcpy(message_file, "");
                printf("Inserire path del file.\n");
                scanf("%s", message_file);

                decode(keyfile, outputdir, i, message_file);
            }

            printf("\nOperazione eseguita correttamente.\n");
        }
        free(choice);
}