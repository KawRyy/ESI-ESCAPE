#ifndef USUARIOS_H
#define USUARIOS_H

// (MÓDULO DE PROCESAMIENTO)

#include "condiciones.h"
#include "temp.h"

// PENDIENTE
//
// Objetos, Puzles, Salas, Conexiones y Partida está en otro ficheros que todavía no compilan.
// Cuando compilen, eliminar e incluir los .h correspondientes...

/*
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

typedef struct {
    int id_jugador;        // ID del usuario que juega
    int id_sala_actual;    // Dónde se quedó el jugador
    // Listas dinámicas para reflejar los cambios en el mundo
    Objetos *lista_objetos;      // Array con la ubicación actual de CADA objeto
    int num_objetos;
    Conexiones *lista_conexiones; // Array con el estado (Abierta/Bloqueada) de cada una
    int num_conexiones;
    Puzles *lista_puzles;        // Array con el estado (Resuelto/Pendiente) de cada uno
    int num_puzles;
} Partida;
*/

////////////////////////////////////////////////////////////////////////////////////////////////

// Al leer el fichero de jugadores hay que crear, con memoria dinámica, los vectores dinámicos:
//
// jugadores
// objetos
//
// Cada uno de estos los representamos con un punterom que debe llevar asociado una variable o campo
// que indique el número de elementos (el vector es dinámico, y si no lo guardamos, no sabemos dónde
// acaba, ni cuándo parar al recorrerlo).
//
// Las funciones deben mantener actualizadas esos números de elementos.
// Luego, no se puede olvidar liberar toda la memoria asignada dinámicamente.
// Por ejemplo, se puede hacer al final de main.

/*
    Login. Incluye la posibilidad de registrar nuevos usuarios.

    Para poder añadir un nuevo jugador si se opta por registrar un nuevo usuario, necesitamos
    modificar el vector de jugadores y el número de jugadores, pasándolos por referencia, es
    decir, con punteros.

    - jugadores         Puntero al vector de jugadores en memoria dinámica (puntero a puntero a Jugadores).
    - número_jugadores  Puntero al número de jugadores.

    Devuelve el id del usuario que inicia la sesión.
    
    Precondición:

      *jugadores apunta a un vector reservado con memoria dinámica o es NULL (si no hay jugadores)
      *número_jugadores > 0 y contiene el número de jugadores que contiene el vector dinámico

    Postcondición:

      Modifica *jugadores y *numero_jugadores si se registran nuevos usuarios
      Devuelve el id del jugador que inicia sesión    
*/

int login(Jugadores **jugadores, int *numero_jugadores);

#endif /* USUARIOS_H */