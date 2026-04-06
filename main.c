#include <stdio.h>
#include <stdlib.h>

#include "estructuras.h"
#include "ficheros.h"
#include "menus.h"

int main() {
<<<<<<< HEAD
    /* Se vuelcan al inicio del juego y son invariantes */
    Objetos *obj = NULL; // Puntero para almacenar la lista de objetos del juego
    Salas *sal = NULL; // Puntero para almacenar la lista de salas del juego
    Conexiones *con = NULL; // Puntero para almacenar la lista de conexiones del juego
    Puzles *puz = NULL; // Puntero para almacenar la lista de puzles del juego
    
    /* Se vuelcan al cargar partida y contienen la informacion del jugador y de las variaciones de las otras estructuras */
    Jugadores *jug = NULL; // Puntero para almacenar la lista de jugadores del juego
    Partida *par = NULL; // Puntero para almacenar la partida del jugador

    int succes = volcado(&sal, &con, &puz, &obj); // Variable para controlar si se han cargado correctamente los datos del juego
    if (succes == 0) { // Si hubo un error al cargar los datos, se muestra un mensaje de error y se termina el programa
        printf("Error al cargar los datos del juego desde los archivos\n");
        return 1;
    }

/* JUGADORES NO ESTÁ INICIALIZADO*/

    // menu_inicial(&jug); // Llama a la función para mostrar el menú inicial y gestionar el acceso de los jugadores

/* PARTIDA NO ESTÁ INICIALIZADA */

    // menu_principal(&par); // Llama a la función para mostrar el menú principal del juego
    
=======
  volcado(NULL, NULL, NULL, NULL, NULL); // Llama a la función para cargar los datos del juego desde los archivos (falta implementar correctamente con punteros)

  menu_inicial(); // Llama a la función para mostrar el menú inicial y gestionar
                  // el acceso de los jugadores
  menu_principal(); // Llama a la función para mostrar el menú principal del
                    // juego y gestionar las acciones del jugador durante la
                    // partida
  menu_partida();   // Llama a la función para mostrar el menú de la partida y
                    // gestionar las acciones específicas dentro de la partida

  return 0;
>>>>>>> 9625f60b9552772341f157a948a035bc9e7c7c45
}