#include "../usuarios.h"
#include <stdio.h>
#include <stdlib.h>

// Prueba independiente del módulo de usuarios (usuarios.[ch])

int main() {
  int n = 0;
  Jugadores *jugador = NULL;
  int id = login(&jugador, &n); // Si es necesario, reserva memoria dinámica para el nuevo jugador...
  printf("Sesión iniciada para el jugador con id = %d", id);
  free(jugador);
}
