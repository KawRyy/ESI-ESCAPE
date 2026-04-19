#ifndef MAPA_H
#define MAPA_H
// (MÓDULO DE PROCESAMIENTO)

// AUTOR:

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
    /* SE MODIFICA DURANTE LA PARTIDA */
    int estado_conexion; // 0: CERRADA ; 1: ABIERTA
    char id_condicionante[5]; // ID del objeto o puzle que condiciona la conexión, si no tiene condición se pone "0"
} Conexiones;


void ExaminarSalidas(Conexiones *con, int num_conexiones, int id_sala_actual, Salas *sal);
void AccionMover(Conexiones *con, int num_conexiones, int *id_sala_actual, Salas *sal);
void DescribirSala(Salas *sal, int id_sala_actual);

void AbrirConexion(Conexiones *con, int k);
int ComprobarConexion(Conexiones *con, int k,int id_sala_actual, char *n);

#endif /* MAPA_H */