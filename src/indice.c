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

tablaHash* iniciarTablaHash(int N){
    int M = N * 1.5;  //se cambia el tamaño a numero primo evitando las colisiones
    tablaHash* tabla = (tablaHash*)malloc(sizeof(tablaHash)); 
    tabla->tamano = M; //guarda el tamaño de la tabla en la variable M
    tabla->tabla = (Vocabulario**)malloc(M * sizeof(Vocabulario*)); //se reserva memoria para un arreglo de listas enlazadas (con doble puntero)
    for (int i = 0; i < M; i++) {
        tabla->tabla[i] = NULL;
    }
    return tabla;

}
void insertarHash(tablaHash* tabla, char* hashtag, int id_post){
    unsigned int indice = hash_djb2(hashtag, tabla->tamano); //llamado a la tabla hash calcula donde debe ir el indice
    Vocabulario* actual = tabla->tabla[indice]; //busca en la tabla de vocabulario 

    while( actual !=NULL){ //busca si el termino existe 
        if(strcmp(actual->hashtag, hashtag) == 0){

            actual->frecuencia++; //si la palabra ya existe, incrementa su contador

            NodoPosteo* nuevo_posteo = (NodoPosteo*)malloc(sizeof(NodoPosteo));
            nuevo_posteo->id_post_global = id_post; //se le asigna id_post_global a id_post
            nuevo_posteo-> siguiente = actual->lista_posteo; //inserta el nodo al inicio de la lista posteo
            actual->lista_posteo = nuevo_posteo;
            return;

        }
        actual = actual->siguiente;
    }
    //si el termino no existe en la tabla lo crea
    Vocabulario* nuevo_vocabulario = (Vocabulario*)malloc(sizeof(Vocabulario));
    strcpy(nuevo_vocabulario->hashtag, hashtag); //copia el hashtag a insertar en la variable hashtag

    nuevo_vocabulario->frecuencia = 1; //si es una palabra nueva, su contador inicia en 1
    
    nuevo_vocabulario->lista_posteo = (NodoPosteo*)malloc(sizeof(NodoPosteo));
    nuevo_vocabulario->lista_posteo->id_post_global = id_post; //se asigna el valor de id_post_global a id_post
    nuevo_vocabulario->lista_posteo->siguiente = NULL; //mueve el nodo de la lista marcando el final
    nuevo_vocabulario->siguiente = tabla->tabla[indice]; //el nuevo vocabulario (nodo) apunta al primer nodo de la tabla
    tabla->tabla[indice] = nuevo_vocabulario; // actualiza la tabla hash evitando colisiones con encadenanamiento separado
    

}
//esta funcion toma todos los contadores de frecuencia, los ordena de mayor a menor y muestra los primeros N resultados(hashtags mas usados)
void mostrarTopN(tablaHash* tabla, int top_n){
    if(tabla == NULL) return;

    Vocabulario** arreglo_temporal = (Vocabulario**)malloc(tabla->tamano *sizeof(Vocabulario*)); //arreglo temporal para meter esos ptr a palabras
    int contador_de_palabras = 0;
    
    for(int i = 0; i < tabla->tamano; i++){ //extraemos todas las palabras de los casilleros de la tabla hash y las metemos al arreglo temporal
        Vocabulario* actual = tabla->tabla[i];
        while(actual != NULL){
            arreglo_temporal[contador_de_palabras] = actual;
            contador_de_palabras++;
            actual == actual->siguiente;
        }
    }
    //aqui nos apoyamos del algoritmo bubble sort, para ordenar de mayor a menor comparando elementos adyacentes, si están en orden incorrecto, se intercambian, y se repite hasta que todo este ordenado
    for(int i = 0; i < contador_de_palabras - 1; i++){ //ordenamos el arreglo de mayor a menor segun su frecuencia
        for(int j = 0; j < contador_de_palabras - i - 1; j++){
            if(arreglo_temporal[j]->frecuencia < arreglo_temporal[j+1]->frecuencia){
                //intercambiamos las posiciones
                Vocabulario* temp = arreglo_temporal[j];
                arreglo_temporal[j] = arreglo_temporal[j+1];
                arreglo_temporal[j+1] = temp;
            }
        }
    }
    
    printf("\n---TOP %d HASHTAGS MAS USADOS---\n", top_n);
    int limite = (top_n < contador_de_palabras) ? top_n: contador_de_palabras;
    for(int i = 0; i < limite; i++){
        printf("%d. $s Aparece %d veces\n", i + 1, arreglo_temporal[i]->hashtag, arreglo_temporal[i]->frecuencia);
    }
    free(arreglo_temporal);
}
