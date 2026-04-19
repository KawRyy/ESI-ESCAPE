#include <stdio.h>
#include <stdlib.h>
#include "ficheros.h"
#include "menus.h"

int main(){
    // DECLARACION DE ESTRUCTURAS QUE SE MODIFICARAN DURANTE LA PARTIDA
    Objetos *obj = NULL;
    int num_objetos = 0; // Necesario para el vector dinámico de objetos, dependiendo del numero de objetos en el fichero, se actualiza en el volcado
    Puzles *puz = NULL;
    int num_puzles = 0; // IDEM
    Conexiones *con = NULL;
    int num_conexiones = 0; // IDEM
    Inventario inv = {NULL, 0};

    // DECLARACION DE LA ESTRUCTURA DE USUARIOS, SE INICIALIZA EN EL VOLCADO CON LOS JUGADORES EXISTENTES, SE MODIFICA EN EL LOGIN AÑADIENDO UN NUEVO USUARIO (SI PROCEDE)
    Jugadores *jug = NULL;
    int num_jugadores = 0; // Necesario para el vector dinámico de jugadores, se actualiza en el login

    // DECLARACION DE LA ESTRUCTURA SALAS, NO SE MODIFICA
    Salas *sal = NULL;
    int num_salas = 0; // IDEM
    int id_sala_actual = 0; // Variable para almacenar el ID de la sala actual del jugador, la sala inicial es 0

    // (1) FLUJO DE DATOS: Se vuelcan los datos de los ficheros a las estructuras dinámicas que se modificaran en partida
    volcado(&sal, &num_salas, &con, &num_conexiones, &puz, &num_puzles, &obj, &num_objetos, &jug, &num_jugadores);

    // (2) FLUJO DE DATOS: Se pasan las direcciones de las estructuras de datos inicializadas al modulo de flujo de datos
    menu_inicial(&sal, num_salas, &id_sala_actual, &con, num_conexiones, &puz, num_puzles, &obj, num_objetos, &jug, &num_jugadores, &inv);
    // Pasamos por valor el numero de estructuras pues no se volverán a modificar durante la partida
    
    return 0;
}
