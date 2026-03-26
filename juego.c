#include <stdio.h>
#include <stdlib.h>
#include "estructuras.h"
#include"ficheros.h"
#include "juego.h"
#include "menu.h"

void buclePartida(Jugadores *jug, Salas *sal, Conexiones *con, Objetos *obj, Puzles *puz){
    

    int x = 0; //Variable para controlar el bucle del menú
    int opcion; //Variable para almacenar la opción elegida por el usuario
    while(x == 0){
        printf("Menu:\n Sala: %s \n--------------------------------\n 1.Describir Sala\n 2.Examinar (objetos y salidas)\n 3.Entrar en otra sala\n 4.Coger Objeto\n 5.Soltar Objeto\n 6.Inventario\n 7.Usar Objeto\n 8.Resolver puzle / introducir código\n 9.Guardar Partida\n 10.Volver\n--------------------------------\n", Salas.nombre_sala[Partida.id_sala_actual]);
        scanf("%d", &opcion); // Lee la opción elegida por el usuario
        switch(opcion){
            case 1:
                printf("%s\n", *sal->descripcion_sala[par->id_sala_actual]); //Escribe por pantalla la sala actual y su descripción
                break;
            case 2:
                printf("Objetos en la sala:\n-------------------------------\n");
                ExaminarObjeto(*par); // Llama a la función para examinar objetos de la sala actual
                printf("\nSalidas disponibles:\n-------------------------------\n");
                ExaminarSalidas(*par); // Llama a la función para examinar las salidas disponibles desde la sala actual
                break;
            case 3:
                accionMover(*par); // Llama a la función para mover al jugador a otra sala (dirección se determinará según la conexión elegida)
                break;
            case 4:
                CogerObjeto(*obj, *jug, *par); // Llama a la función para coger un objeto de la sala actual y añadirlo al inventario del jugador
                break;
            case 5:
                SoltarObjeto(*obj, *jug, *par); // Llama a la función para soltar un objeto del inventario del jugador y colocarlo en la sala actual
                break;
            case 6:
              printf("Inventario:\n-------------------------------\n");
                Inventario(*par); // Llama a la función para mostrar el inventario del jugador
                break;
            case 7:
                UsarObjeto(*obj, *con, *par); // Llama a la función para usar un objeto del inventario del jugador, lo que podría afectar a las conexiones o puzles de la sala actual
                break;
            case 8:
                ResolverPuzle(*puz, *par); // Llama a la función para resolver un puzle de la sala actual, lo que podría afectar a las conexiones o objetos disponibles  
                break;
            case 9:
                guardarPartida(*par, *jug, *con);
                break;
            case 10:
                x = 1; // Salir del bucle
                break;
            default:
                printf("Opción no válida. Por favor, elige una opción del menú.\n");
        }
    }
}





void ExaminarObjeto(Partida *par){
    for(int i = 0; i < (*par)->num_objetos; i++){
        if((*par)->lista_objetos[i].localizacion_objeto == 0 && (*par)->lista_objetos[i].id_sala == (*par)->id_sala_actual){ // Si el objeto está en la sala actual
            printf("%s   ->     %s\n", (*par)->lista_objetos[i].nombre_objeto, (*par)º->lista_objetos[i].descripcion_objeto);
        }
    }
}
void ExaminarSalidas(Partida *par){
    for(int i = 0; i < (*par)->num_conexiones; i++){
        if((*par)->lista_conexiones[i].id_sala_orig == (*par)->id_sala_actual){ // Si la conexión es desde la sala actual 
            if((*par)->lista_conexiones[i].estado_conexion == 1){ // Si la conexión está abierta
                printf("Hacia sala %d\n", (*par)->lista_conexiones[i].id_sala_dest);
            } else { // Si la conexión está bloqueada
                printf("Hacia sala %d (bloqueada)\n", (*par)->lista_conexiones[i].id_sala_dest);
            }
        }
    }  
}

void accionMover(Partida *par){ // Función para mover al jugador a otra sala siempre que esté abierta
 
   int j = 0; // Indica si se ha encontrado una conexión abierta desde la sala actual
    for(int i = 0; i < (*par)->num_conexiones; i++){ // Recorre todas las conexiones para encontrar las que parten de la sala actual
        if((*par)->lista_conexiones[i].id_sala_orig == (*par)->id_sala_actual && (*par)->lista_conexiones[i].estado_conexion == 1){ // Si la conexión es desde la sala actual y está abierta
            printf("%s\n ¿Quieres entrar a la sala %d? (1: Sí, 0: No)\n", (*par)->salas[(*par)->lista_conexiones[i].id_sala_dest].descripcion_sala, (*par)->lista_conexiones[i].id_sala_dest);
            j = 1;
            int respuesta;
            scanf("%d", &respuesta);
            if(respuesta == 1){
                (*par)->id_sala_actual = (*par)->lista_conexiones[i].id_sala_dest; // Actualiza la sala actual del jugador
                printf("Te has movido a la sala %d\n", (*par)->lista_conexiones[i].id_sala_dest);
            }

            }
            
        }
        if(j == 0){
            printf("No hay conexiones abiertas desde esta sala.\n");
        }
    }


void Inventario(Partida *par){
    for(int i = 0; i < (*par)->num_objetos; i++){
        if((*par)->lista_objetos[i].localizacion_objeto == 00){ //  Si el objeto está en el inventario
            printf("%s   ->     %s\n", (*par)->lista_objetos[i].nombre_objeto, (*par)->lista_objetos[i].descripcion_objeto);
        }
    }
}

void CogerObjeto(Objetos *obj, Jugadores *jug, Partida *par){
    // Función para coger un objeto de la sala actual y añadirlo al inventario del jugador
    int i = 0; // Variable para recorrer la lista de objetos
    int j = 0; // Indica si se ha encontrado algún objeto en la sala actual
     for(i = 0; i < (*par)->num_objetos && j == 0; i++){
        if((*par)->lista_objetos[i].localizacion_objeto != 00 && (*par)->lista_objetos[i].id_sala == (*par)->id_sala_actual){ // Detecta los objetos que están en la sala actual
             printf("%s   ->    ¿Deseas coger este objeto? (1: Sí, 0: No)\n", (*par)->lista_objetos[i].nombre_objeto);
             int respuesta;
             scanf("%d", &respuesta);

                if(respuesta == 1){
                    (*par)->lista_objetos[i].localizacion_objeto = 00; // Cambia la localización del objeto a inventario
                    (*jug)->num_items += 1; // Incrementa el número de objetos en el inventario del jugador
                    (*par)->lista_objetos = realloc((*par)->lista_objetos, ((*jug)->num_items) * sizeof(Objetos)); // Redimensiona la lista de objetos del jugador para incluir el nuevo objeto
                    (*par)->lista_objetos[(*jug)->num_items - 1] = (*par)->lista_objetos[i]; // Copia el objeto al final de la lista
                    printf("Has cogido el objeto %s\n", (*par)->lista_objetos[i].nombre_objeto);
                    j = 1; // Indica que se ha cogido un objeto y se sale del bucle
                }
        }
    }
}

void SoltarObjeto(Objetos *obj, Jugadores *jug, Partida *par){
    int i = 0; // Variable para recorrer la lista de objetos
    int j = 0; // Indica si se ha encontrado algún objeto en la sala actual
     for(i = 0; i < (*par)->num_objetos && j == 0; i++){
        if((*par)->lista_objetos[i].localizacion_objeto == 00 ){ // Detecta los objetos que están en el inventario
             printf("%s   ->    ¿Deseas soltar este objeto? (1: Sí, 0: No)\n", (*par)->lista_objetos[i].nombre_objeto);
             int respuesta;
             scanf("%d", &respuesta);
            
                if(respuesta == 1){
                    (*par)->lista_objetos[i].localizacion_objeto = (*par)->id_sala_actual; // Indica que se ha soltado el objeto en la sala actual
                    (*jug)->num_items -= 1; // Disminuye el número de objetos en el inventario del jugador
                    (*par)->lista_objetos = realloc((*par)->lista_objetos, ((*jug)->num_items) * sizeof(Objetos)); // Redimensiona la lista de objetos del jugador para incluir el nuevo objeto
                    
                    
                    printf("Has soltado el objeto %s\n", (*par)->lista_objetos[i].nombre_objeto);
                    j = 1; // Indica que se ha soltado un objeto y se sale del bucle
                }
            
            }
        }
    }

void UsarObjeto(Objetos *obj, Conexiones *con, Partida *par){
    // Función para usar un objeto del inventario del jugador
    int i = 0; // Variable para recorrer la lista de objetos
    int j = 0; // Indica si se ha encontrado algún objeto en el inventario
     for(i = 0; i < (*par)->num_objetos && j == 0; i++){
        if((*par)->lista_objetos[i].localizacion_objeto == 00){ // Detecta los objetos que están en el inventario
             printf("%s   ->    ¿Deseas usar este objeto? (1: Sí, 0: No)\n", (*par)->lista_objetos[i].nombre_objeto);
             int respuesta;
             scanf("%d", &respuesta);

                if(respuesta == 1){
                   if((*con)->id_sala_orig == (*par)->id_sala_actual && (*con)->condicion_conexion == 1 && strcmp((*con)->id_objeto, (*par)->lista_objetos[i].id_objeto) == 0){ // Si la conexión es desde la sala actual, requiere un objeto y el objeto coincide con el que se va a usar
                        (*con)->estado_conexion = 1; // Abre la conexión
                        printf("Has usado el objeto %s para abrir la conexión hacia la sala %d\n", (*par)->lista_objetos[i].nombre_objeto, (*con)->id_sala_dest);
                    } else {
                    printf("No se puede usar el objeto %s en esta situación\n", (*par)->lista_objetos[i].nombre_objeto);
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
     for(i = 0; i < (*par)->num_puzles && j == 0; i++){
        if((*par)->lista_puzles[i].id_sala_puzle == (*par)->id_sala_actual){ // Detecta los puzles que están en la sala actual
             printf("%s   ->    ¿Deseas intentar resolver este puzle? (1: Sí, 0: No)\n", (*par)->lista_puzles[i].descripcion_puzle);
             int respuesta;
             scanf("%d", &respuesta);

                if(respuesta == 1){
                    char solucion[51];
                    printf("%s:\n", (*par)->lista_puzles[i].descripcion_puzle);
                    fgets(solucion, 51, stdin); // Lee la solución introducida por el jugador
                    solucion[strcspn(solucion, "\n")] = '\0'; // Elimina el carácter de nueva línea
                    if(strcmp(solucion, (*par)->lista_puzles[i].solucion_puzle) == 0){ // Si la solución introducida coincide con la solución del puzle
                        printf("Has resuelto el puzle %s\n", (*par)->lista_puzles[i].descripcion_puzle);
                        j = 1; // Indica que se ha resuelto un puzle y se sale del bucle
                    } else {
                    printf("La solución introducida no es correcta para el puzle %s\n", (*par)->lista_puzles[i].descripcion_puzle);
                }
            
            }
        }
    }

}

