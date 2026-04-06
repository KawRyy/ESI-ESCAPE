#include <stdio.h>
#include <stdlib.h>

#include "estructuras.h"
#include "usuarios.h"
#include "juego.h"

void menu_inicial(Jugadores **jug){
    int control = 0;
    do{
        printf("=================================\n");
        printf("    Bienvenido a ESI-ESCAPE\n");
        printf("=================================\n");
        printf("1. Iniciar sesión\n");
        printf("2. Salir\n");
        printf("Seleccione una opción: ");

        scanf("%d", &control);

        switch(control){
        case 1:
/* JUGADORES NO ESTÁ INICIALIZADO || login() RECIBE PARÁMETROS ERRÓNEOS */

        /*  if (login(&jug) == 1){
                printf("Inicio de sesión exitoso... \n");
                break;
            else{
                printf("Inicio de sesión fallido... \n");
                control = 0; // Volver a mostrar el menú
                break;
            };
        */
            break;
        case 2:
            printf("Gracias por jugar. ¡Hasta luego!\n");
            exit(0);
        default:
            printf("Opción no válida. Por favor, seleccione una opción del menú.\n");
    }
    }while(control != 1 && control != 2);
}


void menu_principal(){
    int control = 0;
    
    do{
        printf("=================================\n");
        printf("        Menú Principal\n");
        printf("=================================\n");
        printf("1. Nueva Partida\n");
        printf("2. Cargar Partida\n");
        printf("3. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &control);

        switch(control){
            case 1:
                printf("Iniciando nueva partida...\n");

/* NO EXISTE NINGUNA FUNCIÓN PARA INICIAR NUEVA PARTIDA AÚN */

                // iniciarNuevaPartida(); // Llama a la función para iniciar una nueva partida
                break;
            case 2:
                printf("Cargando partida...\n");
                break;
            case 3:
                printf("Gracias por jugar. ¡Hasta luego!\n");
                exit(0);
            default:
                printf("Opción no válida. Por favor, seleccione una opción del menú.\n");
        }
    }while(control != 1 && control != 2);
    
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