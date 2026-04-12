#ifndef MENUS_H
#define MENUS_H

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

void menu_inicial(Partida **par, Jugadores **jug, Objetos **obj, Salas **sal, Conexiones **con, Puzles **puz);

#endif /* MENUS_H */