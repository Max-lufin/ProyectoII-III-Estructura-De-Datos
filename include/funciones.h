//prototipos de funciones
#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "estructuras.h"

//cargar archivos

void cargarUsuarios();
void cargarPosts();
void cargarAmigos();
void cargarLikes();


//fuciones de busqueda

Usuario* buscarUsuario(long long id_buscado);
Post* buscarPostGlobal(int id_buscado);
void buscarRedContactos(long long int id_inicio, int grado_maximo);

//indice invertido
void insertarVocabulario(char tag[], int id_post);

//liberar memoria

void liberarMemoriaAmigos(Amigo *lista_amigo);
void liberarMemoriaUsuario();
void liberarMemoriaLike(Like *lista_like);
void liberarMemoriaPost();
void liberarMemoriaVocabulario();
void liberarMemoriatablaHash(tablaHash *tabla);
//stopword
int StopWord(char* palabra);
//tabla hash
void insertarHash(tablaHash* tabla, char* hastag, int id_post);
void mostrarTopN(tablaHash* tabla, int top_n);
unsigned long hash_djb2(char* palabra, int M);
tablaHash* iniciarTablaHash(int N);
#endif
