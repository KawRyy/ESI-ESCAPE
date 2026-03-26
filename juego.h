#ifndef JUEGO_H
#define JUEGO_H
#include "estructuras.h"
void buclePartida(Jugadores *jug, Salas *sal, Conexiones *con, Objetos *obj, Puzles *puz);
void Describir_Sala(Partida *par);
void Examinar(Partida *par);  
void Inventario();
void CogerObjeto(Objetos *obj, Jugadores *jug, Partida *par);
void SoltarObjeto(Objetos *obj, Jugadores *jug, Partida *par);


#endif