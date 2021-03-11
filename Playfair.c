
#include "Playfair.h"


//void generateKeyTable(data_block *data);
int removeSpaces(char** string, int length);
int get_index(char* alfabeto, char key);
void create_matrix();
void flag_key(bool* flag, char* alfabeto, char* key);
int alphabet_to_print(char* alfabeto, bool* flag);
char* standardize_string(char *string, char speciale);
char* encode_string(char* string);
char* decode_string(char* string);
int find_row(char lettera);
int find_column(char lettera);
char same_row_rule(int row, int column);
char same_column_rule(int row, int column);
char general_rule(int row, int column);
char decode_general_rule(int row, int column);
char decode_same_row_rule(int row, int column);
char decode_same_column_rule(int row, int column);
void generateKeyTable(data_block *data);

//matrice generata dalla key
static char keyT[MATRIX_SIZE][MATRIX_SIZE];



void encode(char *keyfile_path, char *outputdir_path, int count, ...) {

    char *message;
    char *standardized_message;
    char *encoded_message;

    data_block *data = get_data(keyfile_path);
    generateKeyTable(data);

    va_list list;
    int i;

    va_start(list, count);

    for (int i = 0; i < count; i++) {

        char *elem_path = va_arg(list, char*);

        if ((message = calloc(10, 10 * sizeof(char))) == NULL){
            printf("Cannot allocate message, exiting. . .\n");
            exit(EXIT_FAILURE);
        }

        read_message(elem_path, &message);
        standardized_message = standardize_string(message, data ->speciale);
        encoded_message = encode_string(standardized_message);
        encode_file(encoded_message, outputdir_path, i);

        free(message);


    }

    va_end(list);
}

void decode(char *keyfile_path, char *outputdir_path, int count, ...) {

    char *encoded_message;
    char *decoded_message;

    data_block *data = get_data(keyfile_path);
    generateKeyTable(data);

    va_list list;
    int i;

    va_start(list, count);

    for (int i = 0; i < count; i++) {

        char *elem_path = va_arg(list, char*);

        if ((encoded_message = calloc(10, 10 * sizeof(char))) == NULL){
            printf("Cannot allocate message, exiting. . .\n");
            exit(EXIT_FAILURE);
        }

        read_encoded_message(elem_path, &encoded_message);
        decoded_message = decode_string(encoded_message);
        decode_file(decoded_message, outputdir_path, i);

        free(encoded_message);
    }

}

/*void decode() {
    char *encoded_message;
    char *decoded_message;

    if ((encoded_message = calloc(10, 10 * sizeof(char))) == NULL){
        printf("Cannot allocate message, exiting. . .\n");
        exit(EXIT_FAILURE);
    }

    data_block *data = get_data("../keyfile");

    read_encoded_message("../outputdir/encrypted_message.pf", &encoded_message);
    generateKeyTable(data);
    decoded_message = decode_string(encoded_message);
    decode_file(decoded_message);
}*/
/*void encode() {
    char *message;
    char *standardized_message;
    char *encoded_message;

    if ((message = calloc(10, 10 * sizeof(char))) == NULL){
        printf("Cannot allocate message, exiting. . .\n");
        exit(EXIT_FAILURE);
    }
    data_block *data = get_data("../keyfile");
    read_message("../message.txt", &message);

    generateKeyTable(data);
    standardized_message = standardize_string(message, data->speciale);
    encoded_message = encode_string(standardized_message);
    encode_file(encoded_message);
    free(message);
}*/



/**
 * Funzione che decripta la stringa usando la matrice della chiave
 * @param string la stringa da decriptare
 * @return la stringa decriptata
 */
char* decode_string(char *string) {

    removeSpaces(&string, strlen(string));

    int no_space_length = strlen(string);
    char decrypted_couple[3];
    char current, next;
    char *decrypted_string;
    int row_current, column_current, row_next, column_next;

    if ((decrypted_string = calloc(2, 2 * sizeof(char))) == NULL) {
        printf("Cannot allocate encrypted_string, exiting. . .\n");
        exit(EXIT_FAILURE);
    }

    decrypted_couple[2] = ' ';
    for (int i = 0; i < no_space_length; i += 2) {
        current = string[i];
        next = string[i + 1];

        row_current = find_row(current);
        column_current = find_column(current);

        row_next = find_row(next);
        column_next = find_column(next);

        if (row_current == row_next) {
            decrypted_couple[0] = decode_same_row_rule(row_current, column_current);
            decrypted_couple[1] = decode_same_row_rule(row_current, column_next);
        }

        else if (column_current == column_next) {
            decrypted_couple [0] = decode_same_column_rule(row_current, column_current);
            decrypted_couple [1] = decode_same_column_rule(row_next, column_current);
        }

        else {
            decrypted_couple [0] = decode_general_rule(row_current, column_next);
            decrypted_couple [1] = decode_general_rule(row_next, column_current);
        }
        decrypted_string = (char*) realloc(decrypted_string, (strlen(decrypted_string) + 3) * sizeof(char));
        if (decrypted_string == NULL) {
            printf("Cannot reallocate decrypted_string, exiting. . .\n");
            exit(EXIT_FAILURE);
        }
        strncat(decrypted_string, decrypted_couple, 3);
    }
    return decrypted_string;
}

char decode_general_rule(int row, int column) {
    return keyT[row][column];
}

char decode_same_row_rule(int row, int column) {
    char tmp;

    if (column == 0) {
        tmp = keyT[row][MATRIX_SIZE - 1];
    }
    else {
        tmp = keyT[row][column - 1];
    }
    return  tmp;
}

char decode_same_column_rule(int row, int column) {
    char tmp;

    if (row == MATRIX_SIZE - 1) {
        tmp = keyT[MATRIX_SIZE - 1][column];
    }
    else {
        tmp = keyT[row - 1][column];
    }
    return tmp;
}

/**
 * Funzione che cripta la stringa utilizzando la matrice
 * @param string stringa da criptare
 * @return la string criptata divisa in coppie di lettere
 */
char* encode_string(char* string) {
    removeSpaces(&string, strlen(string));

    int no_space_length = strlen(string);
    char encrypted_couple [3];
    char  current, next;
    char *encrypted_string;
    int row_current, column_current, row_next, column_next;

    if ((encrypted_string = calloc(2, 2 * sizeof(char))) == NULL) {
        printf("Cannot allocate encrypted_string, exiting. . .\n");
        exit(EXIT_FAILURE);
    }

    encrypted_couple[2] = ' ';
    for (int i = 0; i < no_space_length; i += 2) {
        current = string[i];
        next = string[i + 1];

        row_current = find_row(current);
        column_current = find_column(current);

        row_next = find_row(next);
        column_next = find_column(next);

        if (row_current == row_next) {
            encrypted_couple [0] = same_row_rule(row_current, column_current);
            encrypted_couple [1] = same_row_rule(row_current, column_next);
        }

        else if (column_current == column_next) {

            encrypted_couple[0] = same_column_rule(row_current, column_current);
            encrypted_couple[1] = same_column_rule(row_next, column_current);
        }

        else {
            encrypted_couple[0] = general_rule(row_current, column_next);
            encrypted_couple[1] = general_rule(row_next, column_current);
        }

        encrypted_string = (char *) realloc(encrypted_string, (strlen(encrypted_string) + 3) * sizeof(char));
        if (encrypted_string == NULL) {
            printf("Cannot reallocate encrypted_string, exiting. . .\n");
            exit(EXIT_FAILURE);
        }
        strncat(encrypted_string, encrypted_couple, 3);
    }
    return encrypted_string;
}

char general_rule(int row, int column) {
    return keyT[row][column];
}

char same_column_rule(int row, int column) {
    char tmp;

    if (row == MATRIX_SIZE - 1) {
        tmp = keyT[0][column];
    }
    else {
        tmp = keyT[row + 1][column];
    }
    return tmp;
}

char same_row_rule(int row, int column) {
    char tmp;

    if (column == MATRIX_SIZE - 1) {
        tmp = keyT[row][0];
    }
    else {
        tmp = keyT[row][column + 1];
    }
    return  tmp;
}

int find_row(char lettera) {

    for (int i = 0; i < MATRIX_SIZE; i++) {

        for (int j = 0; j < MATRIX_SIZE; j++) {

            if (keyT[i][j] == lettera) {
                return i;
            }
        }
    }
    return -1;
}

int find_column(char lettera) {

    for (int i = 0; i < MATRIX_SIZE; i++) {

        for (int j = 0; j < MATRIX_SIZE; j++) {

            if (keyT[i][j] == lettera) {
                return j;
            }
        }
    }
    return -1;
}


/**
 * Funzione che divide la stringa in coppie di lettere per il criptaggio del messaggio
 * @param string la stringa da dividere in coppie
 * @param speciale carattere con cui sostituire le lettere doppie
 * @return la stringa delle coppie
 */
char* standardize_string(char *string, char speciale) {

    removeSpaces(&string, strlen(string));

    int no_space_length = strlen(string);
    char  current, next;

    char* encrypted_string;
    char couple [3];

    if ((encrypted_string = calloc(2, 2 * sizeof(char))) == NULL) {
        printf("Cannot allocate encrypted_string, exiting. . .\n");
        exit(EXIT_FAILURE);
    }

    couple[2] = ' ';
    for (int i = 0; i < no_space_length; i++) {

        current = string[i];
        next = i == no_space_length - 1 ? speciale :  string[i + 1];
        couple[0] = current;

        if (current == next) {
            couple[1] = speciale;
        } else {
            couple[1] = next;
            i++;
        }

        encrypted_string = (char *) realloc(encrypted_string, (strlen(encrypted_string) + 2) * sizeof(char));
        if (encrypted_string == NULL) {
            printf("Cannot reallocate encrypted_string, exiting. . .\n");
            exit(EXIT_FAILURE);
        }
        strncat(encrypted_string, couple, 3);

    }
        return encrypted_string;

}

/**
 * Funzione per generare la matrice di criptaggio del messaggio
 * @param data la struct contenente tutti i dati necessari
 */
void generateKeyTable(data_block *data) {
    int  length, k;
    bool flag[LUNGHEZZA_ALFABETO];
    char *key;

    length = strlen(data->key);
    key = data->key;

    //inizializzo la matrice
    create_matrix();

    //inizializzo l'array di flag
    for (int i = 0; i < LUNGHEZZA_ALFABETO ; i++) {
        flag[i] = false;
    }


    //uso il flag per segnare quali lettere appartengono alla key
    flag_key(flag, data->alfabeto, key);

    //Scrivo per prime tutte le lettere della key
    int x = 0;
    int y = 0;
    for (int i = 0; i < length; i++) {

        if (flag[get_index(data->alfabeto, key[i])] == true) {
            keyT[x][y] = key[i];
            flag[get_index(data->alfabeto, key[i])] = false;
            if (++y == MATRIX_SIZE) {
                x++;
                y = 0;
            }
        }
    }

    //ripristino i flag per scrivere le altre lettere non appartenenti alla key
    flag_key(flag, data->alfabeto, key);

    //scrivo in ordine le lettere dell'alfabeto non appartenenti alla key
    for (int i = 0; i < MATRIX_SIZE; i++) {

        for (int j = 0; j <  MATRIX_SIZE; j++) {

            if (keyT[i][j] == 'a') {

                keyT[i][j] = data->alfabeto[alphabet_to_print(data->alfabeto, flag)];
            }
        }
    }
}

int get_index(char* alfabeto, char key) {

    for (int i = 0; i < strlen(alfabeto); i++) {

        if (alfabeto[i] == key) {
            return i;
        }
    }
}

void create_matrix() {

    for (int i = 0; i < MATRIX_SIZE; i++) {

        for (int j = 0; j < MATRIX_SIZE; j++) {

            keyT[i][j] = 'a';
        }
    }
}


void flag_key(bool* flag, char* alfabeto, char* key) {

    for (int i = 0; i < strlen(key); i++) {

            flag[get_index(alfabeto, key[i])] = true;
    }
    flag[LUNGHEZZA_ALFABETO - 1] = false;
}

int alphabet_to_print(char* alfabeto, bool* flag) {

    for (int i = 0; i < strlen(alfabeto); i++) {
        if (flag[i] == false){
            flag[i] = true;
            return i;
        }
    }
}

/**
 * Funzione per rimuovere gli spazi da una stringa,
 * @param string stringa passata a parametro
 * @param length lunghezza della stringa passata
 * @return lunghezza della stringa senza spazi
 */
int removeSpaces(char** string, int length) {

    char *old_string = *string;
    int i, count = 0;

    for (i = 0; i < length; i++) {
        if (old_string[i] == ' ') {
            count++;
        }
    }

    int copy_length = length - count;
    count = 0;
    char *new_string;
    if ((new_string = (char*) calloc(copy_length, copy_length * sizeof(char))) == NULL) {
        printf("Cannot allocate new_string, exiting. . .\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < length; ++i) {
        if (old_string[i] != ' ') {
            new_string[count++] = old_string[i];
        }
    }

    free(*string);

    if ((*string = (char*) calloc(copy_length, copy_length * sizeof(char))) == NULL) {
        printf("Cannot allocate string, exiting. . .\n");
        exit(EXIT_FAILURE);
    }
    strncpy(*string, new_string, strlen(new_string));
    free(new_string);
    return count;
}




