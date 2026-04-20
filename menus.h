#ifndef MENUS_H
#define MENUS_H
// (MÓDULO DE FLUJO DE DATOS)

// AUTOR: Francisco Javier Funes Marín

#include "condiciones.h"
#include "usuarios.h"
#include "mapa.h"
#include "ficheros.h"

//Precondición: Recibe las estructuras base del juego actualizadas con el estado del jugador logueado y la sala actual. El menú de juego es donde se desarrolla la experiencia interactiva del escape room, permitiendo al jugador realizar acciones como describir la sala, examinar objetos, coger y soltar objetos, usar objetos, resolver puzles, guardar la partida o salir al menú principal. A lo largo del menú de juego, las estructuras base del juego se actualizan en función de las acciones del jugador, asegurando que el estado del juego se mantenga coherente y actualizado para la experiencia de juego. Si el jugador alcanza la sala de salida (id_sala_actual == 10), se muestra un mensaje de victoria y se ofrecen opciones para guardar la partida o salir sin guardar.
//Postcondición: El menú de juego permite al jugador interactuar con el mundo del escape room, realizar acciones que afectan el estado del juego y avanzar hacia la condición de victoria. A lo largo del menú de juego, las estructuras base del juego se actualizan en función de las acciones del jugador, asegurando que el estado del juego se mantenga coherente y actualizado para la experiencia de juego. Si el jugador alcanza la sala de salida, se muestra un mensaje de victoria y se ofrecen opciones para guardar la partida o salir sin guardar, permitiendo al jugador finalizar su experiencia de juego de manera satisfactoria. Desde el menú de juego, el jugador puede regresar al menú principal para iniciar una nueva partida o cargar otra partida guardada.
void menu_inicial(Salas **sal, int *num_salas, int *id_sala_actual, Conexiones **con, int *num_conexiones, Puzles **puz, int *num_puzles, Objetos **obj, int *num_objetos, Jugadores **jug, int *num_jugadores, Inventario *inv);

#endif /* MENUS_H */