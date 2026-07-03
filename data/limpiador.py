#id[0],screenName[1],tags[2],avatar[3],followersCount[4],friendsCount[5],lang[6],lastSeen[7],tweetId[8],friends[9] 10 datos se deben separar en archivo txt

import csv
#Rutas de los archivos
archivo = "data/dataset_recortadoPrincipal.csv"  
archivo_usuarios = "data/usuarios.txt"
archivo_posts = "data/posts.txt"
archivo_amigos = "data/amigos.txt"
archivo_likes = "data/likes.txt"

usuarios_set = {} #temporal, para guardar los datos antes de filtrarlos

#leemos el CSV y guardamos los datos limpios en la memoria temporal
with open(archivo, 'r') as archivo_csv:
    lector = csv.reader(archivo_csv, delimiter=',', quotechar='"')  #csv.reader lee fila por fila y las separa 
    next(lector)  #salta la primera fila (encabezados)
    
    for fila in lector:
       if len(fila) < 10: continue
       
       id_usuario = fila[0]
       nombre_usuario = fila[1]
       
       posts_sucios = fila[2]  #limpieza de caracteres no deseados en los posts
       posts_limpios = posts_sucios.replace('[', '').replace(']', '').replace('"', '').strip()  #limpia los posts quitando los caracteres no deseados
       
       amigos_sucios = ",".join(fila[9:])  #limpieza de caracteres no deseados en los amigos
       amigos_limpios = amigos_sucios.replace('[', '').replace(']', '').replace('"', '').replace(' ', '')
       lista_amigos = amigos_limpios.split(',')  #separa los amigos en una lista

       #guardamos todo en el diccionario antes de escribir
       usuarios_set[id_usuario] = {
           'nombre': nombre_usuario,
           'post': posts_limpios,
           'amigos': lista_amigos
       }

#BOLA DE NIEVE
LIMITE = 500
id_semilla = None
max_conexiones = 0

#buscamos el usuario más conectado de nuestro dataset
for uid, datos in usuarios_set.items():
    amigos_validos = [a for a in datos['amigos'] if a in usuarios_set]
    if len(amigos_validos) > max_conexiones:
        max_conexiones = len(amigos_validos)
        id_semilla = uid

if not id_semilla: id_semilla = list(usuarios_set.keys())[0]

visitados = set([id_semilla])
cola = [id_semilla]

#atrapar a sus amigos y a los amigos de sus amigos hasta llegar a 500
while cola and len(visitados) < LIMITE:
    actual = cola.pop(0)
    for amigo in usuarios_set[actual]['amigos']:
        if amigo in usuarios_set and amigo not in visitados:
            visitados.add(amigo)
            cola.append(amigo)
            if len(visitados) >= LIMITE:
                break


with open(archivo_usuarios, 'w') as salida_usuarios, \
     open(archivo_posts, 'w') as salida_posts, \
     open(archivo_amigos, 'w') as salida_amigos, \
     open(archivo_likes, 'w') as salida_likes: #abre los archivos
     
    id_post_global = 1
    
    for id_usuario in visitados: #recorre solamente los 500 seleccionados
        datos = usuarios_set[id_usuario]
        nombre_usuario = datos['nombre']
        posts_limpios = datos['post']
        lista_amigos = datos['amigos']
        
        primer_amigo_valido = ""

        for amigo in lista_amigos: #ciclo para guardar el usuario y el amigo que tenga
            #valida que el amigo también este en los 500
            if amigo != "" and amigo in visitados: 
                salida_amigos.write(f"{id_usuario};{amigo}\n")
                if primer_amigo_valido == "":
                    primer_amigo_valido = amigo
             
        salida_usuarios.write(f"{id_usuario};{nombre_usuario}\n")  #escribe el id y el nombre del usuario en el archivo de usuarios

        salida_posts.write(f"{id_post_global};{id_usuario};{posts_limpios}\n")

        if primer_amigo_valido != "":
           #toma el primer amigo de la lista para asignar el like
           salida_likes.write(f"{id_post_global};{primer_amigo_valido}\n")

        id_post_global += 1

print("\nArchivos separados y generados correctamente.")
print("\n-----------------------------------------------")
print("IDs con mayor cantidad de amigos dentro del ecosistema de 500 usuarios: ")
print("-----------------------------------------------")

#extraemos los mejores candidatos
ranking_ids = []

for uid in visitados:
    #contamos cuantas aristas tiene este usuario dentro del grupo de 500
    amigos_reales = [a for a in usuarios_set[uid]['amigos'] if a in visitados]
    if len(amigos_reales) > 0:
        ranking_ids.append((uid, len(amigos_reales)))

#ordenamos la lista de mayor a menor según la cantidad de amigos
ranking_ids.sort(key=lambda x: x[1], reverse=True)

#imprimimos el top 10
for i, (uid, cantidad) in enumerate(ranking_ids[:10]):
    print(f"Opción {i+1}: Ingresar ID {uid} (Garantiza {cantidad} amigos de 1er grado)")
