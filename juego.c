#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "estructuras.h"
#include "ficheros.h"
#include "juego.h"






void ExaminarObjeto(Partida *par){
    for(int i = 0; i < par->num_objetos; i++){
        if(par->lista_objetos[i].localizacion_objeto == par->id_sala_actual){ // Si el objeto está en la sala actual
            printf("%s   ->     %s\n", par->lista_objetos[i].nombre_objeto, par->lista_objetos[i].descripcion_objeto);
        }
    }
}
void ExaminarSalidas(Partida *par){
    for(int i = 0; i < par->num_conexiones; i++){
        if(par->lista_conexiones[i].id_sala_orig == par->id_sala_actual){ // Si la conexión es desde la sala actual 
            if(par->lista_conexiones[i].estado_conexion == 1){ // Si la conexión está abierta
                printf("Hacia sala %d\n", par->lista_conexiones[i].id_sala_dest);
            } else { // Si la conexión está bloqueada
                printf("Hacia sala %d (bloqueada)\n", par->lista_conexiones[i].id_sala_dest);
            }
        }
    }  
}

void accionMover(Partida *par, Salas *sal){ // Función para mover al jugador a otra sala siempre que esté abierta
 
   int j = 0; // Indica si se ha encontrado una conexión abierta desde la sala actual
    for(int i = 0; i < par->num_conexiones; i++){ // Recorre todas las conexiones para encontrar las que parten de la sala actual
        if(par->lista_conexiones[i].id_sala_orig == par->id_sala_actual && par->lista_conexiones[i].estado_conexion == 1){ // Si la conexión es desde la sala actual y está abierta
            printf("%s\n ¿Quieres entrar a la sala %d? (1: Sí, 0: No)\n", sal[par->lista_conexiones[i].id_sala_dest].descripcion_sala, par->lista_conexiones[i].id_sala_dest);
            j = 1;
            int respuesta;
            scanf("%d", &respuesta);
            if(respuesta == 1){
                par->id_sala_actual = par->lista_conexiones[i].id_sala_dest; // Actualiza la sala actual del jugador
                printf("Te has movido a la sala %d\n", par->lista_conexiones[i].id_sala_dest);
            }

            }
            
        }
        if(j == 0){
            printf("No hay conexiones abiertas desde esta sala.\n");
        }
    }


void Inventario(Partida *par){
    for(int i = 0; i < par->num_objetos; i++){
        if(par->lista_objetos[i].localizacion_objeto == 0){ //  Si el objeto está en el inventario
            printf("%s   ->     %s\n", par->lista_objetos[i].nombre_objeto, par->lista_objetos[i].descripcion_objeto);
        }
    }
}


void CogerObjeto(Objetos *obj, Jugadores *jug, Partida *par){
    int i = 0; // Variable para recorrer la lista de objetos
    int j = 0; // Indica si se ha encontrado algún objeto en la sala actual y se ha cogido, para salir del bucle después de coger un objeto
    for(i = 0; i < par->num_objetos && j == 0; i++){
        if(par->lista_objetos[i].localizacion_objeto == par->id_sala_actual){
            printf("%s   ->    ¿Deseas coger este objeto? (1: Sí, 0: No)\n", par->lista_objetos[i].nombre_objeto);
            int respuesta;
            scanf("%d", &respuesta);

            if (respuesta == 1) {
                // 1. Cambiamos la localización a 0 (Inventario)
                par->lista_objetos[i].localizacion_objeto = 0; 
                
                // 2. Incrementamos el contador del jugador
                jug->num_items += 1; 

                // 3. Añadimos el ID al array de strings del jugador
                jug->id_objeto = realloc(jug->id_objeto, jug->num_items * sizeof(char*)); // Redimensionamos el array para añadir un nuevo elemento
                jug->id_objeto[jug->num_items - 1] = malloc(strlen(par->lista_objetos[i].id_objeto) + 1);
                strcpy(jug->id_objeto[jug->num_items - 1], par->lista_objetos[i].id_objeto);

                printf("Has cogido el objeto %s\n", par->lista_objetos[i].nombre_objeto);
                j = 1; ////Variable para recorrer la lista de objetos
            }
        }
    }
}

void SoltarObjeto(Objetos *obj, Jugadores *jug, Partida *par){
    int i = 0; //Variable para recorrer la lista de objetos
    int j = 0; // Indica si se ha encontrado algún objeto en el inventario
    for(i = 0; i < par->num_objetos && j == 0; i++){
        if(par->lista_objetos[i].localizacion_objeto == 0 ){ 
            printf("%s   ->    ¿Deseas soltar este objeto? (1: Sí, 0: No)\n", par->lista_objetos[i].nombre_objeto);
            int respuesta;
            scanf("%d", &respuesta);
            
            if (respuesta == 1) {
                // 1. Cambiamos la localización a la sala actual
                par->lista_objetos[i].localizacion_objeto = par->id_sala_actual; 
                
                // 2. Disminuimos los items del jugador
                jug->num_items -= 1; 
                
                // 3. Buscamos y eliminamos el ID del array del jugador usando un "booleano"
                int encontrado_id = 0; // Variable que actuará como booleano
                
                // Añadimos "&& encontrado_id == 0" a la condición para que actúe como un break
                for(int k = 0; k <= jug->num_items && encontrado_id == 0; k++) {
                    
                    if (jug->id_objeto[k] != NULL && strcmp(jug->id_objeto[k], par->lista_objetos[i].id_objeto) == 0) {
                        free(jug->id_objeto[k]);
                        
                        // Mover el último elemento al hueco que queda (si no es el último)
                        if (k < jug->num_items) {
                            jug->id_objeto[k] = jug->id_objeto[jug->num_items];
                        }
                        
                        // Reducir el array
                        if (jug->num_items > 0) {
                            jug->id_objeto = realloc(jug->id_objeto, jug->num_items * sizeof(char*));
                        } else {
                            free(jug->id_objeto);
                            jug->id_objeto = NULL;
                        }
                        
                        // Cambiamos el valor a 1 para que en la próxima iteración la condición del for falle y salga del bucle
                        encontrado_id = 1; 
                    }
                }

                printf("Has soltado el objeto %s\n", par->lista_objetos[i].nombre_objeto);
                j = 1; //  Indica que se ha soltado un objeto y se sale del bucle
            } 
        }
    }
}

void UsarObjeto(Objetos *obj, Conexiones *con, Partida *par){
    // Función para usar un objeto del inventario del jugador
    int i = 0; // Variable para recorrer la lista de objetos
    int j = 0; // Indica si se ha encontrado algún objeto en el inventario
     for(i = 0; i < par->num_objetos && j == 0; i++){
        if(par->lista_objetos[i].localizacion_objeto == 0){ // Detecta los objetos que están en el inventario
            printf("%s   ->    ¿Deseas usar este objeto? (1: Sí, 0: No)\n", par->lista_objetos[i].nombre_objeto);
            int respuesta;
            scanf("%d", &respuesta);

            if(respuesta == 1){
                   if(con->id_sala_orig == par->id_sala_actual && con->condicion_conexion == 1 /* && strcmp(con->id_objeto, par->lista_objetos[i].id_objeto) == 0 */){ 
                        con->estado_conexion = 1; // Abre la conexión
                        printf("Has usado el objeto %s para abrir la conexión hacia la sala %d\n", par->lista_objetos[i].nombre_objeto, con->id_sala_dest);
                    } else {
                        printf("No se puede usar el objeto %s en esta situación\n", par->lista_objetos[i].nombre_objeto);
                        j = 1; // Indica que se ha usado un objeto y se sale del bucle
                    }
            
            }
        }
    }

}

void ResolverPuzle(Puzles *puz, Partida *par){
    // Función para resolver un puzle de la sala actual
    int i = 0; // Variable para recorrer la lista de puzles
    int j = 0; // Indica si se ha encontrado algún puzle en la sala actual
     for(i = 0; i < par->num_puzles && j == 0; i++){
        if(par->lista_puzles[i].id_sala_puzle == par->id_sala_actual){ // Detecta los puzles que están en la sala actual
            printf("%s   ->    ¿Deseas intentar resolver este puzle? (1: Sí, 0: No)\n", par->lista_puzles[i].descripcion_puzle);
            int respuesta;
            scanf("%d", &respuesta);

            if(respuesta == 1){
                char solucion[51];
                printf("%s:\n", par->lista_puzles[i].descripcion_puzle);
                // Limpiar el buffer de entrada antes de leer la solución, para evitar problemas con scanf y fgets
                while(getchar() != '\n'); // Limpiar buffer
                fgets(solucion, 51, stdin); // Lee la solución introducida por el jugador
                solucion[strcspn(solucion, "\n")] = '\0'; // Elimina el carácter de nueva línea
                if(strcmp(solucion, par->lista_puzles[i].solucion_puzle) == 0){ // Si la solución introducida coincide con la solución del puzle
                    printf("Has resuelto el puzle %s\n", par->lista_puzles[i].descripcion_puzle);
                
                    j = 1; // Indica que se ha resuelto un puzle y se sale del bucle
                } else {
                    printf("La solución introducida no es correcta para el puzle %s\n", par->lista_puzles[i].descripcion_puzle);
                }
            
            }
        }
    }

}
