#ifndef MAPA_H
#define MAPA_H
// (MÓDULO DE PROCESAMIENTO)

// AUTOR: Francisco Javier Funes Marín

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

// Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de examinar salidas en el menú de acciones del jugador
// Postcondición: Se muestra la descripción de las conexiones que parten de la sala actual, indicando si están abiertas o bloqueadas.
void ExaminarSalidas(Conexiones *con, int num_conexiones, int *id_sala_actual, Salas *sal);

// Precondición: Deben haber sido cargados los datos de la partida, haber sido seleccionada la opción de entrar a otra sala en el menú de acciones del jugador, y se ha comprobado que existen conexiones abiertas desde la sala actual.
// Postcondición: Se cambia la sala actual del jugador a la sala seleccionada por el jugador entre las opciones de salas conectadas a través de conexiones abiertas.
void DescribirSala(Salas *sal, int id_sala_actual);

// Precondición: Datos cargados e id_sala_actual válido.
// Postcondición: Muestra información y espera a que el usuario decida salir.
void AbrirConexion(Conexiones *con, int k, int id_sala_actual);

//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de usar objetos o resolver puzles en el menú de acciones del jugador
//Postcondición: Se comprueba si la conexión con índice k en la lista de conexiones tiene como sala de origen la sala actual, tiene el estado de conexión a 0 (bloqueada) y tiene como condicionante el id del objeto o puzle que se está evaluando.
int ComprobarConexion(Conexiones *con, int k,int id_sala_actual, char *n);

#endif /* MAPA_H */