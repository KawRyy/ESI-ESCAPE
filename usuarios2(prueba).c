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

static Jugador *busca_usuario(const char *usuario, Jugador jugador[], int numero_jugadores);
static void lee_cadena(const char *mensaje, char *cadena, int n);
static int registra_nuevo_usuario(Jugador **jugador, int *número_jugadores);
static int registra_usuario(Jugador **jugadores, int *número_jugadores);
static void nuevaPartida(Partida *partida);

// Para depuración...

static void muestra_usuarios(Jugador *jugador, int número_jugadores)
{
    putchar('\n');
    puts(">>> DEPURACIÓN");
    for (int k = 0; k < número_jugadores; ++k)
        printf("%02d-%s-%s-%s\n", jugador[k].id_jugador, jugador[k].nombre_jugador, jugador[k].jugador, jugador[k].contraseña);
    puts("<<< DEPURACIÓN\n");
}

/*
    ENUNCIADO

    La ejecución del programa comienza con un mensaje inicial solicitando el usuario y contraseña para
    acceder a ESI-ESCAPE. Los datos introducidos se deberán contrastar con los datos previamente
    cargados en memoria (procedentes de Jugadores.txt). Si los datos introducidos no se encuentran
    en el sistema, este debe dar la posibilidad de registrarse con el perfil de jugador, dando acceso
    directo al sistema.
*/

/*
    El enunciado dice que se emplee memoria dinámica, se entiende que cuando sea necesario, es
    decir, cuando no se conozca de antemano el número de objetos necesarios. Si el tamaño es fijo y
    conocido, no tiene sentido (no vamos a poner las variables locales en memoria dinámica).

    Para poder añadir un nuevo jugador, necesitamos modificar el vector de jugadores y el número de
    jugadores, pasándolos por referencia, es decir, con punteros.

    Parámetros:

    jugadores           (E/S)   Puntero al vector de jugadores en memoria dinámica (puntero a puntero a Jugador).
    número_jugadores    (E)     Puntero al número de jugadores.
*/

void login(Jugador **jugadores, int *número_jugadores)
{
    Jugador *jugador;
    char usuario[sizeof (*jugadores)->jugador];
    char contraseña[sizeof (*jugadores)->contraseña];
    int sesión_iniciada = 0;

    // El proceso de login finaliza cuando se consigue iniciar sesión.
    while (!sesión_iniciada) {
        // Solicitamos las credenciales.
        lee_cadena("Usuario: ", usuario, sizeof usuario);
        // Buscamos un jugador que se corresponda con ese usuario.
        jugador = busca_usuario(usuario, *jugadores, *número_jugadores);
        // Comprobamos si el uauario existe.     
        if (jugador) {    // O, equivalentemente, p_jugador != NULL.
            // El usuario existe, comprobamos si la contraseña es correcta.
            int intentos = 0;

            while (intentos < MAX_INTENTOS && !sesión_iniciada) {
                lee_cadena("Contraseña: ", contraseña, sizeof contraseña);
                if (strcmp(contraseña, jugador->contraseña)) {
                    // Es incorrecta, informamos y seguimos intentándolo.
                    puts("Contraseña incorrecta\n");
                    ++intentos;
                } else {
                    // Es correcta, terminamos.
                    sesión_iniciada = 1;
                }
            }
            if (intentos == MAX_INTENTOS) {
                // Es incorrecta y se ha alcanzado el máximo número de intentos permitidos.
                puts("Se ha superado el máximo número de intentos permitidos.\n");
            }
        } else {
            // El usuario no existe, informamos y damos la opción de registrarse como nuevo usuario.
            char respuesta[2];
            int respuesta_correcta = 0;

            puts("No existe un jugador con ese nombre\n");
            while (!respuesta_correcta) {
                lee_cadena("¿Desea registrar un nuevo usuario? [S/N] ", respuesta, sizeof(respuesta));
                switch (respuesta[0]) {
                    case 'n': case 'N':
                        respuesta_correcta = 1;
                        break;
                    case 's': case 'S':
                        respuesta_correcta = registra_nuevo_usuario(jugadores, número_jugadores);
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
    }
}

// Búsqueda de un usuario en el vector de jugadores.
//
// Realiza una búsqueda lineal por nombre de usuario y, si encuentra un jugador con ese nombre de usuario, 
// devuelve un puntero a él. En caso contrario, devuelve NULL para indicar que no existe.
//
// Parámetros:
//
// usuario          (E)      Nombre de usuario 
// jugador          (E)      Vector de jugadores (puntero a su primer elemento)
// numero_jugadores (E)      Número de juhadores del vector
//
// Valor devuelto:
//
// Puntero al (primer) jugador con ese nombre de usuario o NULL si no existe ninguno con ese nombre.

Jugador *busca_usuario(const char *usuario, Jugador jugador[], int numero_jugadores)
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

// Muestra un mensaje, lee una línea completa y guarda en la cadena n caracteres como mucho, incluyendo el '\0'.
//
// Parámetros:
//
// mensaje (E)      Mensaje a mostrar 
// cadena  (S)      Cadena leída
// n       (E)      Número máximo de caracteres, incluyendo el terminador

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
        printf(mensaje);
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
            printf("No introduzca un número de caracteres superior a %d.\n\n", n - 1);
    }
    // Copia la línea resultante en la cadena.
    strcpy(cadena, linea);
}

// Registra a un jugador como nuevo usuario.
//
// Valor devuelto:
//
// - 0  Indica que el usuario ya existe y no se ha podido registrar.
// - 1  Indica que el usuario se ha registrado con éxito. 

int registra_nuevo_usuario(Jugador **jugadores, int *número_jugadores)
{
    Jugador nuevo_jugador;

    // El id del nuevo jugador se obtiene incrementando el número de jugadores.
    nuevo_jugador.id_jugador = *número_jugadores + 1;
    // Obtenemos el nombre, el usuario y la contraseña del nuevo jugador.
    lee_cadena("Nombre: ", nuevo_jugador.nombre_jugador, sizeof nuevo_jugador.nombre_jugador);
    lee_cadena("Usuario: ", nuevo_jugador.jugador, sizeof nuevo_jugador.jugador);
    // Buscamos si ese jugador ya está registrado (solo se comprueba el usuario, puede haber personas con el mismo nombre).
    if (busca_usuario(nuevo_jugador.jugador, *jugadores, *número_jugadores)) {
       return 0;
    } else {
        lee_cadena("Contraseña: ", nuevo_jugador.contraseña, sizeof nuevo_jugador.contraseña);
        // Inicialmente, el nuevo jugador no posee objetos.
        nuevo_jugador.id_objetos = NULL;
        nuevo_jugador.num_objetos = 0;
        // Añadimos el nuevo jugador.
        ++*número_jugadores;
        *jugadores = realloc(*jugadores, sizeof(Jugador) * *número_jugadores);  // Reservamos espacio para una estructura extra.
        (*jugadores)[*número_jugadores - 1] = nuevo_jugador;                    // Copiamos la estructura a la última posición.

        // PARA DEPURACIÓN. ELIMINAR ANTES DE ENTREGAR.
        muestra_usuarios(*jugadores, *número_jugadores);

        return 1;
    }
}

int registra_usuario(Jugador **jugadores, int *número_jugadores)
{
while (!respuesta_correcta) {
                lee_cadena("¿Desea registrar un nuevo usuario? [S/N] ", respuesta, sizeof(respuesta));
                switch (respuesta[0]) {
                    case 'n': case 'N':
                        respuesta_correcta = 1;
                        break;
                    case 's': case 'S':
                        respuesta_correcta = registra_nuevo_usuario(jugadores, número_jugadores);
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
    
partida->num_objetos = 0;
partida->num_conexiones = 0;
partida->num_puzles = 0;

void nuevaPartida(Partida *partida)
{

    FILE *f;
    char linea[100];

    f = fopen("Partida.txt", "r");

    if(f == NULL){
        printf("Error al abrir el fichero\n");
        return;
    }

    while(fgets(linea, sizeof(linea), f) != NULL){

        /* JUGADOR */
        if(strncmp(linea, "JUGADOR:", 8) == 0){

            sscanf(linea, "JUGADOR: %d", &partida->id_jugador);
        }

        /* SALA */
        else if(strncmp(linea, "SALA:", 5) == 0){

            sscanf(linea, "SALA: %d", &partida->id_sala_actual);
        }

        /* OBJETO */
        else if(strncmp(linea, "OBJETO:", 7) == 0){

            char id[10], loc[10];

            sscanf(linea, "OBJETO: %[^-]-%s", id, loc);

            strcpy(partida->lista_objetos[partida->num_objetos].id, id);
            strcpy(partida->lista_objetos[partida->num_objetos].localizacion, loc);

            partida->num_objetos++;
        }

        /* CONEXION */
        else if(strncmp(linea, "CONEXION:", 10) == 0){

            int id;
            char estado[10];

            sscanf(linea, "CONEXION: %d-%s", &id, estado);

            partida->lista_conexiones[partida->num_conexiones].id = id;
            strcpy(partida->lista_conexiones[partida->num_conexiones].estado, estado);

            partida->num_conexiones++;
        }

        /* PUZLE */
        else if(strncmp(linea, "PUZLE:", 6) == 0){

            char id[10], estado[15];

            sscanf(linea, "PUZLE: %[^-]-%s", id, estado);

            strcpy(partida->lista_puzles[partida->num_puzles].id, id);
            strcpy(partida->lista_puzles[partida->num_puzles].estado, estado);

            partida->num_puzles++;
        }

    }

    fclose(f);

    printf("Partida cargada correctamente\n");
}