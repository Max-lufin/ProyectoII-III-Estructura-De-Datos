#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/estructuras.h"
#include "../include/funciones.h"
#include "../include/cola.h"


extern Usuario* lista_usuarios;
extern Post* lista_posts;
extern Vocabulario* lista_vocabulario;
static int contador_busqueda_bfs = 0;

//funciones de busqueda

//funcion para encontrar un usuario dado su id
Usuario* buscarUsuario(long long int id_buscado){
    Usuario* actual = lista_usuarios; //nuestro primer usuario cargado
    while(actual != NULL){ //mientras no lleguemos al final de la lista de usuarios
      if(actual->id_usuario == id_buscado) //si el id del user actual coicide con el que buscamos, lo encontramos
      return actual;
      actual = actual->siguiente;
    }
    return NULL; //si llegamos al final sin encontrar nada, devuelve null
}

Post* buscarPostGlobal(int id_buscado){
    Post* actual = lista_posts;
    while(actual != NULL){
      if(actual->id_post_global == id_buscado){
        return actual;
      }
      actual = actual->siguiente;
    }
    return NULL;
}
void buscarRedContactos(long long int id_inicio, int grado_maximo){
    Usuario* origen = buscarUsuario(id_inicio); //para validar que el usuario exista
    if(origen == NULL){
        printf("Error: EL usuario %lld no existe en la red.\n", id_inicio);
        return;
    }
    printf("--- Red De Contactos para el usuario %lld ---\n", id_inicio);

    //iniciamos la estructura del bfs
    contador_busqueda_bfs++;
    Cola* fila = crearCola();

    //nodo cero(origen)
    origen->id_ultima_busqueda = contador_busqueda_bfs;
    encolar(fila, id_inicio, 0);

    int total_encontrados = 0;

    //buscle bfs
    while(!colaVacia(fila)){
        NodoCola* actual = desencolar(fila); //saca al primero de la fila
        long long int id_actual = actual->id_usuario;
        int grado_actual = actual->grado;

        if(grado_actual > 0){ //mostramos la informacion
            printf("<Grado %d> Usuario id: %lld\n", grado_actual, id_actual);
            total_encontrados++;
        }
        if(grado_actual < grado_maximo){ //buscamos mas amigos mientras no hayamos llegado al grado maximo
            Usuario* nodo_usuario = buscarUsuario(id_actual);

            if(nodo_usuario != NULL){
              Amigo* iterar_amigo = nodo_usuario->lista_amigos;

              while(iterar_amigo != NULL){ //recorre todos sus amigos
                Usuario* info_amigo = buscarUsuario(iterar_amigo->id_amigo); //busca el registro entero del amigo

                if(info_amigo != NULL && info_amigo->id_ultima_busqueda != contador_busqueda_bfs){ //si el amigo existe y no fue visitado se marca para que no lo encolen 2 o mas veces
                   info_amigo->id_ultima_busqueda = contador_busqueda_bfs;
                   encolar(fila, iterar_amigo->id_amigo, grado_actual + 1);
                }
                iterar_amigo = iterar_amigo->siguiente; //se va al final de la fila con un grado mayor
              }
            }
        }
        free(actual);
    }
    free(fila);
    printf("--- Total encontrados en la red de contactos: %d ---\n", total_encontrados);
}