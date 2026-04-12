#include "../usuarios.h"
#include <stdlib.h>

int main() {
  int n = 0;
  Jugadores *jugador = NULL;
  login(&jugador, &n); // Si es necesario, reserva memoria dinámica para el nuevo jugador...
  free(jugador);
}
