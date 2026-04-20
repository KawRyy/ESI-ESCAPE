#ifndef CONDICIONES_H
#define CONDICIONES_H
// (MÓDULO DE PROCESAMIENTO)

// AUTOR: Carlos Junquera Lobato

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

//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de examinar objetos en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de los objetos que se encuentran en la sala actual
void ExaminarObjeto(Objetos *obj, int num_objetos, int id_sala_actual);

//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de ver el inventario en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de los objetos que se encuentran en el inventario del jugador
void MostrarInventario(Objetos *obj, Inventario *inv, int num_objetos);

//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de coger objetos en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de los objetos que se encuentran en la sala actual y se pregunta al jugador si desea coger alguno de ellos. 
void CogerObjeto(Inventario *inv, Objetos *obj, int num_objetos, int id_sala_actual);

//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de soltar objetos en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de los objetos que se encuentran en el inventario del jugador y se pregunta si desea soltar alguno de ellos. 
void SoltarObjeto(Inventario *inv, Objetos *obj, int num_objetos, int id_sala_actual);

//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de usar objetos en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de los objetos que se encuentran en el inventario del jugador y se pregunta si desea usar alguno de ellos. 
void UsarObjeto(Objetos *obj, Inventario *inv, int num_conexiones, Conexiones *con, int id_sala_actual);

//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de resolver puzles en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de los puzles que se encuentran en la sala actual y se pregunta al jugador si desea intentar resolver alguno de ellos.
void ResolverPuzle(int num_conexiones, Puzles *puz, int num_puzles, Conexiones *con, int id_sala_actual);

#endif /* CONDICIONES_H */