#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ficheros.h"
#include "mapa.h"    
#include "partida.h" 

void ExaminarSalidas(Partida *par);
void accionMover(Partida *par, Salas *sal);

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
