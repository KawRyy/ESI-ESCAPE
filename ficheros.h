#ifndef FICHEROS_H
#define FICHEROS_H
// (MÓDULO DE CARGA Y GUARDADO)

// AUTOR: Álvaro Rodríguez García

#include "condiciones.h"
#include "usuarios.h"
#include "mapa.h"

//Precondicion: recibe punteros a arrays de Salas, Conexiones, Puzles, Objetos y Jugadores (inicialmente NULL) y punteros a enteros para almacenar el numero de cada entidad
//Postcondicion: lee todos los ficheros y llena las estructuras en memoria. Devuelve 1 si todas las lecturas fueron exitosas o 0 si alguna lectura fallo
int volcado(Salas **s, int *num_s, Conexiones **c, int *num_c, Puzles **p, int *num_p, Objetos **o, int *num_o, Jugadores **j, int *num_j);

//Precondicion: recibe un puntero a un jugador, el ID de la sala actual y los arrays de objetos, conexiones y puzles con su respectivo numero de elementos
//Postcondicion: guarda el progreso del jugador en partida.txt, actualizando solo las entradas correspondientes al jugador para mantener intacto el progreso de los demas jugadores. Devuelve 1 si se guardo correctamente o 0 si hubo error al abrir partida.txt (en cuyo caso no se modifica el archivo)
void guardarPartida(Jugadores **jugadores, int indice_jugador, int *id_sala_actual, Inventario *inv, Objetos *lista_objetos, Conexiones *lista_conexiones, Puzles *lista_puzles);

//Precondicion: recibe un puntero a un jugador, el ID de la sala actual y los arrays de objetos, conexiones y puzles con su respectivo numero de elementos
//Postcondicion: carga el progreso del jugador desde partida.txt, actualizando la sala actual, la ubicacion de los objetos, el estado de las conexiones y el estado de los puzles. Devuelve 1 si se cargo correctamente o 0 si hubo error al abrir partida.txt (en cuyo caso se mantiene el estado base limpio)
int cargarPartida(Jugadores **jugadores, int indice_jugador, int *id_sala_actual, Inventario *inv, Objetos **lista_objetos, int num_objetos, Conexiones **lista_conexiones, int num_conexiones, Puzles **lista_puzles, int num_puzles);

//Precondicion: recibe un puntero a un jugador, el ID de la sala actual y los punteros a las estructuras y sus contadores.
//Postcondicion: reinicia el progreso del jugador, limpiando su inventario y recargando el estado puro del mundo desde los ficheros.
void reinicio(Jugadores **jugadores, int indice_jugador, int *id_sala_actual, Inventario *inv, Objetos **lista_objetos, int *num_objetos, Conexiones **lista_conexiones, int *num_conexiones, Puzles **lista_puzles, int *num_puzles);

//Precondicion: recibe el ID del jugador
//Postcondicion: devuelve 1 si el jugador tiene una partida guardada en partida.txt, 0 en caso contrario
int existePartida(int id_jugador);

#endif /* FICHEROS_H */