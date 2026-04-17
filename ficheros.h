#ifndef FICHEROS_H
#define FICHEROS_H

#include "condiciones.h"
#include "mapa.h"
#include "usuarios.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * contar_lineas: lee un fichero, ignorando líneas vacías y comentarios,
 * y devuelve el número de entidades reales, ideal para usar calloc/malloc.
 */
int contar_lineas(const char *ruta);

/*
 * volcado: carga todos los ficheros de datos del mundo del juego.
 * Retorna 1 si todos los ficheros se cargaron correctamente, 0 si alguno falló.
 */
int volcado(Salas **s, int *num_s, Conexiones **c, int *num_c, Puzles **p, int *num_p, Objetos **o, int *num_o, Jugadores **j, int *num_j);

void guardarPartida(Jugadores **jugadores, int indice_jugador, int *id_sala_actual, Objetos *lista_objetos, Conexiones *lista_conexiones, Puzles *lista_puzles);
int cargarPartida(Jugadores **jugadores, int indice_jugador, int *id_sala_actual, Objetos **lista_objetos, Conexiones **lista_conexiones, Puzles **lista_puzles);
void reinicio(Jugadores **jugadores, int indice_jugador, int *id_sala_actual, Objetos **lista_objetos, Conexiones **lista_conexiones, Puzles **lista_puzles);

#endif /* FICHEROS_H */
