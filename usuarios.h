/*
    Alejandro Palomo Medina
    
    Módulo: Usuarios (usuarios.h)

    Gestión de usuarios del juego.

    Dependencias:
    - estructuras.h

*/

#ifndef USUARIOS_H
#define USUARIOS_H

#include "estructuras.h"

/*
    Login. Incluye la posibilidad de registrar nuevos usuarios.

    Para poder añadir un nuevo jugador si se opta por registrar un nuevo usuario, necesitamos 
    modificar el vector de jugadores y el número de jugadores, pasándolos por referencia, es
    decir, con punteros.

    - jugadores         Puntero al vector de jugadores en memoria dinámica (puntero a puntero a Jugadores).
    - número_jugadores  Puntero al número de jugadores.

    Precondición:

      *jugadores apunta a un vector reservado con memoria dinámica o es NULL (si no hay jugadores)
      *número_jugadores > 0 y contiene el número de jugadores que contiene el vector dinámico

    Postcondición:

      Modifica *jugadores y *numero_jugadores si se registran nuevos usuarios
      Devuelve un puntero al jugador que inicia sesión, o NULL si falla    
*/

Jugadores* login(Jugadores **jugadores, int *numero_jugadores);

#endif /* USUARIOS_H */