#include <stdio.h>
#include <stdlib.h>
#include "menus.h"      
#include "usuarios.h"    
#include "mapa.h"        
#include "condiciones.h" 
#include "ficheros.h"

void menu_inicial(Partida **par, Jugadores **jug, Objetos **obj, Salas **sal, Conexiones **con, Puzles **puz, int* num_jugadores);
static void menu_principal(Partida **par, Jugadores **jug, Objetos **obj, Salas **sal, Conexiones **con, Puzles **puz);
static void menu_juego(Partida **par, Jugadores **jug, Objetos **obj, Salas **sal, Conexiones **con, Puzles **puz);


void menu_inicial(Partida **par, Jugadores **jug, Objetos **obj, Salas **sal, Conexiones **con, Puzles **puz, int* num_jugadores){
    int control = 0;  // Variable para controlar la opción seleccionada en el menú inicial
    int salir = 0;  // Variable para controlar cuándo salir del menú inicial
    do{
        printf("=================================\n");
        printf("    Bienvenido a ESI-ESCAPE\n");
        printf("=================================\n");
        printf("1. Iniciar sesión\n");
        printf("2. Salir\n");
        printf("Seleccione una opción: ");
        if(scanf("%d", &control) != 1) {
            printf("Opción no válida. Por favor, seleccione una opción del menú.\n");
        }
        while (getchar() != '\n'); // Limpia el buffer de entrada siempre

        switch(control){
        case 1:
        /* EL 3 ESTA HARDCODEADO NECESITAMOS EL NUMERO DE JUGADORES */


            Jugadores *jug_login = login(jug, num_jugadores);
            if (jug_login != NULL) { // Llama a la función de login y verifica si el inicio de sesión fue exitoso
                printf("Inicio de sesión exitoso... \n");
                menu_principal(par, jug, obj, sal, con, puz); // Llama a la función para mostrar el menú principal y gestionar las opciones del juego
            }
            else{
                printf("Inicio de sesión fallido... \n");
                }
            break;
        case 2:
            printf("Gracias por jugar. ¡Hasta luego!\n");
            salir = 1; // Establece la variable salir a 1 para salir del menú inicial
            break;
        default:
            printf("Opción no válida. Por favor, seleccione una opción del menú.\n");
    }
    }while(salir == 0);
}


static void menu_principal(Partida **par, Jugadores **jug, Objetos **obj, Salas **sal, Conexiones **con, Puzles **puz){
    int control = 0; // Variable para controlar la opción seleccionada en el menú principal
    int salir = 0; // Variable para controlar cuándo salir del menú principal
    do{
        printf("=================================\n");
        printf("        Menú Principal\n");
        printf("=================================\n");
        printf("1. Nueva Partida\n");
        printf("2. Cargar Partida\n");
        printf("3. Salir del juego\n");
        printf("Seleccione una opción: ");
        if(scanf("%d", &control) != 1){
            printf("Opción no válida. Por favor, seleccione una opción del menú.\n");
            while(getchar() != '\n'); // Limpia el buffer de entrada
        }
        
        switch(control){
            case 1:
                printf("Iniciando nueva partida...\n");

/* NO EXISTE NINGUNA FUNCIÓN PARA INICIAR NUEVA PARTIDA AÚN */

                // iniciarNuevaPartida(par); // Llama a la función para iniciar una nueva partida
                menu_juego(par, jug, obj, sal, con, puz); // Llama al menú de juego para gestionar las acciones dentro de la partida
                break;
            case 2:
                printf("Cargando partida...\n");
                // cargarPartida(*par, id_jugador); // Llama a la función para cargar una partida existente (se le pasará el ID del jugador 
                // que ha iniciado sesión) AUN NO TENGO EL ID TENGO QUE REQUERIRLO EN EL LOGIN
                break;
            case 3:
                printf("Gracias por jugar. ¡Hasta luego!\n");
                salir = 1; // Establece la variable salir a 1 para salir del menú principal
                break;
            default:
                printf("Opción no válida. Por favor, seleccione una opción del menú.\n");
        }
    }while(salir == 0);
}


static void menu_juego(Partida **par, Jugadores **jug, Objetos **obj, Salas **sal, Conexiones **con, Puzles **puz){
    int control = 0; // Variable para controlar la opción seleccionada en el menú de juego
    int salir = 0; // Variable para controlar cuándo salir del menú de juego
    do{
        printf("=================================\n");
        printf("          Menú de Juego\n");
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
        printf("10. Salir al menú principal\n");
        printf("Seleccione una opción: ");
        if(scanf("%d", &control) != 1){
            printf("Opción no válida. Por favor, seleccione una opción del menú.\n");
            while(getchar() != '\n'); // Limpia el buffer de entrada
        }

        switch(control){
            case 1:
                // describirSala(); // Llama a la función para describir la sala actual del jugador
                break;
            case 2:
                printf("===== OBJETOS EN LA SALA ===== \n");
                ExaminarObjeto(*par); // Llama a la función para examinar los objetos presentes en la sala actual
                break;
            case 3:
                printf("===== SALIDAS ===== \n");
                ExaminarSalidas(*par); // Llama a la función para examinar las salidas disponibles desde la sala actual
                break;
            case 4:
                printf("===== COGER OBJETO ===== \n");
                CogerObjeto(*jug,*par); // Llama a la función para coger un objeto de la sala actual y añadirlo al inventario del jugador
                break;
            case 5:
                printf("===== SOLTAR OBJETO ===== \n");
                SoltarObjeto(*jug, *par); // Llama a la función para soltar un objeto del inventario del jugador y dejarlo en la sala actual
                break;
            case 6:
                printf("===== INVENTARIO ===== \n");
                Inventario(*par); // Llama a la función para mostrar el inventario del jugador
                break;
            case 7:
                printf("===== USAR OBJETO ===== \n");
                UsarObjeto(*par); // Llama a la función para usar un objeto del inventario del jugador y aplicar su efecto en la situación actual
                break;
            case 8:
                printf("===== RESOLVER PUZLE ===== \n");
                ResolverPuzle(*par); // Llama a la función para resolver un puzle presente en la sala actual del jugador
                break;
            case 9:
                printf("Guardando partida...\n");
                guardarPartida(*par, *jug); // Llama a la función para guardar el estado actual de la partida (se le pasará la estructura de la partida y del jugador para guardar su progreso)
                break;
            case 10:
                printf("Saliendo al menú principal...\n");
                salir = 1; // Establece la variable salir a 1 para salir del menú de juego
                break;
            default:
                printf("Opción no válida. Por favor, seleccione una opción del menú.\n");
        }
    }while(salir == 0);
}
