#ifndef MENUS_H
#define MENUS_H
// (MÓDULO DE FLUJO DE DATOS)

// AUTOR:

#include "condiciones.h"
#include "usuarios.h"
#include "mapa.h"
#include "ficheros.h"

void menu_inicial(Salas **sal, int *num_salas, int *id_sala_actual, Conexiones **con, int *num_conexiones, Puzles **puz, int *num_puzles, Objetos **obj, int *num_objetos, Jugadores **jug, int *num_jugadores, Inventario *inv);

#endif /* MENUS_H */