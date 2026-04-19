#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "mapa.h"

//Modulo que se encarga de gestionar las acciones relacionadas con el mapa, como examinar las salidas de la sala actual o moverse a otra sala, gestionando las salas y las conexiones

static void AccionMover(int *id_sala_actual, Salas *sal, int *salas_abiertas, int num_salas_abiertas); // Declaración de la función para moverse a otra sala si la salida está abierta, se declara aquí porque solo se utiliza dentro de este módulo

void ExaminarSalidas(Conexiones *con, int num_conexiones, int *id_sala_actual, Salas *sal) { 
    // Función para examinar las salidas disponibles desde la sala actual del jugador
    // Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de examinar salidas en el menú de acciones del jugador
    // Postcondición: Se muestra la descripción de las conexiones que parten de la sala actual, indicando si están abiertas o bloqueadas.

    int sala_conectada; // Variable para almacenar la sala conectada a la sala actual a través de una conexión
    char respuesta; // Variable para almacenar la respuesta del jugador sobre si desea moverse a alguna de las salas conectadas a través de una conexión abierta
    
    int *salas_abiertas = NULL; // Array dinámico para almacenar las salas a las que se puede acceder desde la sala actual (conexiones abiertas)
    int num_salas_abiertas = 0; // Variable para contar el número de salas abiertas encontradas

    // MUESTRA LAS CONEXIONES QUE PARTEN DE LA SALA ACTUAL, INDICANDO SI ESTAN ABIERTAS O BLOQUEADAS
    for(int i = 0; i < num_conexiones; i++) {
        if(con[i].id_sala_orig == *id_sala_actual || con[i].id_sala_dest == *id_sala_actual) { 

            // Lógica de intercambio: Si mi sala es el origen, la sala conectada es el destino. Si mi sala es el destino, la sala conectada es el origen.
            if(con[i].id_sala_orig == *id_sala_actual) {  
                sala_conectada = con[i].id_sala_dest;
            } 
            else {
                sala_conectada = con[i].id_sala_orig; 
            }

            //COMPROBAMOS EL ESTADO DE LA CONEXION PARA MOSTRAR SI LA SALIDA ESTA ABIERTA O BLOQUEADA, Y SI ESTA ABIERTA SE GUARDA LA SALA CONECTADA EN UN ARRAY DINAMICO PARA LUEGO MOSTRARLE AL JUGADOR LAS OPCIONES DE MOVIMIENTO
            if(con[i].estado_conexion == 1) { 
                printf("Hacia sala %d - %s (abierta)\n", sala_conectada, sal[sala_conectada-1].nombre_sala);
                num_salas_abiertas++; // Aumentamos el contador de salas encontradas
                // Redimensionamos el array multiplicando el número de salas por el tamaño de un int
                salas_abiertas = realloc(salas_abiertas, num_salas_abiertas * sizeof(int));
                
                if(salas_abiertas == NULL) { // Comprobamos que la reasignación de memoria se ha realizado correctamente
                    printf("Error al asignar memoria para las salas abiertas.\n");
                    free(salas_abiertas); // Liberamos la memoria asignada antes de salir para evitar fugas de memoria
                    return; // Salimos de la función en caso de error
                }
                // Guardamos el ID de la sala en la última posición disponible

                salas_abiertas[num_salas_abiertas - 1] = sala_conectada; // Restamos 1 al ID de la sala para obtener el índice correcto en el array de salas
            } 
            else { 
                printf("Hacia sala %d - %s (bloqueada)\n", sala_conectada, sal[sala_conectada-1].nombre_sala);
            }
        }
    }  

    // SI SE HAN ENCONTRADO SALIDAS DESDE LA SALA ACTUAL, SE PREGUNTA AL JUGADOR SI DESEA MOVERSE A ALGUNA DE LAS SALAS CONECTADAS A TRAVÉS DE UNA CONEXION ABIERTA
    if (num_salas_abiertas == 0) {
        printf("No parece haber ninguna salida desde esta sala.\n");
    } 
    else { // Si es 1 (es decir, encontró salidas)
        do {
            printf("\n>>> Deseas moverte a alguna de estas salas? (S/N): ");
            
            scanf(" %c", &respuesta);
            while (getchar() != '\n'); // Limpiamos el buffer

            if (respuesta == 'S' || respuesta == 's') {
                AccionMover(id_sala_actual, sal, salas_abiertas, num_salas_abiertas); // Llama a la función para moverse a otra sala si la salida está abierta, pasando el array de salas abiertas y su tamaño
            } else if (respuesta == 'N' || respuesta == 'n') {
                printf("Decides no moverte por ahora.\n");
            } else {
                printf("Respuesta no valida. Introduce S o N.\n");
            }
        } while (respuesta != 'S' && respuesta != 's' && respuesta != 'N' && respuesta != 'n');
    }
    free(salas_abiertas); // Liberamos la memoria asignada al array de salas abiertas para evitar fugas de memoria
}


static void AccionMover(int *id_sala_actual, Salas *sal, int *salas_abiertas, int num_salas_abiertas) {
    // Función para moverse a otra sala si la salida está abierta
    // Precondición: Deben haber sido cargados los datos de la partida, haber sido seleccionada la opción de entrar a otra sala en el menú de acciones del jugador, y se ha comprobado que existen conexiones abiertas desde la sala actual.
    // Postcondición: Se cambia la sala actual del jugador a la sala seleccionada por el jugador entre las opciones de salas conectadas a través de conexiones abiertas.

    int nueva_sala; // Variable para almacenar la nueva sala a la que se desea mover el jugador
    int opcion_valida = 0; // Variable para controlar si se ha seleccionado una opción válida
    int aux = 0; // Variabe para salir del bucle cuando la sala seleccionada sea la correcta

    do {
        printf(">>> Ingresa el numero de la sala a la que deseas moverte: (0 para cancelar): ");
        if (scanf("%d", &nueva_sala) != 1) {
            printf("Entrada no valida. Por favor, ingresa un numero.\n");
            while (getchar() != '\n'); // Limpiamos el buffer
        } 
        else if (nueva_sala == 0) {
            while (getchar() != '\n'); // Limpiamos el buffer de entrada para evitar problemas con entradas posteriores
            printf("Movimiento cancelado...\n");
            opcion_valida = 1; // Establecemos que se ha seleccionado una opción válida para salir del bucle
        }
        else {
            while (getchar() != '\n'); // Limpiamos el buffer de entrada para evitar problemas con entradas posteriores
            // Comprobamos si la nueva sala ingresada por el jugador está entre las salas abiertas disponibles
            for (int i = 0; i < num_salas_abiertas && aux == 0; i++) {
                if (nueva_sala == salas_abiertas[i]) {
                    *id_sala_actual = nueva_sala; // Actualizamos la sala actual del jugador
                    printf("Te has movido a la sala %d - %s\n", nueva_sala, sal[nueva_sala-1].nombre_sala);
                    opcion_valida = 1; // Establecemos que se ha seleccionado una opción válida
                    aux = 1; // Establecemos que se ha encontrado la sala seleccionada para salir del bucle
                }
            }
            if (opcion_valida == 0) {
                printf("Opcion no valida. Por favor, selecciona una de las salas abiertas listadas anteriormente.\n");
            }
        }
    } while (opcion_valida == 0);
}

void DescribirSala(Salas *sal, int id_sala_actual) {
    // Precondición: Datos cargados e id_sala_actual válido.
    // Postcondición: Muestra información y espera a que el usuario decida salir.
    
    int salir = 0;
    int opcion;

        system("cls"); // Limpiamos pantalla para que solo se vea la descripción
        printf("==========================================\n");
        printf("ESTAS EN: %s\n", sal[id_sala_actual-1].nombre_sala);
        printf("==========================================\n");
        printf("%s\n\n", sal[id_sala_actual-1].descripcion_sala);

        printf("Pulse (1) para volver al menu de acciones: ");
    
    do {
        // Comprobación de seguridad del scanf
        if (scanf("%d", &opcion) != 1) {
            printf("Entrada no valida. Por favor, pulse 1.\n");
            while (getchar() != '\n');
            Sleep(1000);
        } else {
            if (opcion == 1) {
                salir = 1;
            } else {
                printf("Pulse 1 para salir.\n");
                Sleep(800);
            }
        }
    } while (salir == 0);
}

int ComprobarConexion(Conexiones *con, int k,int id_sala_actual, char *n){
//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de usar objetos o resolver puzles en el menú de acciones del jugador
//Postcondición: Se comprueba si la conexión con índice k en la lista de conexiones tiene como sala de origen la sala actual, tiene el estado de conexión a 0 (bloqueada) y tiene como condicionante el id del objeto o puzle que se está evaluando.
    if(con[k].id_sala_orig == id_sala_actual && con[k].estado_conexion == 0 && strcmp(con[k].id_condicionante, n) == 0){
        return 1; // La conexión está bloqueada por un objeto que se encuentra en el inventario del jugador
    }else{
        return 0; // La conexión no está bloqueada por un objeto que se encuentra en el inventario del jugador
    }
}

void AbrirConexion(Conexiones *con, int k){
//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de usar objetos o resolver puzles en el menú de acciones del jugador, y se ha comprobado que la conexión con índice k en la lista de conexiones tiene como sala de origen la sala actual, tiene el estado de conexión a 0 (bloqueada) y tiene como condicionante el id del objeto o puzle que se está evaluando.
//Postcondición: Se cambia el estado de la conexión con índice k en la lista de conexiones a 1 (abierta) y se muestra un mensaje indicando que se ha abierto la conexión hacia la sala de destino de dicha conexión.
     con[k].estado_conexion = 1; // Cambia el estado de la conexión a abierta
     printf("Has abierto la conexion hacia la sala %d\n", con[k].id_sala_dest);
}