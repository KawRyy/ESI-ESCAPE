#include <stdio.h>
#include <stdlib.h>
#include "estructuras.h"

void buclePartida(Jugadores *jug, Salas *sal, Conexiones *con, Objetos *obj, Puzles *puz){
    

    int x = 0; //Variable para controlar el bucle del menú
    int opcion; //Variable para almacenar la opción elegida por el usuario
    while(x == 0){
        printf("Menu:\n Sala: %s \n--------------------------------\n 1.Describir Sala\n 2.Examinar (objetos y salidas)\n 3.Entrar en otra sala\n 4.Coger Objeto\n 5.Soltar Objeto\n 6.Inventario\n 7.Usar Objeto\n 8.Resolver puzle / introducir código\n 9.Guardar Partida\n 10.Volver\n--------------------------------\n", Salas.nombre_sala[Partida.id_sala_actual]);
        scanf("%d", &opcion); // Lee la opción elegida por el usuario
        switch(opcion){
            case 1:
                Describir_Sala(*par->id_sala_actual); // Llama a la función para describir la sala actual
                break;
            case 2:
                Examinar(*par); // Llama a la función para examinar objetos y salidas en la sala actual
                break;
            case 3:
                accionMover(); // Llama a la función para mover al jugador a otra sala (dirección se determinará según la conexión elegida)
                break;
            case 4:
                CogerObjeto(*obj); // Llama a la función para coger un objeto de la sala actual y añadirlo al inventario del jugador
                break;
            case 5:
                //Soltar Objeto
                break;
            case 6:
                Inventario(); // Llama a la función para mostrar el inventario del jugador
                break;
            case 7:
                //Usar Objeto
                break;
            case 8:
                //Resolver puzle / introducir código  
                break;
            case 9:
                //Guardar Partida
                break;
            case 10:
                x = 1; // Salir del bucle
                break;
            default:
                printf("Opción no válida. Por favor, elige una opción del menú.\n");
        }
    }
}

void Describir_Sala(Partida *par){
    printf("%s\n", Salas.descripcion_sala[par->id_sala_actual]);
}

void Examinar(Partida *par){
    int i;
    printf("Objetos en la sala:\n-------------------------------\n");
    for(i = 0; i < *par->num_objetos; i++){
        if(*par->lista_objetos[i].localizacion_objeto == 0 && *par->lista_objetos[i].id_sala == *par->id_sala_actual){ // Si el objeto está en la sala actual
            printf("%s   ->     %s\n", *par->lista_objetos[i].nombre_objeto, *par->lista_objetos[i].descripcion_objeto);
        }
    }
    
    printf("\nSalidas disponibles:\n-------------------------------\n");
    for(i = 0; i < *par->num_conexiones; i++){
        if(*par->lista_conexiones[i].id_sala_orig == *par->id_sala_actual){ // Si la conexión es desde la sala actual 
            if(*par->lista_conexiones[i].estado_conexion == 1){ // Si la conexión está abierta
                printf("Hacia sala %d\n", *par->lista_conexiones[i].id_sala_dest);
            } else { // Si la conexión está bloqueada
                printf("Hacia sala %d (bloqueada)\n", *par->lista_conexiones[i].id_sala_dest);
            }
        }
    }

    
}

void accionMover(){ // Función para mover al jugador a otra sala siempre que esté abierta
 
   int j = 0; // Indica si se ha encontrado una conexión abierta desde la sala actual
    for(int i = 0; i < *par->num_conexiones; i++){ // Recorre todas las conexiones para encontrar las que parten de la sala actual
        if(*par->lista_conexiones[i].id_sala_orig == *par->id_sala_actual && *par->lista_conexiones[i].estado_conexion == 1){ // Si la conexión es desde la sala actual y está abierta
            printf("%s\n ¿Quieres entrar a la sala %d? (1: Sí, 0: No)\n", *par->salas[*par->lista_conexiones[i].id_sala_dest].descripcion_sala, *par->lista_conexiones[i].id_sala_dest);
            j = 1;
            int respuesta;
            scanf("%d", &respuesta);
            if(respuesta == 1){
                *par->id_sala_actual = *par->lista_conexiones[i].id_sala_dest; // Actualiza la sala actual del jugador
                printf("Te has movido a la sala %d\n", *par->lista_conexiones[i].id_sala_dest);
            }

            }
            
        }
        if(j == 0){
            printf("No hay conexiones abiertas desde esta sala.\n");
        }
    }


void Inventario(){
    int i;
    printf("Inventario:\n-------------------------------\n");
    for(i = 0; i < *par->num_objetos; i++){
        if(*par->lista_objetos[i].localizacion_objeto == 1){ //  Si el objeto está en el inventario
            printf("%s   ->     %s\n", *par->lista_objetos[i].nombre_objeto, *par->lista_objetos[i].descripcion_objeto);
        }
    }
}

void CogerObjeto(Objetos *obj){
    // Función para coger un objeto de la sala actual y añadirlo al inventario del jugador
    int i = 0;
     for(i = 0; i < *par->num_objetos; i++){
        if(*par->lista_objetos[i].localizacion_objeto == 0 && *par->lista_objetos[i].id_sala == *par->id_sala_actual){ // Detecta los objetos que están en la sala actual
             printf("%s   ->    ¿Deseas coger este objeto? (1: Sí, 0: No)\n", *par->lista_objetos[i].nombre_objeto);
             int respuesta;
             scanf("%d", &respuesta);

                if(respuesta == 1){
                    *par->lista_objetos[i].localizacion_objeto = 1; // Cambia la localización del objeto a inventario
                    realloc()
                    printf("Has cogido el objeto %s\n", *par->lista_objetos[i].nombre_objeto);
                }
        }
    }
}