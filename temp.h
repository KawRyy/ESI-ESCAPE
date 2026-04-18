#ifndef TEMP_H
#define TEMP_H

typedef struct {
    char id_objeto[5];
    char nombre_objeto[16];
    char descripcion_objeto[51];
    /* SE MODIFICA DURANTE LA PARTIDA */
    int localizacion_objeto; // 0: INVENTARIO ; ID: SALA (se indica el id_sala)
} Objetos;

typedef struct {
    int id_jugador;             // Identificador del jugador.
    char nombre_jugador[21];    // Nombre del jugador.
    char jugador[11];           // Nombre de usuario (nickname).
    char contrasena[9];         // Contraseña.
    Objetos *objetos;           // Puntero a vector dinámico de objetos (o NULL si no hay ninguno).
    int num_objetos;            // Número de objetos.
} Jugadores;


#endif /* TEMP_H */