#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/estructuras.h"
#include "../include/funciones.h"
#include "../include/cola.h"

extern Usuario* lista_usuarios;
extern Post* lista_posts;
extern Vocabulario* lista_vocabulario;
extern Cola* c;

void liberarMemoriaAmigos(Amigo *lista_amigo){
  while(lista_amigo != NULL){
    Amigo *nodo = lista_amigo;
    lista_amigo = lista_amigo -> siguiente;
    free(nodo);
  }
}
void liberarMemoriaUsuario(){
  Usuario *actual = lista_usuarios;

  while(actual != NULL){
    Usuario *nodo = actual;
    liberarMemoriaAmigos(actual -> lista_amigos);
    
    actual = actual -> siguiente;
    free(nodo);
  }
  lista_usuarios = NULL;
}
void liberarMemoriaLike(Like *lista_likes){
  while(lista_likes != NULL){
    Like *nodo = lista_likes;
    lista_likes = lista_likes -> siguiente;
    free(nodo);
  }
}

void liberarMemoriaPost(){
  Post *actual = lista_posts;

  while (actual != NULL){
    Post *nodo = actual ;
    liberarMemoriaLike(actual->lista_Like); //se llama a la funcion para liberar la memoria de likes ya que esta esta conectada a la lista post

    actual = actual -> siguiente;
    free(nodo);
    
  }
  lista_posts = NULL;
}
void liberarMemoriaVocabulario(){
  Vocabulario *actual = lista_vocabulario;

  while (actual != NULL){
    Vocabulario *nodo = actual;
    actual = actual -> siguiente;
    free(nodo);
  }
  lista_vocabulario = NULL;
}
void liberarMemoriatablaHash(tablaHash *tabla){
  if(tabla-> tabla != NULL){
    free(tabla ->tabla);
  }
  free(tabla);

}