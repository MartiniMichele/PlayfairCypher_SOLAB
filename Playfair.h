//
// Created by Michele on 26/01/2021.
//

#ifndef MICHELEMARTINI104941_PLAYFAIR_H
#define MICHELEMARTINI104941_PLAYFAIR_H
#include "FileHandler.h"
#include <stdbool.h>
#include <stdarg.h>

#define MATRIX_SIZE 5
#define FLAG_LENGTH 26

void encode(char *keyfile_path, char *outputdir_path, int count, ...);
void decode(char *keyfile_path, char *outputdir_path, int count, ...);



#endif //MICHELEMARTINI104941_PLAYFAIR_H
