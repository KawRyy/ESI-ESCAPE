#ifndef FICHEROS_H
#define FICHEROS_H

#include "objetos_y_puzzles.h"
#include "usuarios.h"
#include "salas_y_conexiones.h"
#include "menus.h"

/*
 * volcado: carga todos los ficheros de datos del mundo del juego.
 * Retorna 1 si todos los ficheros se cargaron correctamente, 0 si alguno falló.
 */
/* Capacidades: 10 salas, 9 conexiones, 5 objetos, 5 puzles. Tamaño de jugadores
 * dinamico, aumentará a    l registrarse un nuevo jugador. TEMPORAL */
int volcado(Salas **s, Conexiones **c, Puzles **p, Objetos **o, Jugadores **j);

/*
 * cargarPartida: busca 'nickname' en jugadores.txt y carga su estado
 * guardado desde partida.txt en la estructura *par.
 * Retorna 1 si el jugador existe, 0 si no se encontró o hubo error.
 * Los arrays internos de *par (lista_objetos, lista_conexiones,
 * lista_puzles) se reservan con malloc; el caller debe liberarlos.
 */
int cargarPartida(Partida *par, char *nickname);

/*
 * guardarPartida: escribe el estado actual de *par (y el inventario
 * de *jug si no es NULL) en partida.txt, sobreescribiendo la entrada
 * anterior del mismo jugador.
 */
void guardarPartida(Partida *par, Jugadores *jug);

#endif /* FICHEROS_H */