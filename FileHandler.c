

#include "FileHandler.h"

FILE* open_file();
void adjust_message(char **message, int size, char* line);
void read_message(char *filename, char** string);
data_block *create_data(char *filename);



/**
 * Funzione che legge il messagio criptato dal file
 * @param filename nome del file da leggere
 * @param string stringa dove salvare il messaggio
 */
void read_encoded_message(char* filename, char** string) {
    FILE *file;
    char *message = *string;

    if ((file = fopen(filename, "r")) == NULL) {
        fclose(file);
        fprintf(stderr, "Cannot open encrypted file, exiting. . .\n");
        exit(EXIT_FAILURE);
    }

    if (fgets(message, MAX_MESSAGE_LENGTH, file) == NULL){
        fclose(file);
        fprintf(stderr, "Cannot read encrypted message, exiting. . .\n");
        exit(EXIT_FAILURE);
    }

    fclose(file);
}
/**
 * Funzione che crea il file del messaggio decodificato
 * @param decoded_string messaggio decodificato
 */
void decode_file(char *decoded_string, char *path, int msg_id) {
    FILE *file;
    char complete_path[251];
    char result [2];
    sprintf(result, "%d", msg_id);

    strcpy(complete_path, "");
    strcat(complete_path, path);
    strcat(complete_path, "/");
    strcat(complete_path, result);
    strcat(complete_path, "decrypted_message.dec");

    file = fopen(complete_path, "w");
    fputs(decoded_string, file);
    fclose(file);


}
/**
 * Funzione che crea il file del messaggio codificato
 * @param encoded_string messaggio codificato
 */
void encode_file(char *encoded_string, char *path, int msg_id) {
    FILE *file;
    char complete_path[251];
    char result [2];
    sprintf(result, "%d", msg_id);

    strcpy(complete_path, "");
    strcat(complete_path, path);
    strcat(complete_path, "/");
    strcat(complete_path, result);
    strcat(complete_path, "encrypted_message.pf");


    file = fopen(complete_path, "w");
    fputs(encoded_string, file);
    fclose(file);

}

/**
 * Funzione per leggere il messaggio da criptare
 * @param filename il file da cui prendere il messaggio
 * @param string la stringa dove salvare il messaggio
 */
void read_message(char *filename, char** string) {
    FILE *file = open_file(filename);
    char *line = NULL;
    int max_line_length, actual_length;

    max_line_length = MAX_MESSAGE_LENGTH;

    if ((line = (char*) calloc(max_line_length, max_line_length * sizeof(char))) == NULL) {
        printf("Cannot allocate line, exiting. . .\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    while (fgets(line, max_line_length, file) != NULL) {

        actual_length += strcspn(line, "\n");
        adjust_message(string, actual_length, line);

    }
    free(line);
    fclose(file);
}

void adjust_message(char **message, int size, char* line) {

    *message = (char*) realloc(*message, (strlen(*message) + size) * sizeof(char));

    strncpy(*message, line, size);

}

data_block *get_data(char *filename){

    data_block *data = create_data(filename);

    return data;
}

/**
 * Funzione per leggere i dati necessari per criptare il messaggio
 * @param filename file da dove prendere i dati
 * @return il data_block compilato
 */
data_block *create_data(char *filename) {
    data_block *new_data;
    if ( (new_data = malloc(sizeof(data_block))) == NULL) {
        printf("Cannot allocate data_block, exiting. . .\n");
        exit(EXIT_FAILURE);
    }
    char string_mancante [6];
    char string_speciale [6];
    FILE *file = open_file(filename);

    if (fgets(new_data->alfabeto, LUNGHEZZA_ALFABETO, file) == NULL) {
        fclose(file);
        fprintf(stderr, "Cannot read alfabeto, exiting. . .\n");
        exit(EXIT_FAILURE);
    }

    fgetc(file);

    if(fgets(string_mancante, STRING_LENGTH, file) == NULL) {

        fclose(file);
        fprintf(stderr, "Cannot read mancante, exiting. . .\n");
        exit(EXIT_FAILURE);
    }
    else new_data->mancante = string_mancante[0];

    if (fgets(string_speciale, STRING_LENGTH, file) == NULL) {
        fclose(file);
        fprintf(stderr, "Cannot read speciale, exiting. . .\n");
        exit(EXIT_FAILURE);
    }
    else new_data->speciale = string_speciale[0];

    new_data->key = malloc(MAX_KEY_LENGTH * sizeof(char));

    if (fgets(new_data->key, MAX_KEY_LENGTH, file) == NULL) {
        fclose(file);
        fprintf(stderr, "Cannot read key, exiting. . .\n");
        exit(EXIT_FAILURE);
    }

    fclose(file);
    return new_data;
}

FILE* open_file(char *filename) {

    FILE *fp;
    fp = fopen(filename, "r");

    if (fp == NULL){
        fprintf(stderr, "Cannot open %s, exiting. . .\n", filename);
        exit(1);
    }
    else return fp;
}


