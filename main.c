#include <stdio.h>
#include <stdlib.h>
#include "ficheros.h"   
#include "menus.h"       
#include "mapa.h"        
#include "condiciones.h" 
#include "usuarios.h"  

int main() {
    /* Se vuelcan al inicio del juego y son invariantes */
    Objetos *obj = NULL; // Puntero para almacenar la lista de objetos del juego
    int num_obj = 0;
    Salas *sal = NULL; // Puntero para almacenar la lista de salas del juego
    int num_sal = 0;
    Conexiones *con = NULL; // Puntero para almacenar la lista de conexiones del juego
    int num_con = 0;
    Puzles *puz = NULL; // Puntero para almacenar la lista de puzles del juego
    int num_puz = 0;
    
    /* Se vuelcan al cargar partida y contienen la informacion del jugador y de las variaciones de las otras estructuras */
    Jugadores *jug = NULL; // Puntero para almacenar la lista de jugadores del juego
    Partida *par = NULL; // Puntero para almacenar la partida del jugador
    int num_jugadores = 0; // Variable para almacenar el número de jugadores cargados desde el archivo

    int succes = volcado(&sal, &num_sal, &con, &num_con, &puz, &num_puz, &obj, &num_obj, &jug, &num_jugadores); // Variable para controlar si se han cargado correctamente los datos del juego
    if (succes == 0) { // Si hubo un error al cargar los datos, se muestra un mensaje de error y se termina el programa
        printf("Error al cargar los datos del juego desde los archivos\n");
        return 1;
    }

    menu_inicial(&par, &jug, &obj, &sal, &con, &puz, &num_jugadores); // Llama a la función para mostrar el menú inicial e iniciar la ejecución del juego

    return 0; // Termina el programa con éxito
}