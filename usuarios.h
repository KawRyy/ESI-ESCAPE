/*
    Módulo: Usuarios (usuarios.h) - Alejandro Palomo Medina

    Gestión de usuarios del juego.

    Dependencias:

    - "ficheros.h"  Lectura/escritura del fichero de usuarios.
*/

#ifndef USUARIOS_H
#define USUARIOS_H

// Al leer el fichero de jugadores hay que crear, con memoria dinámica, el vector dinámico de jugadores
// y el de objetos de cada jugador, aparte de actualizar la variable que lleva la cuenta del número
// real de jugadores y el campo num_objetos de cada jugador.
//
// Luego, no se puede olvodar liberar toda la memoria asignada (al final de main).

typedef struct {
    int id_jugador;             // Identificador del jugador.
    char nombre_jugador[21];    // Nombre del jugador.
    char jugador[11];           // Nombre de usuario (nickname).
    char contraseña[9];         // Contraseña.
    // INVENTARIO DINÁMICO (IDs de objetos). Otra ocpión es una estructura dentro con los dos campos que siguen...
    char **id_objetos;          // Vector de dinámico de cadenas (puntero a puntero).
    int num_objetos;            // Número de objetos.
} Jugadores;

/*
    Login. Incluye la posibilidad de registrar un nuevo usuario.

    Para poder añadir un nuevo jugador si se opta por registrar un nuevo usuario, necesitamos 
    modificar el vector de jugadores y el número de jugadores, pasándolos por referencia, es
    decir, con punteros.

    - jugadores         Puntero al vector de jugadores en memoria dinámica (puntero a puntero a Jugadores).
    - número_jugadores  Puntero al número de jugadores.
*/

void login(Jugadores **jugadores, int *número_jugadores);

#endif /* USUARIOS_H */