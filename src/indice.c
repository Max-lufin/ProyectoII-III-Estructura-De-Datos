#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/estructuras.h"
#include "../include/funciones.h"
#include "../include/cola.h"

extern Usuario* lista_usuarios;
extern Post* lista_posts;
extern Vocabulario* lista_vocabulario;  

//nuestro indice invertido
void insertarVocabulario(char tag[], int id_post){
    Vocabulario *actual = lista_vocabulario; //revisamos el diccionario desde el inicio
    //(caso 1):busca si el hashtag ya existe en nuestro vocabulario, con strcmp comparamos strings, si son iguales devuelve un 0
    while(actual != NULL){
      if(strcmp(actual->hashtag,tag) ==0){
          //si el hashtag ya existe, se inserta el id del post a su lista de posteo, pero antes pedimos memoria para el nuevo nodo de posteo
          NodoPosteo* nuevo_posteo = (NodoPosteo*)malloc(sizeof(NodoPosteo));
          nuevo_posteo->id_post_global = id_post;
          //lo metemos al principio de la lista de posteo con esa palabra
          nuevo_posteo->siguiente = actual->lista_posteo;
          actual->lista_posteo = nuevo_posteo;
          return;
      }
      actual = actual->siguiente; //si no coincide esa palabra, pasamos a la sgte
    }
    //(caso 2):si se llega a terminar el while y no la encuentra, es una palabra nueva, por lo que nos toca pedir memoria para registrarla en el diccionario
    Vocabulario* nuevo_vocabulario = (Vocabulario*)malloc(sizeof(Vocabulario));
    strcpy(nuevo_vocabulario->hashtag, tag); //si el hashtag no existe, creamos un nuevo nodo en el vocabulario, con strcpy copiamos el string
    //al ser una palabra nueva, creamos su primer post/registro
    NodoPosteo* nuevo_posteo = (NodoPosteo*)malloc(sizeof(NodoPosteo));
    nuevo_posteo->id_post_global = id_post;
    nuevo_posteo->siguiente = NULL; //como es el primer posteo de esa palabra, su siguiente es null
    nuevo_vocabulario->lista_posteo = nuevo_posteo;//conectamos ese registro a la palabra recien creada

    //enlazamos esa palabra a la lista global de nuestro diccionario
    nuevo_vocabulario->siguiente = lista_vocabulario;
    lista_vocabulario = nuevo_vocabulario;
}
