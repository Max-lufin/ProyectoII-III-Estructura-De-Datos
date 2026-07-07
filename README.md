### Proyecto — Entregas II y III
**Estructuras de Datos · Universidad de Valparaíso · Escuela de Ingeniería Informática**

---
## Dataset "Twitter Friends"
- https://www.kaggle.com/datasets/hwassner/TwitterFriends
## Descripción

Este proyecto es la evolución de nuestro motor de búsqueda e inferencia de red social, desarrollado en lenguaje C. Para estas entregas, reemplazamos las búsquedas secuenciales exhaustivas por estructuras de alto rendimiento:

- Un **Grafo No Dirigido** para modelar las conexiones entre usuarios
- Una **Tabla Hash dinámica** para la recuperación de información en tiempo constante

---

## Estructura del Proyecto

```
proyecto/
├── src/
│   ├── main.c
│   ├── carga.c
│   ├── buscador.c
│   ├── hash.c
│   ├── indice.c
│   ├── cola.c
│   └── liberarMemoria.c
├── include/
│   ├── estructuras.h
│   ├── funciones.h
│   └── cola.h
└── data/
    ├── usuarios.txt
    ├── posts.txt
    ├── amigos.txt
    └── likes.txt
    └── limpiador.py
    └── dataset_recortadoPrincipal.csv
```

| Carpeta | Contenido |
|---|---|
| `/src` | Archivos fuente en C con la lógica del programa |
| `/include` | Cabeceras con structs y prototipos de funciones |
| `/data` | Archivos de texto normalizados que alimentan el programa |

---

## Instrucciones de Compilación y Ejecución

Ubícate dentro de la carpeta `/src` y ejecuta:

```bash
gcc *.c -o buscador
```

Luego ejecuta el programa con:

```bash
./buscador
```

---

## Arquitectura y Estructuras de Datos

### Grafo No Dirigido
Construido mediante un arreglo de vértices (`Usuario`) conectados por Listas de Adyacencia (`Amigo`). Se evita la duplicidad de aristas mediante la función `yaEsAmigo()` al momento de la carga.

### Tabla Hash — Índice Invertido
Reemplaza la lista enlazada simple de la Entrega I. Utiliza el algoritmo de hashing **djb2** (optimizado con desplazamiento de bits) y resuelve las colisiones mediante **Encadenamiento Separado**.

### Cola FIFO
Estructura auxiliar utilizada para controlar el orden de visita de los nodos en el algoritmo BFS. Las operaciones `encolar` y `desencolar` son ambas `O(1)`.

---

## Funcionalidades y Complejidades Temporales

| Opción | Descripción |
| **1. Información de Usuario** | Busca un perfil y despliega sus amigos directos.
| **2. Búsqueda por Hashtag** | Salta directo al casillero hash de la palabra.
| **3. Red de Contactos (BFS)** | Explora el grafo por niveles (1°, 2° y 3° grado).
| **4. Top-N Hashtags** | Extrae términos de la tabla y los ordena por frecuencia.
| **5. Salir** | Libera toda la memoria de forma ordenada recorriendo listas y casilleros

---

## Parámetros de la Tabla Hash

| Parámetro | Descripción |
|---|---|
| **N** | Cantidad de términos únicos del vocabulario (calculado automáticamente con `descubrirN_Automatico()`) |
| **M** | Primer número primo ≥ 1.5 × N (calculado por `iniciarTablaHash()`) |
| **α (factor de carga)** | α = N / M ≤ 0.67 — garantizado por el dimensionamiento dinámico |

> El tamaño M **no está hardcodeado**. El programa realiza un pre-escaneo del dataset antes de iniciar la carga para calcular N y asignar el M correcto.

---

## Consideraciones Técnicas

### Sello Temporal BFS
Para marcar los nodos visitados y evitar ciclos infinitos, no se resetean variables booleanas recorriendo toda la lista. Se usa un entero global `contador_busqueda_bfs` que sella cada nodo con el número de búsqueda actual, permitiendo reiniciar el estado del grafo entre consultas.

### Gestión de Memoria Segura
El programa no utiliza recolector de basura. Al seleccionar "Salir", se ejecutan funciones destructoras dedicadas que barren la memoria desde las estructuras más profundas hacia la superficie, en este orden:

1. Likes (dentro de cada Post)
2. NodoPosteo (dentro de cada Vocabulario)
3. Posts (lista global)
4. Amigos + Usuarios
5. Tabla Hash (arreglo de punteros + nodos + posteos anidados)

Esto evita estrictamente fugas de memoria (*Memory Leaks*).

### Función Hash djb2
```
hash(0)   = 5381
hash(i)   = hash(i-1) × 33 + c[i]
```
Implementada con desplazamiento de bits: `(hash << 5) + hash + c`, equivalente a `hash × 33 + c`.

---

