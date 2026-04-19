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


static void menu_principal(Salas **sal, int *num_salas, int *id_sala_actual, Conexiones **con, int *num_conexiones, Puzles **puz, int *num_puzles, Objetos **obj, int *num_objetos, Jugadores **jug, int indice_jugador, Inventario *inv){
    int control = 0; // Variable para controlar la opción seleccionada en el menú principal
    int salir = 0; // Variable para controlar cuándo salir del menú principal

    do{
        system("cls");
        printf("=================================\n");
        printf("        Menu Principal\n");
        printf("=================================\n");
        printf("1. Nueva Partida\n");
        printf("2. Cargar Partida\n");
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
                printf("Cargando partida...\n");
                Sleep(1500); // Espera 1.5 segundos
                system("cls");
                // (3.2) FLUJO DE DATOS: Se llama a la función para cargar una partida guardada, se comparan las estructuras base con la partida guardada y se actualizan las estructuras base con los datos de la partida guardada, incluyendo la sala actual del jugador
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
                // (3.3) FLUJO DE DATOS: Se llama a la función para reiniciar el estado de la partida actual, se liberan las estructuras dinámicas y se vuelven a inicializar con los datos de los ficheros para que el jugador pueda iniciar sesión con otro usuario o iniciar una nueva partida sin salir del programa
                reinicio(jug, indice_jugador, id_sala_actual, inv, obj, num_objetos, con, num_conexiones, puz, num_puzles); 
                salir = 1; // Establece la variable salir a 1 para salir del menú principal
                break;
            default:
                printf("Opcion no valida. Por favor, seleccione una opcion del menu.\n");
                Sleep(1000); // Espera 1 segundo
        }
    }while(salir == 0);
}


static void menu_juego(Salas **sal, int *num_salas, int *id_sala_actual, Conexiones **con, int *num_conexiones, Puzles **puz, int *num_puzles, Objetos **obj, int *num_objetos, Jugadores **jug, int indice_jugador, Inventario *inv){
    int control = 0; // Variable para controlar la opción seleccionada en el menú de juego
    int salir = 0; // Variable para controlar cuándo salir del menú de juego
    do{
        system("cls");
        printf("=================================\n");
        printf("          Menu de Juego\n");
        printf("=================================\n");
        printf("1. Describir sala actual\n");
        printf("2. Examinar objetos\n");
        printf("3. Examinar salidas\n");
        printf("4. Entrar a otra sala\n");
        printf("5. Coger objeto\n");
        printf("6. Soltar objeto\n");
        printf("7. Inventario\n");
        printf("8. Usar objeto\n");
        printf("9. Resolver puzle\n");
        printf("10. Guardar partida\n");
        printf("11. Salir al menu principal\n");
        printf("Seleccione una opcion: ");
        if(scanf("%d", &control) != 1){
            printf("Opcion no valida. Por favor, seleccione una opcion del menu.\n");
            while(getchar() != '\n'); // Limpia el buffer de entrada
            Sleep(1000); // Espera 1 segundo
            system("cls");
        }
        while(getchar() != '\n'); // Limpia el buffer de entrada siempre

        switch(control){
            case 1:
                DescribirSala(*sal, *id_sala_actual); // Llama a la función para describir la sala actual del jugador
                Sleep(1500); // Espera 1.5 segundos
                system("cls");
                break;
            case 2:
                system("cls");
                printf("===== OBJETOS EN LA SALA ===== \n");
                ExaminarObjeto(*obj, *num_objetos, *id_sala_actual); // Llama a la función para examinar los objetos presentes en la sala actual
                Sleep(1500); // Espera 1.5 segundos
                system("cls");
                break;
            case 3:
                system("cls");
                printf("===== SALIDAS ===== \n");
                ExaminarSalidas(*con, *num_conexiones, *id_sala_actual, *sal); // Llama a la función para examinar las salidas disponibles desde la sala actual
                Sleep(1500); // Espera 1.5 segundos
                system("cls");
                break;
            case 4:
                system("cls");
                printf("===== ENTRAR A OTRA SALA ===== \n");
                AccionMover(*con, *num_conexiones, id_sala_actual, *sal); // Llama a la función para moverse a otra sala si la salida está abierta
                Sleep(1500); // Espera 1.5 segundos
                system("cls");
                break;
            case 5:
                system("cls");
                printf("===== COGER OBJETO ===== \n");
                CogerObjeto(inv, *obj, *num_objetos, *id_sala_actual); // Llama a la función para coger un objeto de la sala actual y añadirlo al inventario del jugador
                Sleep(1500); // Espera 1.5 segundos
                system("cls");
                break;
            case 6:
                system("cls");
                printf("===== SOLTAR OBJETO ===== \n");
                SoltarObjeto(inv, *obj, *num_objetos, *id_sala_actual); // Llama a la función para soltar un objeto del inventario del jugador y dejarlo en la sala actual
                Sleep(1500); // Espera 1.5 segundos
                system("cls");
                break;
            case 7:
                system("cls");
                printf("===== INVENTARIO ===== \n");
                MostrarInventario(*obj, inv, *num_objetos); // Llama a la función para mostrar el inventario del jugador
                Sleep(1500); // Espera 1.5 segundos
                system("cls");
                break;
            case 8:
                system("cls");
                printf("===== USAR OBJETO ===== \n");
                UsarObjeto(*obj, inv, *num_conexiones, *con, *id_sala_actual); // Llama a la función para usar un objeto del inventario del jugador y aplicar su efecto en la situación actual
                Sleep(1500); // Espera 1.5 segundos
                system("cls");
                break;
            case 9:
                system("cls");
                printf("===== RESOLVER PUZLE ===== \n");
                ResolverPuzle(*num_conexiones, *puz, *num_puzles, *con, *id_sala_actual); // Llama a la función para resolver un puzle presente en la sala actual del jugador
                Sleep(1500); // Espera 1.5 segundos
                system("cls");
                break;
            case 10:
                printf("Guardando partida...\n");
                Sleep(1500); // Espera 1.5 segundos
                guardarPartida(jug, indice_jugador, id_sala_actual, inv, *obj, *con, *puz); // Llama a la función para guardar el estado actual de la partida
                system("cls");
                break;
            case 11:
                printf("Saliendo al menu principal...\n");
                Sleep(1500); // Espera 1.5 segundos
                reinicio(jug, indice_jugador, id_sala_actual, inv, obj, num_objetos, con, num_conexiones, puz, num_puzles); 
                salir = 1; // Establece la variable salir a 1 para salir del menú de juego
                system("cls");
                break;
            default:
                printf("Opcion no valida. Por favor, seleccione una opcion del menu.\n");
                Sleep(1500); // Espera 1.5 segundos
        }
    }while(salir == 0);
}