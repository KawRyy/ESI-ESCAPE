#include <stdio.h>
#include <stdlib.h>

#include "usuarios.h"
#include "juego.h"

void menu_inicial() {
    int opcion;
    
    printf("=================================\n");
    printf("    Bienvenido a ESI-ESCAPE\n");
    printf("=================================\n");
    printf("1. Iniciar sesión\n");
    printf("2. Salir\n");
    printf("Seleccione una opción: ");

    scanf("%d", &opcion);

    switch(opcion) {
        case 1:
            /* login(); */ // FALTA CREAR LAS ESTRUCTURAS PARÁMETROS
            break;
        case 2:
            printf("Gracias por jugar. ¡Hasta luego!\n");
            exit(0);
        default:
            printf("Opción no válida. Por favor, seleccione una opción del menú.\n");
    }

}

/* void buclePartida(Jugadores *jug, Salas *sal, Conexiones *con, Objetos *obj, Puzles *puz){
    

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
    */