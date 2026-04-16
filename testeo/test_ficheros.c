/*
 * test_ficheros.c  — Tests interactivos para ficheros.c (Actualizado)
 *
 * Compilar (desde la raiz del proyecto):
 *   gcc -Wall -Wextra -o test_ficheros test_ficheros.c ficheros.c -I.
 * Ejecutar:
 *   .\test_ficheros.exe
 */

#include "../estructuras.h"
#include "../ficheros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define PAUSA() system("pause")
#define LIMPIAR_PANTALLA() system("cls")
#else
#define PAUSA()                                                                \
  do {                                                                         \
    printf("Presione [Enter] para continuar...");                              \
    getchar();                                                                 \
  } while (0)
#define LIMPIAR_PANTALLA() system("clear")
#endif

/* ── Utilidades de test ─────────────────────────────────────────── */
static int tests_pasados = 0;
static int tests_fallados = 0;

static void reiniciar_contadores() {
  tests_pasados = 0;
  tests_fallados = 0;
}

#define ASSERT(cond, msg)                                                      \
  do {                                                                         \
    if (cond) {                                                                \
      printf("  [OK]   %s\n", msg);                                            \
      tests_pasados++;                                                         \
    } else {                                                                   \
      printf("  [FAIL] %s (linea %d)\n", msg, __LINE__);                       \
      tests_fallados++;                                                        \
    }                                                                          \
  } while (0)

#define INFO(msg) printf("  [INFO] %s\n", msg)

/* ================================================================== */
/*  TEST 1 – volcado                                                   */
/* ================================================================== */
static void test_volcado(void) {
  LIMPIAR_PANTALLA();
  printf("\n=== TEST 1: Probando la funcion 'volcado' ===\n");
  INFO("Esta funcion deberia cargar salas.txt, puzles.txt, conexiones.txt y "
       "objetos.txt en memoria externa.");
  INFO(
      "Iniciando llamada a volcado(&salas, &conexiones, &puzles, &objetos)...");

  Salas *salas = NULL;
  int num_s = 0;
  Conexiones *conexiones = NULL;
  int num_c = 0;
  Puzles *puzles = NULL;
  int num_p = 0;
  Objetos *objetos = NULL;
  int num_o = 0;
  Jugadores *jugadores = NULL;
  int num_j = 0;

  int ret = volcado(&salas, &num_s, &conexiones, &num_c, &puzles, &num_p, &objetos, &num_o, &jugadores, &num_j);

  printf("\n--- Resultados del volcado ---\n");
  ASSERT(ret == 1,
         "volcado() devuelve 1 (Exito al leer/abrir varios ficheros)");

  INFO("Comprobando que los punteros retornados no son NULL (memoria "
       "asignada)...");
  ASSERT(salas != NULL, "El array dinamico 'salas'      NO es NULL");
  ASSERT(conexiones != NULL, "El array dinamico 'conexiones' NO es NULL");
  ASSERT(puzles != NULL, "El array dinamico 'puzles'     NO es NULL");
  ASSERT(objetos != NULL, "El array dinamico 'objetos'    NO es NULL");

  if (salas) {
    INFO("Inspeccionando el contenido de la primera sala obtenida...");
    printf("         ID Sala: %d\n", salas[0].id_sala);
    printf("         Nombre: '%s'\n", salas[0].nombre_sala);
    ASSERT(salas[0].id_sala >= 1,
           "Primera sala tiene id_sala >= 1 (La sala de inicio es la 1)");
    ASSERT(salas[0].nombre_sala[0] != '\0',
           "Primera sala tiene nombre no vacio");
  }

  if (jugadores) {
    int max_jugadores = 0;
    FILE *f_jug = fopen("ficheros/jugadores.txt", "r");
    if (f_jug) {
      char line[512];
      while (fgets(line, sizeof(line), f_jug)) {
        if (line[0] != '/' && line[0] != '\0' && line[0] != '\n') {
          max_jugadores++;
        }
      }
      fclose(f_jug);
    }

    printf("\n  [INFO] Inspeccionando los jugadores volcados en memoria...\n");
    char buf[64];
    while (1) {
      printf("\nIntroduce el ID de un jugador para ver su informacion (0 para terminar la visualizacion): ");
      if (!fgets(buf, sizeof(buf), stdin)) break;
      buf[strcspn(buf, "\n")] = 0;
      int search_id = atoi(buf);
      if (search_id <= 0) break;

      int encontrado = 0;
      for (int i = 0; i < max_jugadores; i++) {
        if (jugadores[i].id_jugador == search_id) {
          printf("  -> DATOS DEL JUGADOR ENCONTRADO:\n");
          printf("       ID: %d\n", jugadores[i].id_jugador);
          printf("       Nombre real: %s\n", jugadores[i].nombre_jugador);
          printf("       Nickname: %s\n", jugadores[i].jugador);
          printf("       Num. objetos: %d\n", jugadores[i].num_items);
          for (int j = 0; j < jugadores[i].num_items; j++) {
            printf("         - ID Objeto %d: %s\n", j + 1, jugadores[i].id_objeto[j]);
          }
          encontrado = 1;
          break;
        }
      }
      if (!encontrado) {
        printf("  [!] Jugador con ID %d no encontrado.\n", search_id);
      }
    }

    /* Liberación profunda de jugadores para evitar memory leaks */
    for (int i = 0; i < max_jugadores; i++) {
      if (jugadores[i].id_objeto) {
        for (int j = 0; j < jugadores[i].num_items; j++) {
          if (jugadores[i].id_objeto[j]) {
            free(jugadores[i].id_objeto[j]);
          }
        }
        free(jugadores[i].id_objeto);
      }
    }
  }

  INFO("Liberando memoria usada en este test...");
  free(salas);
  free(conexiones);
  free(puzles);
  free(objetos);
  free(jugadores);

  printf("\nFIN DEL TEST DE VOLCADO.\n");
  PAUSA();
}

/* ================================================================== */
/*  TEST 2 – cargarPartida                                             */
/* ================================================================== */
static void test_cargarPartida(void) {
  LIMPIAR_PANTALLA();
  printf("\n=== TEST 2: Probando la funcion 'cargarPartida' ===\n");

  /* ---- Caso 1: jugador que NO existe ---- */
  INFO("--- Caso 2.1: Intentar cargar un jugador INEXISTENTE ---");
  {
    Partida par;
    memset(&par, 0, sizeof(Partida));
    INFO("Llamando a cargarPartida con nickname='jugador_falso_123'...");
    int ret = cargarPartida(&par, "jugador_falso_123");
    ASSERT(ret == 0,
           "cargarPartida() devuelve 0 al no encontrar el jugador_falso_123");
    free(par.lista_objetos);
    free(par.lista_conexiones);
    free(par.lista_puzles);
  }
  printf("\n");

  /* ---- Caso 2: obtener el primer nickname real del fichero ---- */
  INFO("--- Caso 2.2: Intentar cargar un jugador REAL de jugadores.txt ---");
  INFO("Buscando un jugador valido en ficheros/jugadores.txt para probar...");
  {
    FILE *f = fopen("ficheros/jugadores.txt", "r");
    char primer_nick[16] = {0};
    if (f) {
      char line[512];
      while (fgets(line, sizeof(line), f)) {
        if (line[0] == '/' || line[0] == '\0' || line[0] == '\n')
          continue;
        char tmp[512];
        strncpy(tmp, line, sizeof(tmp) - 1);
        strtok(tmp, "-");
        strtok(NULL, "-");
        char *nick = strtok(NULL, "-");
        if (nick) {
          strncpy(primer_nick, nick, sizeof(primer_nick) - 1);
          break;
        }
      }
      fclose(f);
    }

    if (primer_nick[0] != '\0') {
      printf("  [INFO] Se encontro al jugador: '%s'\n", primer_nick);
      Partida par;
      memset(&par, 0, sizeof(Partida));

      printf("  [INFO] Llamando a cargarPartida(&par, \"%s\")...\n",
             primer_nick);
      int ret = cargarPartida(&par, primer_nick);

      ASSERT(ret == 1,
             "cargarPartida() devuelve 1 (Exito al cargar al jugador)");
      printf("         ID Jugador leido: %d\n", par.id_jugador);
      ASSERT(par.id_jugador > 0,
             "El 'id_jugador' cargado en struct Partida es > 0");

      printf("         Sala Actual asignada: %d\n", par.id_sala_actual);
      ASSERT(par.id_sala_actual >= 1,
             "La 'id_sala_actual' cargada empieza en 1 (0 es de inventario)");

      if (par.num_objetos > 0) {
        printf("         Objetos cargados en la partida: %d\n",
               par.num_objetos);
        ASSERT(par.lista_objetos != NULL,
               "lista_objetos fue inicializada correctamente por malloc");
      }
      if (par.num_conexiones > 0) {
        printf("         Conexiones cargadas en la partida: %d\n",
               par.num_conexiones);
        ASSERT(par.lista_conexiones != NULL,
               "lista_conexiones fue inicializada correctamente por malloc");
      }
      if (par.num_puzles > 0) {
        printf("         Puzles cargados en la partida: %d\n", par.num_puzles);
        ASSERT(par.lista_puzles != NULL,
               "lista_puzles fue inicializada correctamente por malloc");
      }

      INFO("Liberando memoria de objetos/conexiones/puzles del test de "
           "carga...");
      free(par.lista_objetos);
      free(par.lista_conexiones);
      free(par.lista_puzles);
    } else {
      printf("  [SKIP] No se encontro un jugador real valido en jugadores.txt. "
             "Test ignorado.\n");
    }
  }

  printf("\nFIN DEL TEST DE CARGAR_PARTIDA.\n");
  PAUSA();
}

/* ================================================================== */
/*  TEST 3 – guardarPartida (round-trip con cargarPartida)            */
/* ================================================================== */
static void test_guardarPartida(void) {
  LIMPIAR_PANTALLA();
  printf("\n=== TEST 3: Probando la funcion 'guardarPartida' ===\n");
  INFO("Este test cargara un jugador valido, modificara un dato de prueba (la "
       "sala actual),");
  INFO("guardara la partida en los archivos, volvera a cargarla para verificar "
       "si se guardo,");
  INFO("y por ultimo restaurara la partida a como estaba originalmente.");
  printf("\n");

  INFO("Buscando un jugador valido...");
  FILE *f = fopen("ficheros/jugadores.txt", "r");
  char primer_nick[16] = {0};
  int primer_id = -1;
  if (f) {
    char line[512];
    while (fgets(line, sizeof(line), f)) {
      if (line[0] == '/' || line[0] == '\0' || line[0] == '\n')
        continue;
      char tmp[512];
      strncpy(tmp, line, sizeof(tmp) - 1);
      char *id_s = strtok(tmp, "-");
      strtok(NULL, "-");
      char *nick = strtok(NULL, "-");
      if (id_s && nick) {
        primer_id = atoi(id_s);
        strncpy(primer_nick, nick, sizeof(primer_nick) - 1);
        break;
      }
    }
    fclose(f);
  }

  if (primer_nick[0] == '\0' || primer_id < 0) {
    printf("  [SKIP] No se encontro jugador valido para el test de guardar. "
           "Asegurate de que hay jugadores en jugadores.txt.\n");
    PAUSA();
    return;
  }

  printf("  [INFO] Usando al jugador: '%s' (ID %d)\n", primer_nick, primer_id);

  INFO("Paso A: CARGANDO el estado actual del jugador...");
  Partida par_original;
  memset(&par_original, 0, sizeof(Partida));
  cargarPartida(&par_original, primer_nick);

  int sala_antigua = par_original.id_sala_actual;
  // Si acaba de ser inicializado por primera vez y no habia partida valida,
  // seria 1.
  int sala_nueva = sala_antigua + 1;
  printf("         Sala original del jugador: %d. ¡Procedemos a cambiar la "
         "sala virtualmente a %d!\n",
         sala_antigua, sala_nueva);

  INFO("Paso B: MODIFICANDO y GUARDANDO la partida en "
       "ficheros/partida_tmp.txt, que sobreescribira al original...");
  par_original.id_sala_actual = sala_nueva;

  // NOTA PARA LA MODIFICACIÓN: la firma de guardarPartida() se ha
  // simplificado para no requerir el parámetro Jugadores *jug ya que no
  // se utiliza en la implementación de la función.
  guardarPartida(&par_original);

  INFO("Paso C: RE-CARGANDO la partida y verificando cambios...");
  Partida par_recargado;
  memset(&par_recargado, 0, sizeof(Partida));
  int ret = cargarPartida(&par_recargado, primer_nick);

  ASSERT(ret == 1,
         "cargarPartida() recupera la partida modificada exitosamente.");
  printf("         Sala recargada: %d vs Sala que deberia ser: %d\n",
         par_recargado.id_sala_actual, sala_nueva);
  ASSERT(par_recargado.id_sala_actual == sala_nueva,
         "Verificamos que id_sala_actual del archivo guardado coincide con el "
         "valor alterado.");

  INFO("Paso D: RESTAURANDO la sala original para no romper la partida del "
       "jugador...");
  par_recargado.id_sala_actual = sala_antigua;
  guardarPartida(&par_recargado);
  INFO("Restauracion completa. Todo deberia volver a la normalidad.");

  INFO("Liberando memoria del round-trip...");
  free(par_original.lista_objetos);
  free(par_original.lista_conexiones);
  free(par_original.lista_puzles);
  free(par_recargado.lista_objetos);
  free(par_recargado.lista_conexiones);
  free(par_recargado.lista_puzles);

  printf("\nFIN DEL TEST DE GUARDAR_PARTIDA.\n");
  PAUSA();
}

/* ================================================================== */
/*  MAIN INTERACTIVO                                                   */
/* ================================================================== */
int main(void) {
  int opcion = -1;
  char input[64];

  while (opcion != 0) {
    LIMPIAR_PANTALLA();
    printf("=========================================\n");
    printf("    MENÚ DE PRUEBAS PARA FICHEROS.C      \n");
    printf("=========================================\n");
    printf("1. Testear carga de tablas base (volcado())\n");
    printf("2. Testear carga de jugador (cargarPartida())\n");
    printf("3. Testear guardar cuenta de jugador (guardarPartida())\n");
    printf("4. Correr TODOS los tests de golpe (resumen de codigo)\n");
    printf("0. Salir\n");
    printf("=========================================\n");
    printf("\nElige una opcion: ");

    if (fgets(input, sizeof(input), stdin)) {
      /* Limpieza del salto de linea */
      input[strcspn(input, "\n")] = 0;
      opcion = atoi(input);

      switch (opcion) {
      case 1:
        reiniciar_contadores();
        test_volcado();
        break;
      case 2:
        reiniciar_contadores();
        test_cargarPartida();
        break;
      case 3:
        reiniciar_contadores();
        test_guardarPartida();
        break;
      case 4:
        reiniciar_contadores();
        test_volcado();
        test_cargarPartida();
        test_guardarPartida();
        LIMPIAR_PANTALLA();
        printf("=========================================\n");
        printf("  RESULTADO GLOBAL: %d PASADOS  |  %d FALLADOS\n",
               tests_pasados, tests_fallados);
        printf("=========================================\n");
        PAUSA();
        break;
      case 0:
        printf("\nSaliendo del probador... ¡Hasta luego!\n");
        break;
      default:
        printf("\n[ERROR] Opcion no valida. Pulsa ENTER para continuar.\n");
        PAUSA();
        break;
      }
    }
  }

  return 0;
}
