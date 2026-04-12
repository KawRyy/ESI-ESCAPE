#ifndef SALAS_Y_CONEXIONES_H
#define SALAS_Y_CONEXIONES_H
#include "estructuras.h"

typedef struct {
    int id_sala; // 2 digitos EJ: 99
    char nombre_sala[21];
    char descripcion_sala[151];
    int tipo_sala; // 1:INICIAL ; 2: NORMAL ; 3: SALIDA
} Salas;

typedef struct {
    char id_conexion[4];
    int id_sala_orig;
    int id_sala_dest;
    int estado_conexion; // 0: CERRADA ; 1: ABIERTA
    int condicion_conexion; // 0: no aplica ; 1: requiere objeto ; 2: requiere puzle
} Conexiones;

void ExaminarSalidas(Partida *par);
void accionMover(Partida *par, Salas *sal);

#endif