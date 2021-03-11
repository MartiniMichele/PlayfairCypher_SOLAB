

#ifndef MICHELEMARTINI104941_FILEHANDLER_H
#define MICHELEMARTINI104941_FILEHANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define LUNGHEZZA_ALFABETO 26
#define STRING_LENGTH 6
#define MAX_KEY_LENGTH 256
#define MAX_MESSAGE_LENGTH 512
typedef struct data {
    char alfabeto [LUNGHEZZA_ALFABETO];
    char mancante;
    char speciale;
    char* key;
} data_block;

data_block *get_data(char *filename);
void encode_file(char *encoded_string, char *path, int msg_id);
void read_message(char *filename, char** string);
void read_encoded_message(char* filename, char** string);
void decode_file(char *decoded_string, char *path, int msg_id);

#endif //MICHELEMARTINI104941_FILEHANDLER_H
