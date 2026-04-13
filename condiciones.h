#ifndef CONDICIONES_H
#define CONDICIONES_H

#include "estructuras.h"

void ExaminarObjeto(Partida *par);  
void Inventario(Partida *par, Jugadores *jug);
void CogerObjeto(Jugadores *jug, Partida *par);
void SoltarObjeto(Jugadores *jug, Partida *par);
void UsarObjeto(Partida *par, Jugadores *jug);
void ResolverPuzle(Partida *par);

#endif