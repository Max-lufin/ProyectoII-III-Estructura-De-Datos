#ifndef COLA_H
#define COLA_H
#include "estructuras.h"

Cola* crearCola();
void encolar(Cola* c, long long int id, int grado);
NodoCola* desencolar(Cola* c);
int colaVacia(Cola* c);
#endif