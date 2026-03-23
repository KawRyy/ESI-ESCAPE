#include <stdio.h>
#include <stdlib.h>

#include "estructuras.h"
/*#include "ficheros.h"*/ // FALTA
#include "menus.h"

int main() {
    volcado(NULL, NULL, NULL, NULL); /*Llama a la función para cargar los datos del juego desde los archivos (falta implementar correctamente con punteros)*/

    menu_inicial(); // Llama a la función para mostrar el menú inicial y gestionar el acceso de los jugadores
    menu_principal(); // Llama a la función para mostrar el menú principal del juego y gestionar las acciones del jugador durante la partida
    menu_partida(); // Llama a la función para mostrar el menú de la partida y gestionar las acciones específicas dentro de la partida

    return 0;
}