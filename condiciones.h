#ifndef CONDICIONES_H
#define CONDICIONES_H


typedef struct {
    char id_objeto[5];
    char nombre_objeto[16];
    char descripcion_objeto[51];
    int localizacion_objeto;
} Objetos;

typedef struct {
    char id_puzle[4];
    int id_sala_puzle; // Sala donde se encuentra
    int tipo_elemento; // Elemento que genera resolver el puzle, 1: CÓDIGO ; 2: PALABRA
    char descripcion_puzle[151];
    char solucion_puzle[51];
    int resuelto; // 0: PENDIENTE ; 1: RESUELTO
} Puzles;

void ExaminarObjeto(Partida *par);  
void Inventario(Partida *par);
void CogerObjeto(Jugadores *jug, Partida *par);
void SoltarObjeto(Jugadores *jug, Partida *par);
void UsarObjeto(Partida *par);
void ResolverPuzle(Partida *par);

#endif