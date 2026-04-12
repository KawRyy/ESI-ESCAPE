#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "condiciones.h" 
#include "partida.h"     
#include "usuarios.h"  
#include "mapa.h" 

//Módulo que se encarga de gestionar las condiciones de los objetos, conexiones y puzles, así como el inventario del jugador

void ExaminarObjeto(Partida *par){ 
//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de examinar objetos en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de los objetos que se encuentran en la sala actual
   
    for(int i = 0; i < par->num_objetos; i++){
        if(par->lista_objetos[i].localizacion_objeto == par->id_sala_actual){ // Si el objeto está en la sala actual
            printf("%s   ->     %s\n", par->lista_objetos[i].nombre_objeto, par->lista_objetos[i].descripcion_objeto);
        }
    }
}

void Inventario(Partida *par){
//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de ver el inventario en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de los objetos que se encuentran en el inventario del jugador
   
    for(int i = 0; i < par->num_objetos; i++){
        if(par->lista_objetos[i].localizacion_objeto == 0){ //  Si el objeto está en el inventario
            printf("%s   ->     %s\n", par->lista_objetos[i].nombre_objeto, par->lista_objetos[i].descripcion_objeto);
        }
    }
}


void CogerObjeto(Jugadores *jug, Partida *par){
//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de coger objetos en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de los objetos que se encuentran en la sala actual y se pregunta al jugador si desea coger alguno de ellos. 
//Si el jugador decide coger un objeto, este se añade a su inventario y se elimina de la sala.
   
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
                jug->num_objetos += 1; 

                // 3. Añadimos el ID al array de strings del jugador
                jug->id_objetos = realloc(jug->id_objetos, jug->num_objetos * sizeof(char*)); // Redimensionamos el array para añadir un nuevo elemento
                jug->id_objetos[jug->num_objetos - 1] = malloc(strlen(par->lista_objetos[i].id_objeto) + 1);
                strcpy(jug->id_objetos[jug->num_objetos - 1], par->lista_objetos[i].id_objeto);

                printf("Has cogido el objeto %s\n", par->lista_objetos[i].nombre_objeto);
                j = 1; ////Variable para recorrer la lista de objetos
            }
        }
    }
}

void SoltarObjeto(Jugadores *jug, Partida *par){
//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de soltar objetos en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de los objetos que se encuentran en el inventario del jugador y se pregunta si desea soltar alguno de ellos. 
//Si el jugador decide soltar un objeto, este se elimina de su inventario y se añade a la sala actual.
 
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
                jug->num_objetos -= 1; 
                
                // 3. Buscamos y eliminamos el ID del array del jugador usando un "booleano"
                int encontrado_id = 0; // Variable que actuará como booleano
                
                // Añadimos "&& encontrado_id == 0" a la condición para que actúe como un break
                for(int k = 0; k <= jug->num_objetos && encontrado_id == 0; k++) {
                    
                    if (jug->id_objetos[k] != NULL && strcmp(jug->id_objetos[k], par->lista_objetos[i].id_objeto) == 0) {
                        free(jug->id_objetos[k]);
                        
                        // Mover el último elemento al hueco que queda (si no es el último)
                        if (k < jug->num_objetos) {
                            jug->id_objetos[k] = jug->id_objetos[jug->num_objetos];
                        }
                        
                        // Reducir el array
                        if (jug->num_objetos > 0) {
                            jug->id_objetos = realloc(jug->id_objetos, jug->num_objetos * sizeof(char*));
                        } else {
                            free(jug->id_objetos);
                            jug->id_objetos = NULL;
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

void UsarObjeto(Partida *par){
//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de usar objetos en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de los objetos que se encuentran en el inventario del jugador y se pregunta si desea usar alguno de ellos. 
//Si el jugador decide usar un objeto, se comprueba si este puede ser usado en la situación actual (por ejemplo, 
//si puede abrir una conexión o resolver un puzle) y se aplica el efecto correspondiente.
    
    int i = 0; // Variable para recorrer la lista de objetos
    int j = 0; // Indica si se ha encontrado algún objeto en el inventario
     for(i = 0; i < par->num_objetos && j == 0; i++){
        if(par->lista_objetos[i].localizacion_objeto == 0){ // Detecta los objetos que están en el inventario
            printf("%s   ->    ¿Deseas usar este objeto? (1: Sí, 0: No)\n", par->lista_objetos[i].nombre_objeto);
            int respuesta;
            scanf("%d", &respuesta);

            if(respuesta == 1){
                   if(par->lista_conexiones[i].id_sala_orig == par->id_sala_actual && par->lista_conexiones[i].condicion_conexion == 1 /* && strcmp(con->id_objeto, par->lista_objetos[i].id_objeto) == 0 */){ 
                        par->lista_conexiones[i].estado_conexion = 1; // Abre la conexión
                        printf("Has usado el objeto %s para abrir la conexión hacia la sala %d\n", par->lista_objetos[i].nombre_objeto, par->lista_conexiones[i].id_sala_dest);
                    } else {
                        printf("No se puede usar el objeto %s en esta situación\n", par->lista_objetos[i].nombre_objeto);
                        j = 1; // Indica que se ha usado un objeto y se sale del bucle
                    }
            
            }
        }
    }

}

void ResolverPuzle(Partida *par){
//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de resolver puzles en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de los puzles que se encuentran en la sala actual y se pregunta al jugador si desea intentar resolver alguno de ellos.
//Si el jugador decide intentar resolver un puzle, se le pide que introduzca la solución. Si la solución introducida es correcta, se muestra un mensaje de éxito y se marca el puzle como resuelto.

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
