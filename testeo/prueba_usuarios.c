#include "../usuarios.h"
#include <stdlib.h>

// Prueba independiente del módulo de usuarios (usuarios.[ch])

int main() {
  int n = 0;
  Jugadores *jugador = NULL;
  login(&jugador, &n); // Si es necesario, reserva memoria dinámica para el nuevo jugador...
  free(jugador);
}
