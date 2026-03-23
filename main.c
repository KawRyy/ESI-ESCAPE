#include <stdio.h>
#include <stdlib.h>

#include "estructuras.h"
/*#include "ficheros.h"*/ // FALTA
#include "menus.h"

int main() {
    
    // Por ejemplo:
    // cargarSalas("salas.txt");
    // cargarObjetos("objetos.txt");
    // cargarJugadores("jugadores.txt");

    // Luego se llamaría a la función de login para gestionar el acceso de los jugadores.
    // Por ejemplo:
    // login(&jugadores, &número_jugadores);

    // Finalmente, se iniciaría el bucle principal del juego.
    buclePartida();

    return 0;
}