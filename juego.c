#include <stdio.h>
#include <stdlib.h>
#include "estructuras.h"

void buclePartida(){
    int x = 0; //Variable para controlar el bucle del menú
    int opcion; //Variable para almacenar la opción elegida por el usuario
    while(x == 0){
        printf("Menu:\n Sala: %s \n--------------------------------\n 1.Describir Sala\n 2.Examinar (objetos y salidas)\n 3.Entrar en otra sala\n 4.Coger Objeto\n 5.Soltar Objeto\n 6.Inventario\n 7.Usar Objeto\n 8.Resolver puzle / introducir código\n 9.Guardar Partida\n 10.Volver\n--------------------------------\n", Salas.nombre_sala[Partida.id_sala_actual]);
        scanf("%d", &opcion); // Lee la opción elegida por el usuario
        switch(opcion){
            case 1:
                Describir_Sala(Partida.id_sala_actual); // Llama a la función para describir la sala actual
                break;
            case 2:
                //Examinar (objetos y salidas)
                break;
            case 3:
                //Entrar en otra sala
                break;
            case 4:
                //Coger Objeto
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

static void Describir_Sala(int n){ //n es el id de la sala actual
    printf("%s\n", Salas.descripcion_sala[n]); // Imprime la descripción de la sala actual
}

static void Inventario(){
    int i;
    printf("Inventario:\n-------------------------------\n");
    for(i = 0; i < Partida.num_objetos; i++){
        if(Partida.lista_objetos[i].localizacion_objeto == 1){ //  Si el objeto está en el inventario
            printf("%s   ->     %s\n", Partida.lista_objetos[i].nombre_objeto, Partida.lista_objetos[i].descripcion_objeto);
        }
    }
}