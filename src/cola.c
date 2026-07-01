#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/estructuras.h"
#include "../include/funciones.h"
#include "../include/cola.h"

extern Usuario* lista_usuarios;
extern Post* lista_posts;
extern Vocabulario* lista_vocabulario;

//inicia la estructura de la cola
Cola* crearCola(){
    Cola* c = (Cola*)malloc(sizeof(Cola));
    c->primero = NULL;
    c->ultimo = NULL;
    return c;
}
//
int colaVacia(Cola* c){
    return (c->primero == NULL);
}

void encolar(Cola* c, long long int id, int grado){
    NodoCola* nuevo_nodo = (NodoCola*)malloc(sizeof(NodoCola));
    nuevo_nodo-> id_usuario = id;
     //guarda el id del usuario que se va a encolar
    nuevo_nodo-> grado = grado;
    if(colaVacia(c)){ //verifica si la cola esta vacia, si es asi el nuevo nodo es el primero y ultimo
        c->primero = nuevo_nodo; //asegura que el siguiente nodo sea NULL
    }else{
        c ->ultimo ->siguiente = nuevo_nodo; //enlaza el nodo anterior con el nuevo nodo ingresado
         
    }
    nuevo_nodo->siguiente = NULL;
    c->ultimo = nuevo_nodo;
}
NodoCola* desencolar(Cola* c){
    if(colaVacia(c)){
        return NULL;
    }
    NodoCola* nodo_salida = c->primero;

    c->primero = c->primero->siguiente;

    if(c->primero == NULL){
        c->ultimo = NULL;
    }
    return nodo_salida;
}