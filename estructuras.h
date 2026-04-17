#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H


typedef struct {
    char id_objeto[5];
    char nombre_objeto[16];
    char descripcion_objeto[51];
    int localizacion_objeto;
} Objetos;

typedef struct {
    char id_puzle[5];
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
    char id_conexion[5];
    int id_sala_orig;
    int id_sala_dest;
    int estado_conexion; // 0: CERRADA ; 1: ABIERTA
    char id_condicionante[5]; // ID del objeto o puzle que condiciona la conexión, si no tiene condición se pone "0"
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
    int id_jugador;             // Identificador del jugador.
    char nombre_jugador[21];    // Nombre del jugador.
    char jugador[11];           // Nombre de usuario (nickname).
    char contrasena[9];         // Contraseña.
    Objetos *objetos;           // Puntero a vector dinámico de objetos (o NULL si no hay ninguno).
    int num_objetos;            // Número de objetos.
} Jugadores;

#endif