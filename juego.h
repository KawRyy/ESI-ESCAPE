#ifndef JUEGO_H
#define JUEGO_H
#include "estructuras.h"
void buclePartida(Jugadores *jug, Salas *sal, Conexiones *con, Objetos *obj, Puzles *puz, Partida *par);
void ExaminarObjeto(Partida *par);  
void ExaminarSalidas(Partida *par);
void accionMover(Partida *par);
void Inventario(Partida *par);
void CogerObjeto(Objetos *obj, Jugadores *jug, Partida *par);
void SoltarObjeto(Objetos *obj, Jugadores *jug, Partida *par);
void UsarObjeto(Objetos *obj, Conexiones *con, Partida *par);
void ResolverPuzle(Puzles *puz, Partida *par);

#endif