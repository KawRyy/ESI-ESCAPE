#ifndef JUEGO_H
#define JUEGO_H
#include "estructuras.h"
void ExaminarObjeto(Partida *par);  
void ExaminarSalidas(Partida *par);
void accionMover(Partida *par, Salas *sal);
void Inventario(Partida *par);
void CogerObjeto(Jugadores *jug, Partida *par);
void SoltarObjeto(Jugadores *jug, Partida *par);
void UsarObjeto(Partida *par);
void ResolverPuzle(Partida *par);

#endif