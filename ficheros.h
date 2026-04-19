#ifndef FICHEROS_H
#define FICHEROS_H
// (MÓDULO DE CARGA Y GUARDADO)

// AUTOR: Álvaro Rodríguez García

#include "condiciones.h"
#include "usuarios.h"
#include "mapa.h"
#include "temp.h"

int volcado(Salas **s, int *num_s, Conexiones **c, int *num_c, Puzles **p, int *num_p, Objetos **o, int *num_o, Jugadores **j, int *num_j);
void guardarPartida(Jugadores **jugadores, int indice_jugador, int *id_sala_actual, Inventario *inv, Objetos *lista_objetos, Conexiones *lista_conexiones, Puzles *lista_puzles);
int cargarPartida(Jugadores **jugadores, int indice_jugador, int *id_sala_actual, Inventario *inv, Objetos **lista_objetos, int num_objetos, Conexiones **lista_conexiones, int num_conexiones, Puzles **lista_puzles, int num_puzles);
void reinicio(Jugadores **jugadores, int indice_jugador, int *id_sala_actual, Inventario *inv, Objetos **lista_objetos, int *num_objetos, Conexiones **lista_conexiones, int *num_conexiones, Puzles **lista_puzles, int *num_puzles);
int existePartida(int id_jugador);

#endif /* FICHEROS_H */