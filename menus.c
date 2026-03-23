#include <stdio.h>
#include <stdlib.h>

#include "usuarios.h"
#include "juego.h"

void menu_inicial() {
    int opcion;
    
    printf("---------------------------------\n");
    printf("    Bienvenido a ESI-ESCAPE\n");
    printf("---------------------------------\n");
    printf("1. Iniciar sesión\n");
    printf("2. Salir\n");
    printf("Seleccione una opción: ");

    scanf("%d", &opcion);

    switch(opcion) {
        case 1:
            login(); // Llama a la función de login para gestionar el acceso de los jugadores
            break;
        case 2:
            printf("Gracias por jugar. ¡Hasta luego!\n");
            exit(0);
        default:
            printf("Opción no válida. Por favor, seleccione una opción del menú.\n");
    }

}