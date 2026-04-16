#ifndef GESTION_H
#define GESTION_H

#include "estructuras.h"

void nueva_partida(Jugadores *jugador, int *id_sala_actual, Objetos **lista_objetos, Conexiones **lista_conexiones, Puzles **lista_puzles);
void borrarPartida(char *id_jugador);
void guardarPartida(Jugadores *jug, int *id_sala_actual, Objetos *lista_objetos, Conexiones *lista_conexiones, Puzles *lista_puzles);
int cargarPartida(Jugadores *jug, int *id_sala_actual, Objetos **lista_objetos, Conexiones **lista_conexiones, Puzles **lista_puzles);

#endif // !GESTION_H