#ifndef CONDICIONES_H
#define CONDICIONES_H


typedef struct {
    char id_objeto[5];
    char nombre_objeto[16];
    char descripcion_objeto[51];
    int localizacion_objeto;
} Objetos;

typedef struct {
    char id_puzle[5];
    int id_sala_puzle; // Sala donde se encuentra
    int tipo_elemento; // Elemento que genera resolver el puzle, 1: CÓDIGO ; 2: PALABRA
    char descripcion_puzle[151];
    char solucion_puzle[51];
    int resuelto; // 0: PENDIENTE ; 1: RESUELTO
} Puzles;

void ExaminarObjeto(Objetos *obj, int num_objetos, int id_sala_actual);  
void Inventario(Objetos *obj, Jugadores *jug, int num_objetos);
void CogerObjeto(Jugadores *jug, Objetos *obj, int num_objetos, int id_sala_actual);
void SoltarObjeto(Jugadores *jug, Objetos *obj, int num_objetos, int id_sala_actual);
void UsarObjeto(Objetos *obj, Jugadores *jug, int num_conexiones, Conexiones *con, int id_sala_actual);
void ResolverPuzle(Puzles *puz, Jugadores *jug, int num_conexiones, Puzles *puz, int num_puzles, Conexiones *con, int id_sala_actual);

#endif