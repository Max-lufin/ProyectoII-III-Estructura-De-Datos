#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/estructuras.h"
#include "../include/funciones.h"
#include "../include/cola.h"

extern Usuario* lista_usuarios;
extern Post* lista_posts;
extern Vocabulario* lista_vocabulario;

//funciones de carga de usuarios

void cargarUsuarios(){
    printf("entrando a usuarios \n");
    FILE *archivo_usuarios = fopen("../data/usuarios.txt", "r");
    if (archivo_usuarios == NULL) {
        printf("Error: No se pudo abrir el archivo usuarios.txt\n");
        return;
    }
    int long long id_leido;
    char nombre_leido[1000];
    int usuarios_cargados = 0;

//como es texto formateado usamos fscanf hasta que termine el archivo
//el "%d;%[^\n]\n" indica que es un entero  luego se lee todo hassta encontrar un punto y coma o un salto de linea.
    while(fscanf(archivo_usuarios, "%lld;%[^\n]\n",&id_leido,nombre_leido) ==2){
        //solicitamos memoria
        Usuario* nuevo_usuario = (Usuario*)malloc(sizeof(Usuario));
        nuevo_usuario->id_usuario = id_leido;
        strcpy(nuevo_usuario->nombre_usuario, nombre_leido);
        nuevo_usuario->id_ultima_busqueda = 0;
        nuevo_usuario->lista_amigos = NULL;
        
        nuevo_usuario->siguiente = lista_usuarios; 
        lista_usuarios = nuevo_usuario;

        usuarios_cargados++;

    }
    fclose(archivo_usuarios);
    printf("Exito: se cargaron los usuarios %d\n",usuarios_cargados);
     
}

void cargarPosts(){
  printf("entrando a post\n");
  FILE *archivo_post = fopen("../data/posts.txt","r");
  if (archivo_post == NULL){
    printf("Error: No se pudo abrir el archivo post.txt\n");
    return;
  }
  int id_global_leido;
  char tags_leida[1000];
  long long int usuario_leido;
  while(fscanf(archivo_post,"%d;%lld;%[^\n]\n",&id_global_leido,&usuario_leido,tags_leida)== 3){
      Post *nuevo_post = (Post*)malloc(sizeof(Post));
      nuevo_post -> id_post_global = id_global_leido; // nuevo post apunta a id global para darle el valor del id leido
      nuevo_post -> id_usuario_post = usuario_leido;  //igual que lo anterior pero en este caso se aplica en id usuario

      strcpy(nuevo_post -> tags,tags_leida); //con la funcion strcpy se guarda el valor de "tags leida" en tags
      nuevo_post -> lista_Like = NULL; 
      nuevo_post -> siguiente = lista_posts;  //se ingresa el dato al inicio de la lista 
      lista_posts = nuevo_post; //ahora la lista empieza en el nuevo post

      char* palabra_token = strtok(tags_leida, " ");//se corta la primera palabra en donde haya un espacio
      while(palabra_token != NULL){//si aun quedan palabras en esa oracion
        //verificamos que no sea una stopword(nuestra lista negra)
        if(!StopWord(palabra_token)){ //el "!" significa "NOT", por lo que si esa palabra NO es una stopword, insertala
            insertarVocabulario(palabra_token, id_global_leido);
        }
        palabra_token = strtok(NULL, " ");//se extrae la sgte palabra de ese string
      } 
  }
  fclose(archivo_post);
  printf("Exito: se cargaron los post\n");
}

void cargarAmigos(){
  printf("entrando a amigos\n");
  FILE *archivo_amigos = fopen("../data/amigos.txt","r");
  if (archivo_amigos == NULL){
    printf("Error: No se pudo abrir el archivo amigos.txt\n");
    return;
  }
  
  long long int id_usuario_leido, id_amigo_leido;
  while(fscanf(archivo_amigos,"%lld;%lld\n",&id_usuario_leido,&id_amigo_leido) ==2){
    //A->B
    Usuario* usuario = buscarUsuario(id_usuario_leido); //se llama a la funcion buscarUsuario
    if(usuario == NULL){
      //printf("usuario no encontrado %lld\n",id_usuario_leido);
      continue;
    }
    Amigo* nuevo_amigo = (Amigo*)malloc(sizeof(Amigo));
    nuevo_amigo-> id_amigo = id_amigo_leido;
    nuevo_amigo -> siguiente = usuario -> lista_amigos; // usuario accede a la lista de amigos
    usuario -> lista_amigos = nuevo_amigo;
    //B->A
    Usuario* usuario_inverso = buscarUsuario(id_amigo_leido); //buscamos al amigo 'B' en la lista global de usuarios
 
    if(usuario_inverso != NULL){ //si el amigo existe, le agregamos el usuario original 'A'
      Amigo* nuevo_amigo_inverso = (Amigo*)malloc(sizeof(Amigo));
      nuevo_amigo_inverso->id_amigo = id_usuario_leido; //guardamos el id de 'A'

      nuevo_amigo_inverso->siguiente = usuario_inverso->lista_amigos; //lo insertamos al inicio de la lista de amigos de 'B'
      usuario_inverso->lista_amigos = nuevo_amigo_inverso;
    }
  }
  fclose(archivo_amigos);
  printf("Exito: se cargaron los amigos\n");
}


void cargarLikes(){
  printf("entrando a likes\n");
    FILE *archivo_likes = fopen("../data/likes.txt","r");
    if (archivo_likes == NULL){
      printf("ERROR: no se pudo abrir el archivo likes.txt\n");
      return; 
    }
    long long int id_post;
    long long int id_amigo;
    int likes_cargados = 0;//contador para saber cuantos likes se cargaron
    //lee linea por linea hasta que el archivo termine
    while(fscanf(archivo_likes, "%lld;%lld\n",&id_post,&id_amigo)==2){
        //primero busca el post al que le dieron like
        Post* actual = lista_posts; //ptr global a la lista de posts, quiere decir que estamos al inicio de todos los post
        while(actual != NULL && actual->id_post_global != id_post){ //recorre la lista mientras el final no sea null y el id no sea el que buscamos
            actual = actual->siguiente; //avanza a la sgte post
        }
        //insertamos el like
        //si el while anterior terminó y el "actual" no es null, quiere decir que encontramos el post al que le dieron like, entonces insertamos el like a su lista de likes
        if(actual != NULL){
            //pedimos memoria del tamaño de un like
            Like* nuevo = (Like*)malloc(sizeof(Like));
            nuevo->id_usuario_like = id_amigo; //el id del amigo que dio like se guarda en el nodo like
            nuevo->siguiente = actual->lista_Like; //enlazamos ese like a la lista de likes del post, es como otra lista que vive dentro del post
            actual->lista_Like = nuevo;
            likes_cargados++;
        }
    }
    fclose(archivo_likes);
    printf("Exito: likes cargados %d\n",likes_cargados);
    
}

//filtro de las stopwords
int StopWord(char* palabra){
  //las stopwords que mas se podrian repetir
  const char* stopwords[] = {"que", "su", "sus", "se", "es", "al", "y", "o", "u", "el", "la", "las", "los", "un", "una", "unos", "unas", "a", "del", "de", "ante", "con", "por", "en"};

  int numero_stopwords = sizeof(stopwords)/sizeof(stopwords[0]);
  for(int i = 0; i < numero_stopwords; i++){
    if(strcmp(palabra, stopwords[i])==0){
      return 1;
    }
  }
  return 0;
}