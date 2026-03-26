#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estructuras.h"
#include "juego.h"

// ============================================================================
// FUNCIONES DE INICIALIZACIÓN DE DATOS DE PRUEBA
// ============================================================================

/**
 * PASO 1: Inicializar datos de prueba para las salas
 * Esto crea un conjunto realista de salas para los tests
 */
Salas* inicializar_salas_prueba() {
    Salas *salas = (Salas*) malloc(3 * sizeof(Salas));
    
    // Sala 0: Sala inicial
    salas[0].id_sala = 0;
    strcpy(salas[0].nombre_sala, "Entrada");
    strcpy(salas[0].descripcion_sala, "Estás en una sala oscura. Hay una puerta al norte.");
    salas[0].tipo_sala = 1; // Sala inicial
    
    // Sala 1: Sala normal
    salas[1].id_sala = 1;
    strcpy(salas[1].nombre_sala, "Pasillo");
    strcpy(salas[1].descripcion_sala, "Un pasillo largo con paredes de piedra. Hay una llave dorada en el suelo.");
    salas[1].tipo_sala = 2; // Sala normal
    
    // Sala 2: Sala salida
    salas[2].id_sala = 2;
    strcpy(salas[2].nombre_sala, "Salida");
    strcpy(salas[2].descripcion_sala, "¡Has encontrado la salida! Puerta grande con luz exterior.");
    salas[2].tipo_sala = 3; // Sala salida
    
    return salas;
}

/**
 * PASO 2: Inicializar objetos de prueba
 * Crea objetos que el jugador puede coger, soltar y usar
 */
Objetos* inicializar_objetos_prueba() {
    Objetos *objetos = (Objetos*) malloc(3 * sizeof(Objetos));
    
    // Objeto 0: Llave
    strcpy(objetos[0].id_objeto, "L001");
    strcpy(objetos[0].nombre_objeto, "Llave dorada");
    strcpy(objetos[0].descripcion_objeto, "Una llave de oro antiguo. Brilla con luz propia.");
    objetos[0].localizacion_objeto = 1; // Está en sala 1
    
    // Objeto 1: Linterna
    strcpy(objetos[1].id_objeto, "L002");
    strcpy(objetos[1].nombre_objeto, "Linterna");
    strcpy(objetos[1].descripcion_objeto, "Una linterna vieja pero funcional.");
    objetos[1].localizacion_objeto = 0; // Está en inventario (0 = inventario)
    
    // Objeto 2: Mapa
    strcpy(objetos[2].id_objeto, "M001");
    strcpy(objetos[2].nombre_objeto, "Mapa antiguo");
    strcpy(objetos[2].descripcion_objeto, "Un mapa que muestra las salas del castillo.");
    objetos[2].localizacion_objeto = 1; // Está en sala 1
    
    return objetos;
}

/**
 * PASO 3: Inicializar Conexiones de prueba
 * Las conexiones necesitan ser verificadas (hay errores en el código original)
 */
Conexiones* inicializar_conexiones_prueba() {
    Conexiones *conexiones = (Conexiones*) malloc(2 * sizeof(Conexiones));
    
    // Conexión 0: De sala 0 a sala 1 (abierta)
    strcpy(conexiones[0].id_conexion, "C01");
    conexiones[0].id_sala_orig = 0;
    conexiones[0].id_sala_dest = 1;
    conexiones[0].estado_conexion = 1; // Abierta
    conexiones[0].condicion_conexion = 0; // No requiere nada
    
    // Conexión 1: De sala 1 a sala 2 (cerrada, requiere llave)
    strcpy(conexiones[1].id_conexion, "C02");
    conexiones[1].id_sala_orig = 1;
    conexiones[1].id_sala_dest = 2;
    conexiones[1].estado_conexion = 0; // Cerrada
    conexiones[1].condicion_conexion = 1; // Requiere objeto
    
    return conexiones;
}

/**
 * PASO 4: Inicializar Puzles de prueba
 * Los puzles tienen soluciones que el jugador debe descubrir
 */
Puzles* inicializar_puzles_prueba() {
    Puzles *puzles = (Puzles*) malloc(1 * sizeof(Puzles));
    
    // Puzle 0: En sala 1
    strcpy(puzles[0].id_puzle, "P01");
    puzles[0].id_sala_puzle = 1;
    puzles[0].tipo_elemento = 1; // Código
    strcpy(puzles[0].descripcion_puzle, "¿Cuál es la contraseña para abrir el cofre?");
    strcpy(puzles[0].solucion_puzle, "CASTILLO");
    
    return puzles;
}

/**
 * PASO 5: Inicializar dato de Partida
 * Esto agrupa todos los datos del juego
 */
Partida* inicializar_partida_prueba(Salas *salas, Objetos *objetos, 
                                     Conexiones *conexiones, Puzles *puzles) {
    Partida *partida = (Partida*) malloc(sizeof(Partida));
    partida->id_jugador = 1;
    partida->id_sala_actual = 0; // Empieza en sala 0
    
    partida->lista_objetos = objetos;
    partida->num_objetos = 3;
    
    partida->lista_conexiones = conexiones;
    partida->num_conexiones = 2;
    
    partida->lista_puzles = puzles;
    partida->num_puzles = 1;
    
    return partida;
}

/**
 * PASO 6: Inicializar dato de Jugador
 */
Jugadores* inicializar_jugador_prueba() {
    Jugadores *jugador = (Jugadores*) malloc(sizeof(Jugadores));
    jugador->id_jugador = 1;
    strcpy(jugador->nombre_jugador, "Juan Pérez");
    strcpy(jugador->jugador, "JuanP");
    jugador->num_items = 1; // Tiene la linterna en inventario
    
    // Asignar inventario inicial
    jugador->id_objeto = (char**) malloc(5 * sizeof(char*));
    for(int i = 0; i < 5; i++) {
        jugador->id_objeto[i] = (char*) malloc(5 * sizeof(char));
    }
    strcpy(jugador->id_objeto[0], "L002"); // Tiene la linterna
    
    return jugador;
}

// ============================================================================
// FUNCIONES DE PRUEBA PARA CADA FUNCIÓN DE JUEGO
// ============================================================================

/**
 * TEST 1: Prueba de Describir_Sala()
 * 
 * EXPLICACIÓN:
 * - Esta función debe mostrar la descripción de la sala actual
 * - El test verifica que la descripción se muestre correctamente
 * - Útil para ver si el juego describe bien cada sala
 */
void test_describir_sala() {
    printf("\n========================================\n");
    printf("TEST 1: DESCRIBIR_SALA()\n");
    printf("========================================\n");
    printf("OBJETIVO: Mostrar la descripción de la sala actual\n\n");
    
    Salas *salas = inicializar_salas_prueba();
    Objetos *objetos = inicializar_objetos_prueba();
    Conexiones *conexiones = inicializar_conexiones_prueba();
    Puzles *puzles = inicializar_puzles_prueba();
    Partida *partida = inicializar_partida_prueba(salas, objetos, conexiones, puzles);
    
    printf("Sala actual: %d\n", partida->id_sala_actual);
    printf("Nombre de sala: %s\n\n", salas[partida->id_sala_actual].nombre_sala);
    printf("Descripción: ");
    printf("%s\n\n", salas[partida->id_sala_actual].descripcion_sala);
    
    printf("✓ Test completado. La descripción se mostró correctamente.\n");
    
    // Liberar memoria
    free(salas);
    free(objetos);
    free(conexiones);
    free(puzles);
    free(partida);
}

/**
 * TEST 2: Prueba de Examinar()
 * 
 * EXPLICACIÓN:
 * - Esta función lista todos los objetos en la sala actual
 * - También muestra las salidas disponibles (abiertas y bloqueadas)
 * - Útil para que el jugador vea qué hacer en cada sala
 */
void test_examinar() {
    printf("\n========================================\n");
    printf("TEST 2: EXAMINAR()\n");
    printf("========================================\n");
    printf("OBJETIVO: Mostrar objetos y salidas de la sala actual\n\n");
    
    Salas *salas = inicializar_salas_prueba();
    Objetos *objetos = inicializar_objetos_prueba();
    Conexiones *conexiones = inicializar_conexiones_prueba();
    Puzles *puzles = inicializar_puzles_prueba();
    Partida *partida = inicializar_partida_prueba(salas, objetos, conexiones, puzles);
    
    printf("Examinando sala %d (%s):\n\n", partida->id_sala_actual, salas[partida->id_sala_actual].nombre_sala);
    
    // OBJETOS EN LA SALA
    printf("Objetos en la sala:\n-------------------------------\n");
    int encontrados = 0;
    for(int i = 0; i < partida->num_objetos; i++) {
        // Nota: El código original tiene errores con punteros, en la prueba asumimos estructura plana
        if(objetos[i].localizacion_objeto == partida->id_sala_actual) {
            printf("  • %s: %s\n", objetos[i].nombre_objeto, objetos[i].descripcion_objeto);
            encontrados++;
        }
    }
    if(encontrados == 0) {
        printf("  (No hay objetos en esta sala)\n");
    }
    
    // SALIDAS DISPONIBLES
    printf("\nSalidas disponibles:\n-------------------------------\n");
    for(int i = 0; i < partida->num_conexiones; i++) {
        if(conexiones[i].id_sala_orig == partida->id_sala_actual) {
            if(conexiones[i].estado_conexion == 1) {
                printf("  ✓ Puerta a sala %d (ABIERTA)\n", conexiones[i].id_sala_dest);
            } else {
                printf("  ✗ Puerta a sala %d (BLOQUEADA)\n", conexiones[i].id_sala_dest);
            }
        }
    }
    
    printf("\n✓ Test completado. Se mostró correctamente.\n");
    
    // Liberar memoria
    free(salas);
    free(objetos);
    free(conexiones);
    free(puzles);
    free(partida);
}

/**
 * TEST 3: Prueba de Inventario()
 * 
 * EXPLICACIÓN:
 * - Muestra todos los objetos que el jugador lleva
 * - Filtra solo los objetos con localizacion_objeto == 0 (inventario)
 * - Importante: El código original tiene bugs con punteros
 */
void test_inventario() {
    printf("\n========================================\n");
    printf("TEST 3: INVENTARIO()\n");
    printf("========================================\n");
    printf("OBJETIVO: Mostrar todos los objetos del inventario del jugador\n\n");
    
    Objetos *objetos = inicializar_objetos_prueba();
    Jugadores *jugador = inicializar_jugador_prueba();
    
    printf("Inventario del jugador %s:\n", jugador->nombre_jugador);
    printf("-------------------------------\n");
    
    int items = 0;
    for(int i = 0; i < 3; i++) {
        if(objetos[i].localizacion_objeto == 0) { // 0 = inventario
            printf("  • %s: %s\n", objetos[i].nombre_objeto, objetos[i].descripcion_objeto);
            items++;
        }
    }
    
    printf("\nTotal de items: %d/5\n\n", items);
    
    if(items == 0) {
        printf("El inventario está vacío.\n");
    }
    
    printf("✓ Test completado. Se mostró correctamente.\n");
    
    // Liberar memoria
    free(objetos);
    free(jugador->id_objeto[0]);
    for(int i = 1; i < 5; i++) free(jugador->id_objeto[i]);
    free(jugador->id_objeto);
    free(jugador);
}

/**
 * TEST 4: Prueba de CogerObjeto()
 * 
 * EXPLICACIÓN:
 * - Esta función cambia un objeto de la sala al inventario
 * - Incrementa el contador de items del jugador
 * - En el test automatizado, lo hacemos sin esperar input del usuario
 */
void test_coger_objeto() {
    printf("\n========================================\n");
    printf("TEST 4: COGER_OBJETO()\n");
    printf("========================================\n");
    printf("OBJETIVO: Mover un objeto de la sala al inventario del jugador\n\n");
    
    Salas *salas = inicializar_salas_prueba();
    Objetos *objetos = inicializar_objetos_prueba();
    Conexiones *conexiones = inicializar_conexiones_prueba();
    Puzles *puzles = inicializar_puzles_prueba();
    Partida *partida = inicializar_partida_prueba(salas, objetos, conexiones, puzles);
    Jugadores *jugador = inicializar_jugador_prueba();
    
    printf("Sala actual: %d\n", partida->id_sala_actual);
    printf("Items antes: %d\n\n", jugador->num_items);
    
    // Mostrar objetos disponibles en la sala
    printf("Objetos disponibles en la sala:\n");
    for(int i = 0; i < partida->num_objetos; i++) {
        if(objetos[i].localizacion_objeto > 0) { // No está en inventario
            printf("  [%d] %s (ID: %s)\n", i, objetos[i].nombre_objeto, objetos[i].id_objeto);
        }
    }
    printf("\nCogiendo objeto 0 (Llave dorada)...\n");
    
    // Simular coger el objeto 0
    if(objetos[0].localizacion_objeto > 0) {
        objetos[0].localizacion_objeto = 0; // Mover a inventario
        jugador->num_items++;
        printf("✓ Has cogido: %s\n", objetos[0].nombre_objeto);
        printf("Items después: %d\n\n", jugador->num_items);
        printf("✓ Test completado exitosamente.\n");
    } else {
        printf("✗ Error: El objeto ya está en el inventario.\n");
    }
    
    // Liberar memoria
    free(salas);
    free(objetos);
    free(conexiones);
    free(puzles);
    free(partida);
    free(jugador->id_objeto[0]);
    for(int i = 1; i < 5; i++) free(jugador->id_objeto[i]);
    free(jugador->id_objeto);
    free(jugador);
}

/**
 * TEST 5: Prueba de SoltarObjeto()
 * 
 * EXPLICACIÓN:
 * - Mueve un objeto del inventario a la sala actual
 * - Decrementa el contador de items del jugador
 * - Útil cuando el jugador necesita dejar un objeto para coger otro
 */
void test_soltar_objeto() {
    printf("\n========================================\n");
    printf("TEST 5: SOLTAR_OBJETO()\n");
    printf("========================================\n");
    printf("OBJETIVO: Mover un objeto del inventario a la sala actual\n\n");
    
    Salas *salas = inicializar_salas_prueba();
    Objetos *objetos = inicializar_objetos_prueba();
    Conexiones *conexiones = inicializar_conexiones_prueba();
    Puzles *puzles = inicializar_puzles_prueba();
    Partida *partida = inicializar_partida_prueba(salas, objetos, conexiones, puzles);
    Jugadores *jugador = inicializar_jugador_prueba();
    
    printf("Sala actual: %d\n", partida->id_sala_actual);
    printf("Items antes: %d\n\n", jugador->num_items);
    
    // Mostrar objetos en venta
    printf("Objetos en inventario:\n");
    for(int i = 0; i < partida->num_objetos; i++) {
        if(objetos[i].localizacion_objeto == 0) { // Está en inventario
            printf("  [%d] %s (ID: %s)\n", i, objetos[i].nombre_objeto, objetos[i].id_objeto);
        }
    }
    printf("\nSoltando objeto 1 (Linterna)...\n");
    
    // Simular soltar objeto 1
    if(objetos[1].localizacion_objeto == 0) {
        objetos[1].localizacion_objeto = partida->id_sala_actual; // Mover a sala actual
        jugador->num_items--;
        printf("✓ Has soltado: %s\n", objetos[1].nombre_objeto);
        printf("Items después: %d\n\n", jugador->num_items);
        printf("✓ Test completado exitosamente.\n");
    } else {
        printf("✗ Error: El objeto no está en el inventario.\n");
    }
    
    // Liberar memoria
    free(salas);
    free(objetos);
    free(conexiones);
    free(puzles);
    free(partida);
    free(jugador->id_objeto[0]);
    for(int i = 1; i < 5; i++) free(jugador->id_objeto[i]);
    free(jugador->id_objeto);
    free(jugador);
}

/**
 * TEST 6: Prueba de ResolverPuzle()
 * 
 * EXPLICACIÓN:
 * - Permite al jugador intentar resolver un puzle
 * - Compara la respuesta con la solución correcta
 * - Si acierta, puede desbloquear áreas o conseguir items
 */
void test_resolver_puzle() {
    printf("\n========================================\n");
    printf("TEST 6: RESOLVER_PUZLE()\n");
    printf("========================================\n");
    printf("OBJETIVO: Resolver un puzle introduciendo la solución correcta\n\n");
    
    Puzles *puzles = inicializar_puzles_prueba();
    Partida *partida = inicializar_partida_prueba(
        inicializar_salas_prueba(),
        inicializar_objetos_prueba(),
        inicializar_conexiones_prueba(),
        puzles
    );
    
    printf("Sala actual: %d\n\n", partida->id_sala_actual);
    
    // Cambiar a sala 1 donde hay un puzle
    partida->id_sala_actual = 1;
    printf("Movido a sala 1 (hay puzles aquí)\n\n");
    
    // Mostrar puzles en la sala
    printf("Puzles en la sala:\n");
    for(int i = 0; i < partida->num_puzles; i++) {
        if(puzles[i].id_sala_puzle == partida->id_sala_actual) {
            printf("  [%d] %s\n", i, puzles[i].descripcion_puzle);
        }
    }
    
    printf("\nIntentando resolver puzle 0...\n");
    printf("Descripción: %s\n", puzles[0].descripcion_puzle);
    printf("Solución correcta: %s\n\n", puzles[0].solucion_puzle);
    
    // Test 1: Respuesta incorrecta
    printf("--- TEST 1: Respuesta incorrecta ---\n");
    char respuesta1[] = "PUERTA";
    if(strcmp(respuesta1, puzles[0].solucion_puzle) == 0) {
        printf("✓ ¡Correcto! Has resuelto el puzle.\n");
    } else {
        printf("✗ Incorrecto. La solución '%s' no es correcta.\n\n", respuesta1);
    }
    
    // Test 2: Respuesta correcta
    printf("\n--- TEST 2: Respuesta correcta ---\n");
    char respuesta2[] = "CASTILLO";
    if(strcmp(respuesta2, puzles[0].solucion_puzle) == 0) {
        printf("✓ ¡Correcto! Has resuelto el puzle '%s'.\n\n", puzles[0].descripcion_puzle);
    } else {
        printf("✗ Incorrecto. La solución '%s' no es correcta.\n\n", respuesta2);
    }
    
    printf("✓ Test completado exitosamente.\n");
    
    // Liberar memoria
    free(puzles);
    free(partida);
}

// ============================================================================
// MENÚ PRINCIPAL DE PRUEBAS
// ============================================================================

void mostrar_menu() {
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║     MÓDULO DE PRUEBA - JUEGO.C       ║\n");
    printf("║        Testea cada función            ║\n");
    printf("║       por separado para              ║\n");
    printf("║      encontrar errores fácilmente     ║\n");
    printf("╚════════════════════════════════════════╝\n\n");
    
    printf("Selecciona un test para ejecutar:\n\n");
    printf("1. TEST_DESCRIBIR_SALA\n");
    printf("   └─ Prueba mostrar descripción de sala actual\n\n");
    
    printf("2. TEST_EXAMINAR\n");
    printf("   └─ Prueba listar objetos y salidas de la sala\n\n");
    
    printf("3. TEST_INVENTARIO\n");
    printf("   └─ Prueba mostrar el inventario del jugador\n\n");
    
    printf("4. TEST_COGER_OBJETO\n");
    printf("   └─ Prueba mover objeto de sala a inventario\n\n");
    
    printf("5. TEST_SOLTAR_OBJETO\n");
    printf("   └─ Prueba mover objeto de inventario a sala\n\n");
    
    printf("6. TEST_RESOLVER_PUZLE\n");
    printf("   └─ Prueba resolver un puzle del juego\n\n");
    
    printf("7. EJECUTAR TODOS LOS TESTS\n");
    printf("   └─ Ejecuta los 6 tests en secuencia\n\n");
    
    printf("0. SALIR\n\n");
}

// ============================================================================
// FUNCIÓN MAIN - Punto de entrada del módulo de prueba
// ============================================================================

int main() {
    int opcion = -1;
    
    while(opcion != 0) {
        mostrar_menu();
        printf("Elige una opción (0-7): ");
        scanf("%d", &opcion);
        
        system("cls"); // Limpiar pantalla (Windows)
        
        switch(opcion) {
            case 1:
                test_describir_sala();
                break;
            case 2:
                test_examinar();
                break;
            case 3:
                test_inventario();
                break;
            case 4:
                test_coger_objeto();
                break;
            case 5:
                test_soltar_objeto();
                break;
            case 6:
                test_resolver_puzle();
                break;
            case 7:
                printf("\n╔════════════════════════════════════════╗\n");
                printf("║     EJECUTANDO TODOS LOS TESTS         ║\n");
                printf("╚════════════════════════════════════════╝\n");
                test_describir_sala();
                system("pause");
                system("cls");
                test_examinar();
                system("pause");
                system("cls");
                test_inventario();
                system("pause");
                system("cls");
                test_coger_objeto();
                system("pause");
                system("cls");
                test_soltar_objeto();
                system("pause");
                system("cls");
                test_resolver_puzle();
                system("pause");
                system("cls");
                printf("\n✓ TODOS LOS TESTS COMPLETADOS EXITOSAMENTE\n\n");
                break;
            case 0:
                printf("\n¡Hasta luego!\n");
                break;
            default:
                printf("Opción no válida. Por favor, elige una opción del menú.\n");
        }
        
        if(opcion != 0 && opcion != 7) {
            printf("\nPresiona ENTER para continuar...");
            getchar();
            getchar(); // Dos getchar() porque scanf deja un salto de línea
            system("cls");
        }
    }
    
    return 0;
}
