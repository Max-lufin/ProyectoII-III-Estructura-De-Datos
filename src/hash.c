#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/estructuras.h"
#include "../include/funciones.h"
#include "../include/cola.h"

unsigned long hash_djb2(char* palabra, int M) {
    unsigned long hash = 5381; //valor base para la tabla
    int c;

    while ((c = *palabra++)) {
        hash = ((hash << 5) + hash) + c; //calcula hash  * 33 + c
    }

    return hash % M; //devuelve el indice de la tabla
}