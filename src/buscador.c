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
    Usuario* origen = buscarUsuario(id_inicio);
    if(origen == NULL){
        printf("Error: El usuario %lld no existe en la red.\n", id_inicio);
        return;
    }
    printf("\n--- Red de Contactos para el usuario %lld ---\n", id_inicio);

    // Arreglos para acumular resultados por grado
    // 500 es el tope del dataset; ajusta si usas un dataset mayor
    long long int grado1[500], grado2[500], grado3[500];
    int cnt1 = 0, cnt2 = 0, cnt3 = 0;

    contador_busqueda_bfs++;
    Cola* fila = crearCola();

    origen->id_ultima_busqueda = contador_busqueda_bfs;
    encolar(fila, id_inicio, 0);

    while(!colaVacia(fila)){
        NodoCola* actual = desencolar(fila);
        long long int id_actual = actual->id_usuario;
        int grado_actual = actual->grado;

        // Acumular según grado
        if(grado_actual == 1 && cnt1 < 500) grado1[cnt1++] = id_actual;
        else if(grado_actual == 2 && cnt2 < 500) grado2[cnt2++] = id_actual;
        else if(grado_actual == 3 && cnt3 < 500) grado3[cnt3++] = id_actual;

        // Seguir expandiendo si no llegamos al grado máximo
        if(grado_actual < grado_maximo){
            Usuario* nodo_usuario = buscarUsuario(id_actual);
            if(nodo_usuario != NULL){
                Amigo* iterar_amigo = nodo_usuario->lista_amigos;
                while(iterar_amigo != NULL){
                    Usuario* info_amigo = buscarUsuario(iterar_amigo->id_amigo);
                    if(info_amigo != NULL && info_amigo->id_ultima_busqueda != contador_busqueda_bfs){
                        info_amigo->id_ultima_busqueda = contador_busqueda_bfs;
                        encolar(fila, iterar_amigo->id_amigo, grado_actual + 1);
                    }
                    iterar_amigo = iterar_amigo->siguiente;
                }
            }
        }
        free(actual);
    }
    free(fila);

    // Imprimir resultados agrupados por grado
    printf("\n--- Contactos de 1er Grado (%d encontrados) ---\n", cnt1);
    for(int i = 0; i < cnt1; i++)
        printf("  Usuario ID: %lld\n", grado1[i]);
    if(cnt1 == 0) printf("  (ninguno)\n");

    printf("\n--- Contactos de 2do Grado (%d encontrados) ---\n", cnt2);
    for(int i = 0; i < cnt2; i++)
        printf("  Usuario ID: %lld\n", grado2[i]);
    if(cnt2 == 0) printf("  (ninguno)\n");

    printf("\n--- Contactos de 3er Grado (%d encontrados) ---\n", cnt3);
    for(int i = 0; i < cnt3; i++)
        printf("  Usuario ID: %lld\n", grado3[i]);
    if(cnt3 == 0) printf("  (ninguno)\n");

    printf("\n--- Total encontrados: %d ---\n", cnt1 + cnt2 + cnt3);
}