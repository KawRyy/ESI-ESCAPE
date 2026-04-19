#ifndef CONDICIONES_H
#define CONDICIONES_H
// (MÓDULO DE PROCESAMIENTO)

// AUTOR:

#include "mapa.h" // Se incluyen las estructuras de mapa para el .c, se refleja aqui por legibilidad


typedef struct {
    char id_objeto[5];
    char nombre_objeto[16];
    char descripcion_objeto[51];
    /* SE MODIFICA DURANTE LA PARTIDA */
    int localizacion_objeto; // 0: INVENTARIO ; ID: SALA (se indica el id_sala)
} Objetos;

typedef struct {
    char id_puzle[5];
    int id_sala_puzle; // Sala donde se encuentra
    int tipo_elemento; // Elemento que genera resolver el puzle, 1: CÓDIGO ; 2: PALABRA
    char descripcion_puzle[151];
    char solucion_puzle[51];
    /* SE MODIFICA DURANTE LA PARTIDA */
    int resuelto; // 0: PENDIENTE ; 1: RESUELTO
} Puzles;

typedef struct {
    Objetos *Inventario; // Puntero a vector dinámico de objetos (o NULL si no hay ninguno).
    int num_objetos; // Número de objetos.
}Inventario;

void ExaminarObjeto(Objetos *obj, int num_objetos, int id_sala_actual);  
void MostrarInventario(Objetos *obj, Inventario *inv, int num_objetos);
void CogerObjeto(Inventario *inv, Objetos *obj, int num_objetos, int id_sala_actual);
void SoltarObjeto(Inventario *inv, Objetos *obj, int num_objetos, int id_sala_actual);
void UsarObjeto(Objetos *obj, Inventario *inv, int num_conexiones, Conexiones *con, int id_sala_actual);
void ResolverPuzle(int num_conexiones, Puzles *puz, int num_puzles, Conexiones *con, int id_sala_actual);

#endif /* CONDICIONES_H */