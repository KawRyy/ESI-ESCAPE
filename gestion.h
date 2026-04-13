#ifndef GESTION_H
#define GESTION_H

#include "estructuras.h"

void nueva_partida(Jugadores *jugador, Partida *partida);
void borrarPartida(char *id_jugador);
void guardarpartida(Partida *par);
int cargarPartida(Partida *par, char *id_jugador);

#endif // !GESTION_H