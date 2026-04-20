#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "menus.h"      
#include "usuarios.h"    
#include "mapa.h"        
#include "condiciones.h" 
#include "ficheros.h"

static void menu_principal(Salas **sal, int *num_salas, int *id_sala_actual, Conexiones **con, int *num_conexiones, Puzles **puz, int *num_puzles, Objetos **obj, int *num_objetos, Jugadores **jug, int indice_jugador, Inventario *inv); 
static void menu_juego(Salas **sal, int *num_salas, int *id_sala_actual, Conexiones **con, int *num_conexiones, Puzles **puz, int *num_puzles, Objetos **obj, int *num_objetos, Jugadores **jug, int indice_jugador, Inventario *inv);
static void MostrarVictoria(int *salir, Jugadores **jug, int indice_jugador, int *id_sala_actual, Inventario *inv, Objetos **obj, int *num_objetos, Conexiones **con, int *num_conexiones, Puzles **puz, int *num_puzles);

//Precondición: Recibe punteros a las estructuras base del juego (salas, conexiones, puzles, objetos, jugadores e inventario) que ya han sido inicializadas con los datos de los ficheros. Estas estructuras se pasan por referencia para que cualquier modificación realizada en ellas dentro de los menús se refleje en el estado global del juego. El menú inicial es el punto de entrada al juego y desde aquí se accede al menú principal y al menú de juego. El flujo de datos se mantiene a través de la llamada a funciones y la manipulación directa de las estructuras base, asegurando que el estado del juego se actualice correctamente en función de las acciones del jugador.
//Postcondición: El menú inicial permite al jugador iniciar sesión, acceder al menú principal para comenzar una nueva partida o cargar una partida guardada, y desde el menú principal se accede al menú de juego donde se desarrolla la experiencia interactiva del escape room. A lo largo de estos menús, las estructuras base del juego se actualizan en función de las acciones del jugador, como moverse entre salas, interactuar con objetos, resolver puzles y guardar el progreso. El flujo de datos se mantiene coherente y actualizado en todo momento, permitiendo una experiencia de juego fluida y consistente.
void menu_inicial(Salas **sal, int *num_salas, int *id_sala_actual, Conexiones **con, int *num_conexiones, Puzles **puz, int *num_puzles, Objetos **obj, int *num_objetos, Jugadores **jug, int *num_jugadores, Inventario *inv){
    int control = 0;  // Variable para controlar la opción seleccionada en el menú inicial
    int salir = 0;  // Variable para controlar cuándo salir del menú inicial

    do{
        system("cls");
        printf("=================================\n");
        printf("    Bienvenido a ESI-ESCAPE\n");
        printf("=================================\n");
        printf("1. Iniciar sesion\n");
        printf("2. Salir\n");
        printf("Seleccione una opcion: ");
        if(scanf("%d", &control) != 1) {
            printf("Opcion no valida. Por favor, seleccione una opcion del menu.\n");
            while (getchar() != '\n'); // Limpia el buffer de entrada
            Sleep(1000); // Espera 1 segundo antes de mostrar el menú nuevamente
            system("cls");
        }
        while (getchar() != '\n'); // Limpia el buffer de entrada siempre

        switch(control){
        case 1: {
            int id_logueado = login(jug, num_jugadores); // Llama a la función de login, devuelve el id_jugador del usuario logueado
            if (id_logueado > -1) {
                // Buscar el índice del jugador logueado en el array
                int indice_jugador = -1;
                for (int i = 0; i < *num_jugadores; i++) {
                    if ((*jug)[i].id_jugador == id_logueado) {
                        indice_jugador = i;
                        break;
                    }
                }
                if (indice_jugador >= 0) {
                    printf("Inicio de sesion exitoso... \n");
                    Sleep(1500); // Espera 1.5 segundos
                    menu_principal(sal, num_salas, id_sala_actual, con, num_conexiones, puz, num_puzles, obj, num_objetos, jug, indice_jugador, inv);
                } else {
                    printf("Error interno: jugador no encontrado.\n");
                    Sleep(1000);
                }
            } else {
                printf("Inicio de sesion fallido... \n");
                Sleep(1500); // Espera 1.5 segundos
                system("cls");
            }
            break;
        }
        case 2:
            printf("Gracias por jugar. Hasta luego!\n");
            Sleep(1000); // Espera 1 segundo
            salir = 1; // Establece la variable salir a 1 para salir del menú inicial
            break;
        default:
            printf("Opcion no valida. Por favor, seleccione una opcion del menu.\n");
            Sleep(1000); // Espera 1 segundo
    }
    }while(salir == 0);
}

//Precondición: Recibe las estructuras base del juego actualizadas con el estado del jugador logueado. El menú principal permite al jugador iniciar una nueva partida, cargar una partida guardada o salir al inicio de sesión. Si el jugador elige iniciar una nueva partida, se llama al menú de juego con las estructuras base para comenzar la experiencia interactiva. Si elige cargar una partida, se verifica si existe una partida guardada para ese jugador y se carga el estado del juego antes de llamar al menú de juego. El flujo de datos se mantiene a través de la manipulación directa de las estructuras base, asegurando que cualquier cambio realizado en ellas dentro del menú principal se refleje en el estado global del juego.
//Postcondición: El menú principal permite al jugador acceder al menú de juego para desarrollar la experiencia del escape room, ya sea comenzando una nueva partida o cargando una partida guardada. A lo largo del menú principal, las estructuras base del juego se actualizan en función de las acciones del jugador, como iniciar una nueva partida o cargar una partida guardada, asegurando que el estado del juego se mantenga coherente y actualizado para la experiencia de juego. Desde el menú principal, el jugador puede acceder al menú de juego donde se desarrolla la experiencia interactiva del escape room.
static void menu_principal(Salas **sal, int *num_salas, int *id_sala_actual, Conexiones **con, int *num_conexiones, Puzles **puz, int *num_puzles, Objetos **obj, int *num_objetos, Jugadores **jug, int indice_jugador, Inventario *inv){
    int control = 0; // Variable para controlar la opción seleccionada en el menú principal
    int salir = 0; // Variable para controlar cuándo salir del menú principal

    do{
        system("cls");

        // CONDICION DE VICTORIA

        printf("=================================\n");
        printf("        Menu Principal\n");
        printf("=================================\n");
        printf("1. Nueva Partida\n");
        
        int hay_partida = existePartida((*jug)[indice_jugador].id_jugador); // Llama a la función para comprobar si existe una partida guardada para el jugador logueado, devuelve 1 si existe y 0 si no existe

        if (hay_partida) {
            printf("2. Cargar Partida\n");
        } else {
            printf("2. [No disponible] Cargar Partida\n");
        }
        printf("3. Salir al inicio de sesion\n");
        printf("Seleccione una opcion: ");
        if(scanf("%d", &control) != 1){ // Scanf devuelve el numero de exitos de lectura
            printf("Opcion no valida. Por favor, seleccione una opcion del menu.\n");
            while(getchar() != '\n'); // Limpia el buffer de entrada
            Sleep(1000); // Espera 1 segundo
            system("cls");
        }
        while(getchar() != '\n'); // Limpia el buffer de entrada siempre
        
        switch(control){
            case 1:
                printf("Iniciando nueva partida...\n");
                Sleep(1500); // Espera 1.5 segundos
                system("cls");
                // (3.1) FLUJO DE DATOS: Como las estructuras base ya están inicializadas con los datos de los ficheros, se llama directamente al menú de juego pasando las estructuras base
                menu_juego(sal, num_salas, id_sala_actual, con, num_conexiones, puz, num_puzles, obj, num_objetos, jug, indice_jugador, inv); // Pasamos directamente las estructuras base al menu de juego
                break;
            case 2:
                
            // (3.2) FLUJO DE DATOS: Se llama a la función para cargar una partida guardada, se comparan las estructuras base con la partida guardada y se actualizan las estructuras base con los datos de la partida guardada, incluyendo la sala actual del jugador
                if (!existePartida((*jug)[indice_jugador].id_jugador)) {
                    printf("No tienes ninguna partida guardada. Inicia una 'Nueva Partida' primero.\n");
                    Sleep(2000);
                    break;
                }
                printf("Cargando partida...\n");
                Sleep(1500); // Espera 1.5 segundos
                system("cls");
                
                if(cargarPartida(jug, indice_jugador, id_sala_actual, inv, obj, *num_objetos, con, *num_conexiones, puz, *num_puzles) == 0) { // Llama a la función para cargar una partida guardada y actualizar el estado del juego
                    printf("Error al cargar la partida.\n");
                    Sleep(1000); // Espera 1 segundo
                    system("cls");
                } else {
                    menu_juego(sal, num_salas, id_sala_actual, con, num_conexiones, puz, num_puzles, obj, num_objetos, jug, indice_jugador, inv); // Llama al menú de juego pasando las estructuras actualizadas con los datos de la partida cargada
                }
                break;
            case 3:
                printf("Gracias por jugar. Hasta luego!\n");
                Sleep(1000); // Espera 1 segundo 
                salir = 1; // Establece la variable salir a 1 para salir del menú principal
                break;
            default:
                printf("Opcion no valida. Por favor, seleccione una opcion del menu.\n");
                Sleep(1000); // Espera 1 segundo
        }
    }while(salir == 0);
}

//Precondición: Recibe las estructuras base del juego actualizadas con el estado del jugador logueado y la sala actual. El menú de juego es donde se desarrolla la experiencia interactiva del escape room, permitiendo al jugador realizar acciones como describir la sala, examinar objetos, coger y soltar objetos, usar objetos, resolver puzles, guardar la partida o salir al menú principal. A lo largo del menú de juego, las estructuras base del juego se actualizan en función de las acciones del jugador, asegurando que el estado del juego se mantenga coherente y actualizado para la experiencia de juego. Si el jugador alcanza la sala de salida (id_sala_actual == 10), se muestra un mensaje de victoria y se ofrecen opciones para guardar la partida o salir sin guardar.
//Postcondición: El menú de juego permite al jugador interactuar con el mundo del escape room, realizar acciones que afectan el estado del juego y avanzar hacia la condición de victoria. A lo largo del menú de juego, las estructuras base del juego se actualizan en función de las acciones del jugador, asegurando que el estado del juego se mantenga coherente y actualizado para la experiencia de juego. Si el jugador alcanza la sala de salida, se muestra un mensaje de victoria y se ofrecen opciones para guardar la partida o salir sin guardar, permitiendo al jugador finalizar su experiencia de juego de manera satisfactoria. Desde el menú de juego, el jugador puede regresar al menú principal para iniciar una nueva partida o cargar otra partida guardada.
static void menu_juego(Salas **sal, int *num_salas, int *id_sala_actual, Conexiones **con, int *num_conexiones, Puzles **puz, int *num_puzles, Objetos **obj, int *num_objetos, Jugadores **jug, int indice_jugador, Inventario *inv) {
    int control = 0;
    int salir = 0;
    int confirmacion = 0;

    do {
        system("cls");

        // CONDICION DE VICTORIA
        if (*id_sala_actual == 10) {
            MostrarVictoria(&salir, jug, indice_jugador, id_sala_actual, inv, obj, num_objetos, con, num_conexiones, puz, num_puzles);
        } 
        else {
            printf("=================================\n");
            printf("          Menu de Juego\n");
            printf("=================================\n");
            printf("1. Describir sala actual\n");
            printf("2. Examinar objetos\n");
            printf("3. Examinar salidas\n");
            printf("4. Coger objeto\n");
            printf("5. Soltar objeto\n");
            printf("6. Inventario\n");
            printf("7. Usar objeto\n");
            printf("8. Resolver puzle\n");
            printf("9. Guardar partida\n");
            printf("10. Salir al menu principal\n");
            printf("Seleccione una opcion: ");

            if (scanf("%d", &control) != 1) {
                printf("Opcion no valida. Por favor, seleccione una opcion del menu.\n");
                while (getchar() != '\n'); 
                Sleep(1000);
            }
            while (getchar() != '\n'); 

            switch (control) {
                case 1:
                    DescribirSala(*sal, *id_sala_actual);
                    system("pause"); // Pausa para leer la descripción
                    break;

                case 2:
                    system("cls");
                    printf("===== OBJETOS EN LA SALA ===== \n");
                    ExaminarObjeto(*obj, *num_objetos, *id_sala_actual);
                    Sleep(3000);
                    break;

                case 3:
                    system("cls");
                    printf("===== SALIDAS ===== \n");
                    ExaminarSalidas(*con, *num_conexiones, id_sala_actual, *sal);
                    Sleep(4000);
                    break;

                case 4:
                    system("cls");
                    printf("===== COGER OBJETO ===== \n");
                    CogerObjeto(inv, *obj, *num_objetos, *id_sala_actual);
                    Sleep(1500);
                    break;

                case 5:
                    system("cls");
                    printf("===== SOLTAR OBJETO ===== \n");
                    SoltarObjeto(inv, *obj, *num_objetos, *id_sala_actual);
                    Sleep(1500);
                    break;

                case 6:
                    system("cls");
                    printf("===== INVENTARIO ===== \n");
                    MostrarInventario(*obj, inv, *num_objetos);
                    Sleep(2500);
                    break;

                case 7:
                    system("cls");
                    printf("===== USAR OBJETO ===== \n");
                    UsarObjeto(*obj, inv, *num_conexiones, *con, *id_sala_actual);
                    Sleep(2500);
                    break;

                case 8:
                    system("cls");
                    printf("===== RESOLVER PUZLE ===== \n");
                    ResolverPuzle(*num_conexiones, *puz, *num_puzles, *con, *id_sala_actual);
                    Sleep(2500);
                    break;

                case 9:
                    printf("Guardando partida...\n");
                    Sleep(1500);
                    guardarPartida(jug, indice_jugador, id_sala_actual, inv, *obj, *con, *puz);
                    break;

                case 10:
                    printf("Seguro que quieres salir? Se perdera el progreso no guardado. (1: Si, 0: No): ");
                    do {
                        if (scanf("%d", &confirmacion) != 1) {
                            printf("Entrada no valida. Elija 1 o 0: ");
                            while (getchar() != '\n');
                            continue;
                        }
                        while (getchar() != '\n');

                        if (confirmacion == 1) {
                            salir = 1;
                            printf("Saliendo al menu principal...\n");
                            Sleep(1500);
                            reinicio(jug, indice_jugador, id_sala_actual, inv, obj, num_objetos, con, num_conexiones, puz, num_puzles);
                        } else if (confirmacion == 0) {
                            printf("Continuando partida...\n");
                            Sleep(1000);
                        }
                    } while (confirmacion != 0 && confirmacion != 1);
                    break;

                case 11: // OPCION SECRETA
                    printf("Modo Dios - Ingrese ID Sala: ");
                    scanf("%d", id_sala_actual);
                    while (getchar() != '\n');
                    break;

                default:
                    printf("Opcion no valida.\n");
                    Sleep(1500);
            } 
        }

    } while (salir == 0);
}

//Precondición: Recibe punteros a las estructuras base del juego actualizadas con el estado del jugador logueado y la sala actual. Esta función se llama cuando el jugador alcanza la sala de salida (id_sala_actual == 10) y muestra un mensaje de victoria, ofreciendo opciones para guardar la partida o salir sin guardar. El flujo de datos se mantiene a través de la manipulación directa de las estructuras base, asegurando que cualquier cambio realizado en ellas dentro de esta función se refleje en el estado global del juego.
//Postcondición: Si el jugador elige guardar la partida, se llama a la función para guardar el estado del juego, se actualizan las estructuras base con los datos de la partida guardada y se regresa al menú principal. Si el jugador elige salir sin guardar, se regresa al menú principal sin guardar el progreso. En ambos casos, se muestra un mensaje de salida y se reinician las estructuras base para preparar una nueva partida o cargar otra partida guardada desde el menú principal.
static void MostrarVictoria(int *salir, Jugadores **jug, int indice_jugador, int *id_sala_actual, Inventario *inv, Objetos **obj, int *num_objetos, Conexiones **con, int *num_conexiones, Puzles **puz, int *num_puzles) {
    int control = 0;

    system("cls");
    printf("===================================\n");
    printf("|         ¡FELICIDADES!           |\n");        
    printf("| Has alcanzado la sala de salida |\n");
    printf("|          ¡HAS GANADO!           |\n");
    printf("===================================\n\n");
    
    printf("1. Guardar y salir al menu principal\n");
    printf("2. Salir sin guardar\n");
    printf("Seleccione una opcion: ");

    do {
        if (scanf("%d", &control) != 1) {
            while (getchar() != '\n'); 
            printf("Opcion no valida. Intente de nuevo: ");
            continue;
        }
        while (getchar() != '\n');

        if (control == 1) {
            printf("\nGuardando tu victoria...\n");
            Sleep(1500);
            
            // Misma logica que en menu de juego
            guardarPartida(jug, indice_jugador, id_sala_actual, inv, *obj, *con, *puz);
            
            reinicio(jug, indice_jugador, id_sala_actual, inv, obj, num_objetos, con, num_conexiones, puz, num_puzles);
            
            *salir = 1; // Establecemos salir a 1 para salir del menu de juego y volver al menu principal
        } 
        else if (control == 2) {
            printf("\nSaliendo al menu principal...\n");
            Sleep(1500);
            
            // Llamada idéntica a la del menu_juego
            reinicio(jug, indice_jugador, id_sala_actual, inv, obj, num_objetos, con, num_conexiones, puz, num_puzles);
            
            *salir = 1;
        } 
        else {
            printf("Por favor, elija 1 o 2: ");
        }

    } while (control != 1 && control != 2);

    system("cls");
}