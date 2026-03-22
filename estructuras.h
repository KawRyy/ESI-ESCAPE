#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

typedef struct {
    int id_jugador; // 2 digitos EJ: 99
    char nombre_jugador[21];
    char jugador[11]; // nickname
    char contraseña[9];

    // INVENTARIO DINÁMICO (IDs de objetos)
    char **id_objeto;
    int num_items;
} Jugadores;

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
    char id_objeto[5];
    char nombre_objeto[16];
    char descripcion_objeto[51];
    int localizacion_objeto; // 0: SALA ; 1: INVENTARIO
} Objetos;

typedef struct {
    char id_puzle[4];
    int id_sala_puzle; // Sala donde se encuentra
    int tipo_elemento; // Elemento que genera resolver el puzle, 1: CÓDIGO ; 2: PALABRA
    char descripcion_puzle[151];
    char solucion_puzle[51];
} Puzles;

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

#endif // ESTRUCTURAS_H