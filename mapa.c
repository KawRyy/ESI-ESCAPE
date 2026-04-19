#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "mapa.h"

//Modulo que se encarga de gestionar las acciones relacionadas con el mapa, como examinar las salidas de la sala actual o moverse a otra sala, gestionando las salas y las conexiones

void ExaminarSalidas(Conexiones *con, int num_conexiones, int id_sala_actual, Salas *sal) { 
    // Función para examinar las salidas disponibles desde la sala actual del jugador
    // Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de examinar salidas en el menú de acciones del jugador
    // Postcondición: Se muestra la descripción de las conexiones que parten de la sala actual, indicando si están abiertas o bloqueadas.

    int salidas_encontradas = 0; 

    for(int i = 0; i < num_conexiones; i++) {
        // Comprobamos si la sala actual está en cualquiera de los extremos de la conexión
        if(con[i].id_sala_orig == id_sala_actual || con[i].id_sala_dest == id_sala_actual) { 
            
            salidas_encontradas = 1;

            // Determinamos cuál es la sala objetivo a la que mira esta conexión
            int sala_conectada;
            if(con[i].id_sala_orig == id_sala_actual) {
                sala_conectada = con[i].id_sala_dest; 
            } else {
                sala_conectada = con[i].id_sala_orig; 
            }

            // Imprimimos el estado hacia la sala conectada, incluyendo su nombre
            // Usamos %s para imprimir sal[sala_conectada].nombre_sala
            if(con[i].estado_conexion == 1) { 
                printf("Hacia sala %d - %s (abierta)\n", sala_conectada, sal[sala_conectada].nombre_sala);
            } else { 
                printf("Hacia sala %d - %s (bloqueada)\n", sala_conectada, sal[sala_conectada].nombre_sala);
            }
        }
    }  

    // Si el bucle termina y no encontró ninguna conexión asociada a la sala actual
    if (salidas_encontradas == 0) {
        printf("No parece haber ninguna salida desde esta sala.\n");
    }
}

void AccionMover(Conexiones *con, int num_conexiones, int *id_sala_actual, Salas *sal) {
    // Función para moverse a otra sala desde la sala actual del jugador
    // Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de entrar a otra sala en el menú de acciones del jugador
    // Postcondición: Se pregunta al jugador por cada conexión que parte de la sala actual si desea entrar a la sala conectada. Si el jugador decide entrar a una sala conectada, se cambia la sala actual a esa sala y se muestra un mensaje indicando el cambio de sala. Si el jugador decide no entrar a ninguna de las salas conectadas no se modifica su posicion.
   
    char respuesta;
    int encontrado = 0;

    for (int i = 0; i < num_conexiones; i++) {
        // 1. Comprobamos si la sala actual es parte de esta conexión (ya sea origen o destino)
        if (con[i].estado_conexion == 1 && 
           (con[i].id_sala_orig == *id_sala_actual || con[i].id_sala_dest == *id_sala_actual)) {
            
            encontrado = 1;

            // 2. Lógica de intercambio: Si mi sala es el origen, voy al destino. 
            // Si mi sala es el destino, voy al origen.
            int destino;
            if (*id_sala_actual == con[i].id_sala_orig) {
                destino = con[i].id_sala_dest;
            } else {
                destino = con[i].id_sala_orig;
            }

            printf("\n>>> ¿Quieres entrar a: %s? (S/N)\n", sal[destino - 1].nombre_sala);
            printf("---- DESCRIPCION ----\n%s\n", sal[destino - 1].descripcion_sala);

            if (scanf(" %c", &respuesta) != 1) {
                while (getchar() != '\n'); 
            }

            if (respuesta == 'S' || respuesta == 's') {
                *id_sala_actual = destino; 
                printf("Te has movido a la sala: %s\n", sal[destino - 1].nombre_sala);
                return; // IMPORTANTE: Salir de la función al moverte para no procesar más conexiones en el mismo turno
            } else if (respuesta == 'N' || respuesta == 'n') {
                printf("Decides no entrar.\n");
            } else {
                printf("Respuesta no valida.\n");
                i--; 
            }
        }
    }

    if (encontrado != 1) {
        printf("No hay conexiones abiertas desde esta sala.\n");
    }
}


void DescribirSala(Salas *sal, int id_sala_actual) {
    // Precondición: Datos cargados e id_sala_actual válido.
    // Postcondición: Muestra información y espera a que el usuario decida salir.
    
    int salir = 0;
    int opcion;

        system("cls"); // Limpiamos pantalla para que solo se vea la descripción
        printf("==========================================\n");
        printf("ESTAS EN: %s\n", sal[id_sala_actual - 1].nombre_sala);
        printf("==========================================\n");
        printf("%s\n\n", sal[id_sala_actual - 1].descripcion_sala);

        printf("Pulse (1) para volver al menu de acciones: ");
    
    do {
        // Comprobación de seguridad del scanf
        if (scanf("%d", &opcion) != 1) {
            printf("Entrada no valida. Por favor, pulse 1.\n");
            while (getchar() != '\n');
            Sleep(1000);
        } else {
            if (opcion == 1) {
                salir = 1;
            } else {
                printf("Pulse 1 para salir.\n");
                Sleep(800);
            }
        }
    } while (salir == 0);
}

int ComprobarConexion(Conexiones *con, int k,int id_sala_actual, char *n){
//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de usar objetos o resolver puzles en el menú de acciones del jugador
//Postcondición: Se comprueba si la conexión con índice k en la lista de conexiones tiene como sala de origen la sala actual, tiene el estado de conexión a 0 (bloqueada) y tiene como condicionante el id del objeto o puzle que se está evaluando.
    if(con[k].id_sala_orig == id_sala_actual && con[k].estado_conexion == 0 && strcmp(con[k].id_condicionante, n) == 0){
        return 1; // La conexión está bloqueada por un objeto que se encuentra en el inventario del jugador
    }else{
        return 0; // La conexión no está bloqueada por un objeto que se encuentra en el inventario del jugador
    }
}

void AbrirConexion(Conexiones *con, int k){
//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de usar objetos o resolver puzles en el menú de acciones del jugador, y se ha comprobado que la conexión con índice k en la lista de conexiones tiene como sala de origen la sala actual, tiene el estado de conexión a 0 (bloqueada) y tiene como condicionante el id del objeto o puzle que se está evaluando.
//Postcondición: Se cambia el estado de la conexión con índice k en la lista de conexiones a 1 (abierta) y se muestra un mensaje indicando que se ha abierto la conexión hacia la sala de destino de dicha conexión.
     con[k].estado_conexion = 1; // Cambia el estado de la conexión a abierta
     printf("Has abierto la conexion hacia la sala %d\n", con[k].id_sala_dest);
}