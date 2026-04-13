#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "menus.h"      
#include "usuarios.h"    
#include "mapa.h"        
#include "condiciones.h" 
#include "ficheros.h"

static void menu_principal(Partida **par, Jugadores *jug_login, Objetos **obj, Salas **sal, Conexiones **con, Puzles **puz);
static void menu_juego(Partida **par, Jugadores *jug_login, Objetos **obj, Salas **sal, Conexiones **con, Puzles **puz);

void menu_inicial(Partida **par, Jugadores **jug, Objetos **obj, Salas **sal, Conexiones **con, Puzles **puz, int* num_jugadores){
    int control = 0;  // Variable para controlar la opción seleccionada en el menú inicial
    int salir = 0;  // Variable para controlar cuándo salir del menú inicial
    Jugadores *jug_login = NULL; // Puntero para almacenar el jugador que inicia sesión

    do{        system("cls");        printf("=================================\n");
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
        case 1:
            jug_login = login(jug, num_jugadores);
            if (jug_login != NULL) { // Llama a la función de login y verifica si el inicio de sesión fue exitoso
                printf("Inicio de sesion exitoso... \n");
                Sleep(1500); // Espera 1.5 segundos
                menu_principal(par, jug_login, obj, sal, con, puz); // Llama a la función para mostrar el menú principal y gestionar las opciones del juego
            }
            else{
                printf("Inicio de sesion fallido... \n");
                Sleep(1500); // Espera 1.5 segundos
                system("cls");
                }
            break;
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


static void menu_principal(Partida **par, Jugadores *jug_login, Objetos **obj, Salas **sal, Conexiones **con, Puzles **puz){
    int control = 0; // Variable para controlar la opción seleccionada en el menú principal
    int salir = 0; // Variable para controlar cuándo salir del menú principal
    do{
        system("cls");
        printf("=================================\n");
        printf("        Menu Principal\n");
        printf("=================================\n");
        printf("1. Nueva Partida\n");
        printf("2. Cargar Partida\n");
        printf("3. Salir del juego\n");
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
                printf("Iniciando nueva partida...\n");
                Sleep(1500); // Espera 1.5 segundos
                system("cls");

                if (*par == NULL) {
                    *par = malloc(sizeof(Partida));
                }
                borrarPartida(jug_login->id_jugador);
                if (cargarPartida(*par, jug_login->id_jugador)) {
                    menu_juego(par, jug_login, obj, sal, con, puz);
                } else {
                    printf("Error al iniciar nueva partida.\n");
                    Sleep(1500);
                }
                break;
            case 2:
                printf("Cargando partida...\n");
                Sleep(1500); // Espera 1.5 segundos
                system("cls");
                
                if (*par == NULL) {
                    *par = malloc(sizeof(Partida));
                }
                if (cargarPartida(*par, jug_login->id_jugador)) {
                    menu_juego(par, jug_login, obj, sal, con, puz);
                } else {
                    printf("No se pudo cargar la partida.\n");
                    Sleep(1500);
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


static void menu_juego(Partida **par, Jugadores *jug_login, Objetos **obj, Salas **sal, Conexiones **con, Puzles **puz){
    int control = 0; // Variable para controlar la opción seleccionada en el menú de juego
    int salir = 0; // Variable para controlar cuándo salir del menú de juego
    do{
        system("cls");
        printf("=================================\n");
        printf("          Menu de Juego\n");
        printf("=================================\n");
        printf("1. Describir sala\n");
        printf("2. Examinar objetos\n");
        printf("3. Entrar en otra sala\n");
        printf("4. Coger objeto\n");
        printf("5. Soltar objeto\n");
        printf("6. Inventario\n");
        printf("7. Usar objeto\n");
        printf("8. Resolver puzle\n");
        printf("9. Guardar partida\n");
        printf("10. Salir al menu principal\n");
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
                describirSala(*sal, *par); // Llama a la función para describir la sala actual del jugador
                break;
            case 2:
                system("cls");
                printf("===== OBJETOS EN LA SALA ===== \n");
                ExaminarObjeto(*par); // Llama a la función para examinar los objetos presentes en la sala actual
                Sleep(1000); // Espera 1 segundo
                break;
            case 3:
                system("cls");
                printf("===== SALIDAS ===== \n");
                ExaminarSalidas(*par); // Llama a la función para examinar las salidas disponibles desde la sala actual
                Sleep(1000); // Espera 1 segundo
                break;
            case 4:
                system("cls");
                printf("===== COGER OBJETO ===== \n");
                CogerObjeto(jug_login, *par); // Llama a la función para coger un objeto de la sala actual y añadirlo al inventario del jugador
                Sleep(1000); // Espera 1 segundo
                break;
            case 5:
                system("cls");
                printf("===== SOLTAR OBJETO ===== \n");
                SoltarObjeto(jug_login, *par); // Llama a la función para soltar un objeto del inventario del jugador y dejarlo en la sala actual
                Sleep(1000); // Espera 1 segundo
                break;
            case 6:
                system("cls");
                printf("===== INVENTARIO ===== \n");
                Inventario(*par); // Llama a la función para mostrar el inventario del jugador
                Sleep(1000); // Espera 1 segundo
                break;
            case 7:
                system("cls");
                printf("===== USAR OBJETO ===== \n");
                UsarObjeto(*par); // Llama a la función para usar un objeto del inventario del jugador y aplicar su efecto en la situación actual
                Sleep(1000); // Espera 1 segundo
                break;
            case 8:
                system("cls");
                printf("===== RESOLVER PUZLE ===== \n");
                ResolverPuzle(*par); // Llama a la función para resolver un puzle presente en la sala actual del jugador
                Sleep(1000); // Espera 1 segundo
                break;
            case 9:
                printf("Guardando partida...\n");
                Sleep(1500); // Espera 1.5 segundos
                guardarPartida(*par); // Llama a la función para guardar el estado actual de la partida
                break;
            case 10:
                printf("Saliendo al menu principal...\n");
                Sleep(1000); // Espera 1 segundo
                salir = 1; // Establece la variable salir a 1 para salir del menú de juego
                break;
            default:
                printf("Opcion no valida. Por favor, seleccione una opcion del menu.\n");
                Sleep(1000); // Espera 1 segundo
        }
    }while(salir == 0);
}
