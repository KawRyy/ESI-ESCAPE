#ifndef USUARIOS_H
#define USUARIOS_H

// (MÓDULO DE PROCESAMIENTO)

#include "condiciones.h"
#include "temp.h"

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