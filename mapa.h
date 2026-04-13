#ifndef MAPA_H
#define MAPA_H

#include "estructuras.h"

//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de examinar salidas en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de las conexiones que parten de la sala actual, indicando si están abiertas o bloqueadas.
void ExaminarSalidas(Partida *par);

//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de moverse en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de las conexiones que parten de la sala actual y se pregunta al jugador si desea moverse a alguna de las salas conectadas.
void accionMover(Partida *par, Salas *sal);

//Precondición: Deben haber sido cargados los datos de la partida
//Postcondición: Se muestra la descripción de la sala actual del jugador
void describirSala(Partida *par, Salas *sal);

#endif