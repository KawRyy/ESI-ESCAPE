/*
    Alejandro Palomo Medina

    Módulo: Usuarios (usuarios.h/usuarios.c)

    Prrograma de prueba para el módulo.Gestión de usuarios del juego.

    Dependencias:

    - usuarios.h
    - Varias cabeceras de la biblioteca de C
*/

#include "../usuarios.h"
#include <stdio.h>
#include <stdlib.h>

// Prueba independiente del módulo de usuarios (usuarios.[ch])

int main()
{
  int n = 0;
  Jugadores *jugador = NULL;
  int id = login(&jugador, &n); // Si es necesario, reserva memoria dinámica para el nuevo jugador...
  printf("Sesión iniciada para el jugador con id = %d", id);
  free(jugador);
}
