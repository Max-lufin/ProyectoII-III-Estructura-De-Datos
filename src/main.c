#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/estructuras.h"
#include "../include/funciones.h"
#include "../include/cola.h"

//ptr globales, aquí se guardan las listas de usuarios, posts y vocabulario, para que puedan ser accedidos desde cualquier función. De esto se sostiene todo
Usuario* lista_usuarios = NULL;
Post* lista_posts = NULL;
Vocabulario* lista_vocabulario = NULL;
tablaHash* mi_tabla_hash = NULL;
int main(){
  printf("--------------------------------\n");
  printf("------Iniciando Buscador--------\n");
  printf("--------------------------------\n");

  
  int N_real = descubrirN_Automatico();
  mi_tabla_hash = iniciarTablaHash(N_real); //iniciamos la tabla hash con un N que es la cantidad de palabras unicas 

  cargarUsuarios();
  cargarPosts(); //aqui adentro se usan los casilleros del hash
  cargarAmigos();
  cargarLikes();

  printf("Datos cargados exitosamente.\n");

  int opcion = 0;
    
  while(opcion != 5){

    printf("--------------------------------\n");
    printf("1. Informacion de usuario\n");
    printf("2. Busqueda por hashtag\n");
    printf("3. Buscar en red de contactos\n");
    printf("4. Mostrar TopN hashtags\n");
    printf("5. Salir\n");
    printf("--------------------------------\n");
    printf("Ingrese una opcion: ");

    if(scanf("%d", &opcion) != 1){
      
        printf("Opcion invalida. Por favor ingrese un numero nuevamente.\n");
        break;
    }
        
    switch(opcion){

      case 1:{
        long long int id_buscar;
        int contador_amigos = 0;
        printf("Ingrese el ID del usuario: ");
        scanf("%lld", &id_buscar);
            
        Usuario* usr = buscarUsuario(id_buscar);
        if(usr != NULL){
          printf("---SE ENCONTRO EL PERFIL---\n");
          printf("Nombre:   %s\n", usr->nombre_usuario);
          printf("ID:       %lld\n", usr->id_usuario);
          printf("Amigos:   \n");

          Amigo* amigo_actual = usr->lista_amigos;

          //recorremos la sub-lista de amigos del usuario encontrado
          while(amigo_actual != NULL){
            printf("ID Amigo: %lld\n", amigo_actual->id_amigo);
            amigo_actual = amigo_actual->siguiente;
            contador_amigos++;
          }
          if(contador_amigos ==0){
            printf("No tiene amigos registrados.\n");
          } 
          else{
            printf("Total de amigos: %d\n", contador_amigos);
          }
        }
        else{
          printf("Usuario con ID %lld no encontrado.\n", id_buscar);
        }
        break;
      }
      case 2:{
        char hashtag_buscar[1000];
        printf("Ingrese el termino a buscar\n");
        scanf("%s",hashtag_buscar);

        if(StopWord(hashtag_buscar)){
          printf("Error: La palabra '%s' es una stopword y fue omitida del indice. Ingrese un termino valido\n", hashtag_buscar);
          break;
        }
        unsigned long indice  = hash_djb2(hashtag_buscar, mi_tabla_hash->tamano); //calculamos el indice de nuestro casillero
        Vocabulario* vocabulario_actual = mi_tabla_hash->tabla[indice]; //vamos al casillero exacto
        int encontrados = 0;
        //se busca la palabra en el diccionario
        while(vocabulario_actual != NULL){
          if(strcmp(vocabulario_actual->hashtag, hashtag_buscar) == 0){
            encontrados = 1;
            printf("---SE ENCONTRARON LOS POST---\n" );
            //si existe la palabara, se recorre la lista posteo
            NodoPosteo* posteo_actual = vocabulario_actual->lista_posteo;
            int elementos = 0;
            while(posteo_actual != NULL){
              Post* info_post = buscarPostGlobal(posteo_actual->id_post_global);
              if(info_post != NULL){
                printf("ID Post: %d\n", info_post->id_post_global);
                printf("Texto: %s\n", info_post->tags);
                Like* like_actual = info_post->lista_Like;
                int contador_likes = 0;
                while(like_actual != NULL){
                  contador_likes++;
                  like_actual = like_actual->siguiente;

                }
                printf("Likes: %d\n", contador_likes);


              }
              posteo_actual = posteo_actual->siguiente;
              elementos++;
            }
            printf("Total de post encontrados con el hashtag %s: %d\n", hashtag_buscar, elementos);
            break;
          }
          vocabulario_actual = vocabulario_actual->siguiente;
        }
        if(encontrados ==0){
          printf("No se encontraron post con el hashtag %s\n", hashtag_buscar);
        }
        break;
      }
      case 3:{
        long long int id_origen;
        int lim_grado;
        printf("\n---BUSQUEDA BFS POR GRADOS---\n");
        printf("Ingrese el ID del usuario inicial: ");
        scanf("%lld", &id_origen);
        printf("Ingrese el limite maximo de grados de conexion(ej:3): ");
        scanf("%d", &lim_grado);

        if(lim_grado <= 0){
           printf("Error: El grado debe ser mayor a 0.\n");
        } else{
            buscarRedContactos(id_origen, lim_grado);
        }
        break;
      }
      case 4:{
        int cantidad_top;
        printf("\n---CONSULTAR TopN HASHTAGS---\n");
        printf("Ingrese la cantidad de hashtags a mostrar: ");
        scanf("%d", &cantidad_top);

        if(cantidad_top > 0){
          mostrarTopN(mi_tabla_hash, cantidad_top);
        } else{
          printf("Error: Ingrese un numero mayor a 0.\n");
        }
        
        printf("\n ---METRICAS DE LA TABLA HASH---\n");
        Metricas(mi_tabla_hash);
        break;
      }

    
      case 5:{
        printf("entrando al case 5\n");
        liberarMemoriaPost();
        liberarMemoriaUsuario();
        liberarMemoriaVocabulario(); //con la entrega final esta funcion ya no es necesaria, pero la estructura vive en la tabla hash 
        liberarMemoriatablaHash(mi_tabla_hash);
        printf("Cerrando el programa\n");
        opcion = 5;
        break;
      }
    } 
  }       
  return 0;
} 

