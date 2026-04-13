#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ficheros.h"
#include "mapa.h"    
#include "estructuras.h" 

//Modulo que se encarga de gestionar las acciones relacionadas con el mapa, como examinar las salidas de la sala actual o moverse a otra sala, gestionando las salas y las conexiones

void ExaminarSalidas(Partida *par);
void accionMover(Partida *par, Salas *sal);

void ExaminarSalidas(Partida *par){
//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de examinar salidas en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de las conexiones que parten de la sala actual, indicando si están abiertas o bloqueadas.

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
//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de moverse en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de las conexiones que parten de la sala actual y se pregunta al jugador si desea moverse a alguna de las salas conectadas.

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
        if(j == 0){ // Si no se ha encontrado ninguna conexión abierta desde la sala actual
            printf("No hay conexiones abiertas desde esta sala.\n");
    }
}


void describirSala(Partida *par, Salas *sal){ // Función para describir la sala actual del jugador
//Precondición: Deben haber sido cargados los datos de la partida
//Postcondición: Se muestra la descripción de la sala actual del jugador

    for(int i = 0; i < par->num_conexiones; i++){
        if(par->lista_conexiones[i].id_sala_orig == par->id_sala_actual){ // Si la conexión es desde la sala actual 
            printf("%s\n", sal[par->lista_conexiones[i].id_sala_orig].descripcion_sala); // Muestra la descripción de la sala actual
            break;
        }
    }  
}