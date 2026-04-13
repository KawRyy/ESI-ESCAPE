#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H


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
    char id_jugador[5];    // ID del usuario que juega
    int id_sala_actual;    // Dónde se quedó el jugador

    // Listas dinámicas para reflejar los cambios en el mundo
    Objetos *lista_objetos;
    int num_objetos;

    Conexiones *lista_conexiones;
    int num_conexiones;

    Puzles *lista_puzles;
    int num_puzles;
} Partida;

typedef struct {
    char id_jugador[5];
    char nombre_jugador[16];
    char nickname[16];
    char contrasena[16];
    Objetos *objetos; // Array dinámico de objetos en el inventario del jugador
    int num_objetos;
} Jugadores;

#endif