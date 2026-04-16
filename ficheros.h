#ifndef FICHEROS_H
#define FICHEROS_H

#include "estructuras.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * volcado: carga todos los ficheros de datos del mundo del juego.
 * Retorna 1 si todos los ficheros se cargaron correctamente, 0 si alguno falló.
 */
int volcado(Salas **s, Conexiones **c, Puzles **p, Objetos **o, Jugadores **j, int *jugadores);

void guardarPartida(Jugadores *jug, int *id_sala_actual, Objetos *lista_objetos, Conexiones *lista_conexiones, Puzles *lista_puzles);
int cargarPartida(Jugadores *jug, int *id_sala_actual, Objetos **lista_objetos, Conexiones **lista_conexiones, Puzles **lista_puzles);
void nueva_partida(Jugadores *jugador, int *id_sala_actual, Objetos **lista_objetos, Conexiones **lista_conexiones, Puzles **lista_puzles);

#endif /* FICHEROS_H */
