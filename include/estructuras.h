#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

//nodo para los calculos
typedef struct NodoTemp{
    char palabra[1000];
    struct NodoTemp* siguiente;
} NodoTemp;

//nodo para la cola de espera del BFS
typedef struct NodoCola{
    long long int id_usuario;
    int grado;
    struct NodoCola* siguiente;
} NodoCola;

//estructura de la cola
typedef struct Cola{
    NodoCola* primero;
    NodoCola* ultimo;
} Cola;

typedef struct NodoAmigo {
    long long int id_amigo;
    struct NodoAmigo *siguiente;
} Amigo;

typedef struct NodoLike{
    long long int id_usuario_like;

    struct NodoLike* siguiente;
} Like;

typedef struct NodoPosteo{
    int id_post_global;
    struct NodoPosteo* siguiente;
} NodoPosteo;

typedef struct Post{
    long long int id_post_global;
    long long int id_usuario_post;
    char tags[1000];
    Like *lista_Like;
    struct Post *siguiente;
  
} Post;

typedef struct NodoUsuario{
    long long int id_usuario;
    char nombre_usuario[1000];
    Post *lista_post;
    Amigo * lista_amigos;
    struct NodoUsuario* siguiente;
    int id_ultima_busqueda; //id para el BFS
    
} Usuario;

//este es el indice invertido
//P.D. con "hashtag", nos referimos a una palabra
typedef struct NodoVocabulario{
    char hashtag[1000];
    int frecuencia; //contador para saber el N
    NodoPosteo* lista_posteo;
    struct NodoVocabulario* siguiente;
} Vocabulario;

//estructura tabla hash
typedef struct tablaHash{ //tabla es un arreglo de punteros 
    int tamano;  //tamaño de la tabla hash
    Vocabulario** tabla; //doble puntero a Vocabulario, para poder crear el arreglo de punteros 
    int total_terminos; //contador de terminos de la tabla
    int largo_maximo_cadena; 
    int cantidad_ocupadas;
    int total_colisiones;
}tablaHash;

#endif
