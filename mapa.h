#ifndef MAPA_H
#define MAPA_H


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