/*
    Módulo: Usuarios (usuarios.c)

    Gestión de usuarios del juego.

    Dependencias:

    - "usuarios.h"
    - Varias cabeceras de la biblioteca de C
*/

#include "usuarios.h"
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

// Máximo número de intentos al introducir una contraseña.

#define MAX_INTENTOS 3

// Máxima longitud de una línea (en principio, BUFSIZ, el tamaño del buffer de E/S).

#define MAX_LONGITUD_LINEA BUFSIZ

// Declaraciones de funciones auxiliares privadas al módulo.

static Jugadores *busca_usuario(const char *usuario, Jugadores jugador[], int numero_jugadores);
static void lee_cadena(const char *mensaje, char *cadena, int n);
static int solicita_contrasena(Jugadores* jugador);
static void registra_usuario(Jugadores **jugadores, int *numero_jugadores);
static int nuevo_usuario(Jugadores **jugadores, int *numero_jugadores);

/*
    ENUNCIADO

    La ejecución del programa comienza con un mensaje inicial solicitando el usuario y contraseña para
    acceder a ESI-ESCAPE. Los datos introducidos se deberán contrastar con los datos previamente
    cargados en memoria (procedentes de Jugadores.txt). Si los datos introducidos no se encuentran
    en el sistema, este debe dar la posibilidad de registrarse con el perfil de jugador, dando acceso
    directo al sistema.
*/

/*
  Login. Incluye la posibilidad de registrar nuevos usuarios.

  Para poder añadir un nuevo jugador si se opta por registrar un nuevo usuario, necesitamos
  modificar el vector de jugadores y el número de jugadores, pasándolos por referencia, es
  decir, con punteros.

  - jugadores           (E/S)   Puntero al vector de jugadores en memoria dinámica (puntero a puntero a Jugador).
  - numero_jugadores    (E)     Puntero al número de jugadores.

  Devuelve el id del usuario que inicia la sesión.

  Precondición:

    *jugadores apunta a un vector reservado con memoria dinámica o es NULL (si no hay jugadores)
    *numero_jugadores > 0 y contiene el número de jugadores que contiene el vector dinámico

  Postcondición:

    Modifica *jugadores y *numero_jugadores si se registran nuevos usuarios
    Devuelve el id del jugador que inicia sesión    
*/

int login(Jugadores **jugadores, int *numero_jugadores)
{
    Jugadores *jugador;
    char usuario[sizeof (*jugadores)->jugador];
    int sesion_iniciada = 0;

    // El proceso de login finaliza cuando se consigue iniciar sesión.
    while (!sesion_iniciada) {
        // Solicitamos las credenciales.
        lee_cadena("Usuario: ", usuario, sizeof usuario);
        // Buscamos un jugador que se corresponda con ese usuario.
        jugador = busca_usuario(usuario, *jugadores, *numero_jugadores);
        // Comprobamos si el uauario existe.    
        if (jugador) {    // O, equivalentemente, jugador != NULL.
            // El usuario existe, solicitamos ls contraseña.
            sesion_iniciada = solicita_contrasena(jugador);
        } else {
            // El usuario no existe, informamos y damos la opción de registrarse como nuevo usuario.
            registra_usuario(jugadores, numero_jugadores);
        }
    }
    return jugador->id_jugador;
}

// Búsqueda de un usuario en el vector de jugadores.
//
// Realiza una búsqueda lineal por nombre de usuario y, si encuentra un jugador con ese nombre de usuario,
// devuelve un puntero a él. En caso contrario, devuelve NULL para indicar que no existe.
//
// Parámetros:
//
// - usuario          (E)      Nombre de usuario
// - jugador          (E)      Vector de jugadores (puntero a su primer elemento)
// - numero_jugadores (E)      Número de juhadores del vector
//
// Valor devuelto:
//
// Puntero al (primer) jugador con ese nombre de usuario o NULL si no existe ninguno con ese nombre.

Jugadores *busca_usuario(const char *usuario, Jugadores* jugador, int numero_jugadores)
{
    int encontrado = 0;
    int k;

    for (k = 0; k < numero_jugadores && !encontrado; ++k) {
        if (!strcmp(usuario, jugador[k].jugador)) {  // ¿Las cadenas son iguales?
            encontrado = 1;                          // Sí, terminamos el bucle.
        }
    }
    // Cuidado, pues aunque o encuentre, el bucle incrementa la k...
    if (encontrado) {
        return &jugador[k - 1];  // O, equivalentemente, jugador + k - 1.
    } else {
        return NULL;
    }
}

// Muestra un mensaje, lee una línea completa y guarda en la cadena n caracteres
// como mucho, incluyendo el '\0'.
//
// Parámetros:
//
// - mensaje (E)      Mensaje a mostrar
// - cadena  (S)      Cadena leída
// - n       (E)      Número máximo de caracteres, incluyendo el terminador
//
// Precondición:
//
//   mensaje apunta a una cadena válida
//   n > 0
//
// Postcondición:
//
//   cadena contiene los n - 1 primeros caracteres de la entrada seguidos de '\0'

void lee_cadena(const char *mensaje, char *cadena, int n)
{
    // Línea de máxima longitud.
    char linea[MAX_LONGITUD_LINEA];
    // Booleano que indica si la longitud raal de la línea no supera a n.
    int longitud_correcta = 0;

    // Si n supera la máxima longitud de línea, la igualamos.
    if (n > MAX_LONGITUD_LINEA) {
        n = MAX_LONGITUD_LINEA;
    }
    // Repetimos mientras no tengamos una línea de la longitud correcta.
    while (!longitud_correcta) {
        char *p;

        // Muestra el mensaje.
        fputs(mensaje,stdout);
        // Lee una línea completa de la entrada estándar.
        fgets(linea, MAX_LONGITUD_LINEA, stdin);
        // Aseguramos que exista un '\0'. Ver el manual: puede no haberlo si fgets falla...
        linea[MAX_LONGITUD_LINEA - 1] = '\0';
        // Corta la línea en el primer '\n' que pueda aparecer en ella.
        p = strchr(linea, '\n');
        if (p)  // O, equivalentemente, p != NULL.
            *p = '\0';
        // Comprobamos que la longitud real de la línea no supera a la permitida.
        if (strlen(linea) < n)
            longitud_correcta = 1;
        else
            printf("No introduzca un numero de caracteres superior a %d.\n\n", n - 1);
    }
    // Copia la línea resultante en la cadena.
    strcpy(cadena, linea);
}

// Solicita la contraseña de un jugador.
//
// Parámetros:
//
// - jugador          (E)      Vector de jugadores (puntero a su primer elemento)
//
// Precondición:
//
// - El jugador debe existir (el parámetro jugador no puede ser NULL)
//
// Poscondición:
//
// - Devuelve 1 si la contraseña es correcta y 0 si no lo es (tras MAX_INTENTOS)

int solicita_contrasena(Jugadores* jugador)
{
    char contrasena[sizeof jugador->contrasena];
    int contrasena_correcta = 0;
    int intentos = 0;

    while (intentos < MAX_INTENTOS && !contrasena_correcta) {
        lee_cadena("Contrasena: ", contrasena, sizeof contrasena);
        if (strcmp(contrasena, jugador->contrasena)) {
            // Es incorrecta, informamos y seguimos intentándolo.
            puts("Contrasena incorrecta\n");
            ++intentos;
        } else {
            // Es correcta, terminamos.
            contrasena_correcta = 1;
        }
    }
    if (intentos == MAX_INTENTOS) {
        // Es incorrecta y se ha alcanzado el máximo número de intentos permitidos.
        puts("Se ha superado el máximo número de intentos permitidos.\n");
    }
    return contrasena_correcta;
}

// Solicita si se quiere registrar a un jugador como nuevo usuario.
//
// Parámetros:
//
//  - jugadores           (E/S)   Puntero al vector de jugadores en memoria dinámica (puntero a puntero a Jugador).
//  - numero_jugadores    (E)     Puntero al número de jugadores.

void registra_usuario(Jugadores **jugadores, int *numero_jugadores)
{
    char respuesta[2];
    int respuesta_correcta = 0;

    while (!respuesta_correcta) {
        lee_cadena("El usuario no existe. ¿Desea registrar un nuevo usuario? [S/N] ", respuesta, sizeof(respuesta));
        switch (respuesta[0]) {
            case 'n': case 'N':
                respuesta_correcta = 1;
                break;
            case 's': case 'S':
                respuesta_correcta = nuevo_usuario(jugadores, numero_jugadores);
                if (respuesta_correcta) {
                    puts("Se ha registrado correctamente el usuario\n");
                } else {
                    puts("El usuario ya existe.\n");
                }
                break;
            default:
                puts("Conteste 'S' o 'N'.");
        }
        putchar('\n');
    }
}

// Registra a un jugador como nuevo usuario si no existe.
//
// Parámetros:
//
//  - jugadores           (E/S)   Puntero al vector de jugadores en memoria dinámica (puntero a puntero a Jugador).
//  - numero_jugadores    (E)     Puntero al número de jugadores.
//
//
// Valor devuelto:
//
// - 0  Indica que el usuario ya existe y no se ha podido registrar.
// - 1  Indica que el usuario se ha registrado con éxito.

int nuevo_usuario(Jugadores **jugadores, int *numero_jugadores)
{
    Jugadores nuevo_jugador;

    // El id del nuevo jugador se obtiene incrementando el número de jugadores.
    nuevo_jugador.id_jugador = *numero_jugadores + 1;
    // Obtenemos el nombre, el usuario y la contraseña del nuevo jugador.
    lee_cadena("Nombre: ", nuevo_jugador.nombre_jugador, sizeof nuevo_jugador.nombre_jugador);
    lee_cadena("Usuario: ", nuevo_jugador.jugador, sizeof nuevo_jugador.jugador);
    // Buscamos si ese jugador ya está registrado (solo se comprueba el usuario, puede haber personas con el mismo nombre).
    if (busca_usuario(nuevo_jugador.jugador, *jugadores, *numero_jugadores)) {
       return 0;
    } else {
        lee_cadena("Contrasena: ", nuevo_jugador.contrasena, sizeof nuevo_jugador.contrasena);
        // Añadimos el nuevo jugador.
        ++*numero_jugadores;
        Jugadores *tmp = realloc(*jugadores, sizeof(Jugadores) * *numero_jugadores);  // Reservamos espacio para una estructura extra.
        // Comprobamos si se ha posido reservar memoria.
        if (!tmp) {
            --*numero_jugadores;  // Revertir el incremento.
            puts("Error al asignar memoria para el nuevo jugador.\n");
            return 0;
        }
        *jugadores = tmp;
        (*jugadores)[*numero_jugadores - 1] = nuevo_jugador;    // Copiamos la estructura a la última posición.

        // Guardamos el jugador en el fichero para que sea persistente.
        FILE *f = fopen("jugadores.txt", "a");
        if (f) {
            fprintf(f, "%02d-%s-%s-%s\n", nuevo_jugador.id_jugador, nuevo_jugador.nombre_jugador, nuevo_jugador.jugador, nuevo_jugador.contrasena);
            fclose(f);
        } else {
            puts("Error: No se pudo abrir el archivo de jugadores para guardar.\n");
        }

        return 1;
    }
}