#include "ficheros.h"
// Módulo de ficheros
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CPY(d, s) (strncpy((d), (s), sizeof(d) - 1), (d)[sizeof(d) - 1] = '\0')

//Precondicion: Recibe dos cadenas de caracteres, una que representa el nombre de un fichero original y otra que representa el nombre de un fichero temporal
//Postcondicion: Elimina el fichero original y renombra el fichero temporal con el nombre del fichero original
static void reemplazar(const char *orig, const char *tmp) {
  remove(orig);
  rename(tmp, orig);
}

//Precondicion: recibe un puntero a un array de Salas (inicialmente NULL)
//Postcondicion: lee el fichero de salas, asigna dinamicamente el array y lo llena con los datos. Devuelve el numero de salas leidas o -1 si hubo error al abrir el fichero. El formato del fichero debe ser: ID-NOMBRE-TIPO-DESCRIPCION (ej: 01-Sala Inicial-INICIAL-Descripcion de la sala)
static int leer_salas(Salas **salas) {
  FILE *f = fopen("salas.txt", "r"); //Abre el fichero de salas
  if (!f) return -1; // Si no se pudo abrir, devuelve -1

  *salas = NULL;
  char line[512]; // Buffer para leer lineas
  int n = 0;

  while (fgets(line, sizeof(line), f)) {
    line[strcspn(line, "\r\n")] = '\0'; // Elimina saltos de linea
    if (line[0] == '/' || line[0] == '\0') continue; // Ignora lineas vacias o comentarios

    Salas *tmp = realloc(*salas, (n + 1) * sizeof(Salas));
    if (!tmp) {
      free(*salas); // Si no se pudo asignar memoria, libera el array actual y devuelve -1
      *salas = NULL;
      fclose(f);
      return -1;
    }
    *salas = tmp; // Asigna el nuevo puntero al array de salas

    char *id_sala_str = strtok(line, "-"), *nombre_sala_str = strtok(NULL, "-"), *tipo_sala_str = strtok(NULL, "-"), *descripcion_sala_str = strtok(NULL, "-"); // Separa campos por '-'

    if (id_sala_str && nombre_sala_str && tipo_sala_str && descripcion_sala_str) {
      (*salas)[n].id_sala = atoi(id_sala_str); // Convierte el ID de sala a entero
      CPY((*salas)[n].nombre_sala, nombre_sala_str); // Copia el nombre de la sala
      (*salas)[n].tipo_sala = (strcmp(tipo_sala_str, "INICIAL") == 0) ? 1 : (strcmp(tipo_sala_str, "NORMAL") == 0) ? 2 : 3; // Determina el tipo de sala (1: INICIAL, 2: NORMAL, 3: SALIDA)
      CPY((*salas)[n].descripcion_sala, descripcion_sala_str); // Copia la descripcion de la sala
      n++;
    }
  }

  fclose(f); // Cierra el fichero de salas
  return n; // Devuelve el número de salas leídas exitosamente
}

//Precondicion: recibe un puntero a un array de Puzles (inicialmente NULL)
//Postcondicion: lee el fichero de puzles, asigna dinamicamente el array y lo llena con los datos. Devuelve el numero de puzles leidos o -1 si hubo error al abrir el fichero. El formato del fichero debe ser: ID-SALA-TIPO-DESCRIPCION-SOLUCION (ej: P01-01-CODIGO-Descripcion del puzle-1234)
static int leer_puzles(Puzles **puzles) {
  FILE *f = fopen("puzles.txt", "r"); //Abre el fichero de puzles
  if (!f) return -1; // Si no se pudo abrir, devuelve -1

  *puzles = NULL;
  char line[512]; // Buffer para leer lineas
  int n = 0;

  while (fgets(line, sizeof(line), f)) {
    line[strcspn(line, "\r\n")] = '\0'; // Elimina saltos de linea
    if (line[0] == '/' || line[0] == '\0') continue; // Ignora lineas vacias o comentarios

    Puzles *tmp = realloc(*puzles, (n + 1) * sizeof(Puzles)); // Redimensiona el array de puzles para agregar un nuevo puzle
    if (!tmp) {
      free(*puzles); // Si no se pudo asignar memoria, libera el array actual y devuelve -1
      *puzles = NULL;
      fclose(f);
      return -1;
    }
    *puzles = tmp; // Asigna el nuevo puntero al array de puzles
    memset(&(*puzles)[n], 0, sizeof(Puzles)); // Limpia la nueva entrada de puzle

    char *id_puzle_str = strtok(line, "-"); // Separa campos por '-'
    strtok(NULL, "-"); // SALTAR el nombre del puzle (campo 2)
    char *id_sala_puzle_str = strtok(NULL, "-"); // El ID de sala es el campo 3
    char *descripcion_puzle_str = strtok(NULL, "-");
    char *solucion_puzle_str = strtok(NULL, "-");

    // Verifica que se hayan obtenido todos los campos necesarios para crear el puzle
    if (id_puzle_str && id_sala_puzle_str && descripcion_puzle_str && solucion_puzle_str) {
      CPY((*puzles)[n].id_puzle, id_puzle_str); // Copia el ID del puzle
      (*puzles)[n].id_sala_puzle = atoi(id_sala_puzle_str); // Convierte el ID de sala a entero
      // Lógica de tipo: 1 si es numérico, 2 si tiene letras
      (*puzles)[n].tipo_elemento = (strspn(solucion_puzle_str, "0123456789") == strlen(solucion_puzle_str)) ? 1 : 2;
      CPY((*puzles)[n].descripcion_puzle, descripcion_puzle_str); // Copia la descripcion del puzle
      CPY((*puzles)[n].solucion_puzle, solucion_puzle_str); // Copia la solucion del puzle
      (*puzles)[n].resuelto = 0; // Inicialmente, el puzle no está resuelto
      n++;
    }
  }
  fclose(f); // Cierra el fichero de puzles
  return n; // Devuelve el número de puzles leídos exitosamente
}

//Precondicion: recibe un puntero a un array de Conexiones (inicialmente NULL)
//Postcondicion: lee el fichero de conexiones, asigna dinamicamente el array y lo llena con los datos. Devuelve el numero de conexiones leidas o -1 si hubo error al abrir el fichero. El formato del fichero debe ser: ID-SALA_ORIGEN-SALA_DESTINO-ESTADO-CONDICIONANTE (ej: C01-01-02-Activa-0)
static int leer_conexiones(Conexiones **conexiones) {
  FILE *f = fopen("conexiones.txt", "r"); //Abre el fichero de conexiones
  if (!f) return -1; // Si no se pudo abrir, devuelve -1

  *conexiones = NULL;
  char line[512]; // Buffer para leer lineas
  int n = 0;

  while (fgets(line, sizeof(line), f)) {
    line[strcspn(line, "\r\n")] = '\0'; // Elimina saltos de linea
    if (line[0] == '/' || line[0] == '\0') continue; // Ignora lineas vacias o comentarios

    Conexiones *tmp = realloc(*conexiones, (n + 1) * sizeof(Conexiones)); // Redimensiona el array de conexiones para agregar una nueva conexión
    if (!tmp) {
      free(*conexiones); // Si no se pudo asignar memoria, libera el array actual y devuelve -1
      *conexiones = NULL;
      fclose(f);
      return -1;
    }
    *conexiones = tmp; // Asigna el nuevo puntero al array de conexiones

    char *id_con = strtok(line, "-"), *orig = strtok(NULL, "-"), *dest = strtok(NULL, "-"), *est = strtok(NULL, "-"), *cond = strtok(NULL, "-"); // Separa campos por '-'

    if (id_con && orig && dest && est && cond) {
      CPY((*conexiones)[n].id_conexion, id_con); // Copia el ID de la conexión
      (*conexiones)[n].id_sala_orig = atoi(orig); // Convierte el ID de sala origen a entero
      (*conexiones)[n].id_sala_dest = atoi(dest); // Convierte el ID de sala destino a entero
      (*conexiones)[n].estado_conexion = (strcmp(est, "Activa") == 0) ? 1 : 0; // Determina el estado de la conexión (1: Activa, 0: Bloqueada)
      CPY((*conexiones)[n].id_condicionante, cond); // Copia el ID del condicionante (objeto o puzle que condiciona la conexión, o "0" si no tiene)
      n++;
    }
  }
  fclose(f); // Cierra el fichero de conexiones
  return n; // Devuelve el número de conexiones leídas exitosamente
}

//Precondicion: recibe un puntero a un array de Objetos (inicialmente NULL)
//Postcondicion: lee el fichero de objetos, asigna dinamicamente el array y lo llena con los datos. Devuelve el numero de objetos leidos o -1 si hubo error al abrir el fichero. El formato del fichero debe ser: ID-NOMBRE-DESCRIPCION-UBICACION (ej: OB01-Objeto Uno-Descripcion del objeto-01)
static int leer_objetos(Objetos **objetos) {
  FILE *f = fopen("objetos.txt", "r"); //Abre el fichero de objetos
  if (!f) return -1; // Si no se pudo abrir, devuelve -1

  *objetos = NULL; // Inicializa el puntero a NULL para luego asignar memoria dinámicamente
  char line[512];
  int n = 0;

  while (fgets(line, sizeof(line), f)) {
    line[strcspn(line, "\r\n")] = '\0'; // Elimina saltos de linea
    if (line[0] == '/' || line[0] == '\0') continue; // Ignora lineas vacias o comentarios

    Objetos *tmp = realloc(*objetos, (n + 1) * sizeof(Objetos)); // Redimensiona el array de objetos para agregar un nuevo objeto
    if (!tmp) {
      free(*objetos); // Si no se pudo asignar memoria, libera el array actual y devuelve -1
      *objetos = NULL;
      fclose(f);
      return -1;
    }
    *objetos = tmp; // Asigna el nuevo puntero al array de objetos
    memset(&(*objetos)[n], 0, sizeof(Objetos));

    char *id = strtok(line, "-"), *nom = strtok(NULL, "-"), *desc = strtok(NULL, "-"), *loc = strtok(NULL, "-"); // Separa campos por '-'

    if (id && nom && desc && loc) { 
      CPY((*objetos)[n].id_objeto, id); // Copia el ID del objeto
      CPY((*objetos)[n].nombre_objeto, nom); // Copia el nombre del objeto
      CPY((*objetos)[n].descripcion_objeto, desc); // Copia la descripcion del objeto
      (*objetos)[n].localizacion_objeto = atoi(loc); // Convierte la ubicación a entero (0 = inventario, >0 = sala)
      n++;
    }
  }
  fclose(f); // Cierra el fichero de objetos
  return n; // Devuelve el número de objetos leídos exitosamente
}

//Precondicion: recibe un puntero a un array de Jugadores (inicialmente NULL)
//Postcondicion: lee el fichero de jugadores, asigna dinamicamente el array y lo llena con los datos. Devuelve el numero de jugadores leidos o -1 si hubo error al abrir el fichero. El formato del fichero debe ser: ID-NOMBRE-NICKNAME-CONTRASENA-OBJETOS (ej: J01-Jugador Uno-jug1-pass123-OB01,OB02)
static int leer_jugadores(Jugadores **jugadores) {
  FILE *f = fopen("jugadores.txt", "r"); //Abre el fichero de jugadores
  if (!f) return -1; // Si no se pudo abrir, devuelve -1

  *jugadores = NULL; // Inicializa el puntero a NULL para luego asignar memoria dinámicamente
  char line[512];
  int n = 0;

  while (fgets(line, sizeof(line), f)) {
    line[strcspn(line, "\r\n")] = '\0'; // Elimina saltos de linea
    if (line[0] == '/' || line[0] == '\0') continue; // Ignora lineas vacias o comentarios

    Jugadores *tmp_jug = realloc(*jugadores, (n + 1) * sizeof(Jugadores)); // Redimensiona el array de jugadores para agregar un nuevo jugador
    if (!tmp_jug) {
      free(*jugadores); // Si no se pudo asignar memoria, libera el array actual y devuelve -1
      *jugadores = NULL;
      fclose(f);
      return -1;
    }
    *jugadores = tmp_jug; // Asigna el nuevo puntero al array de jugadores
    memset(&(*jugadores)[n], 0, sizeof(Jugadores)); // Inicializa punteros a NULL y contadores a 0

    char *id = strtok(line, "-"), *nom = strtok(NULL, "-"), *nick = strtok(NULL, "-"), *pw = strtok(NULL, "-"); // Separa campos por '-'. El inventario se carga en cargarPartida, no aqui

    if (id && nom && nick && pw) {
      (*jugadores)[n].id_jugador = atoi(id); // Convierte el ID del jugador a entero
      CPY((*jugadores)[n].nombre_jugador, nom); // Copia el nombre del jugador
      CPY((*jugadores)[n].jugador, nick); // Copia el nickname del jugador
      CPY((*jugadores)[n].contrasena, pw); // Copia la contraseña del jugador
      // El inventario se carga en cargarPartida, no aqui
      n++;
    }
  }
  fclose(f); // Cierra el fichero de jugadores
  return n; // Devuelve el número de jugadores leídos exitosamente
}

//Precondicion: recibe punteros a arrays de Salas, Conexiones, Puzles, Objetos y Jugadores (inicialmente NULL) y punteros a enteros para almacenar el numero de cada entidad
//Postcondicion: lee todos los ficheros y llena las estructuras en memoria. Devuelve 1 si todas las lecturas fueron exitosas o 0 si alguna lectura fallo
int volcado(Salas **s, int *num_s, Conexiones **c, int *num_c, Puzles **p, int *num_p, Objetos **o, int *num_o, Jugadores **j, int *num_j) {

  *num_s = leer_salas(s);
  *num_p = leer_puzles(p);
  *num_c = leer_conexiones(c);
  *num_o = leer_objetos(o);
  *num_j = leer_jugadores(j);

  return (*num_s >= 0 && *num_p >= 0 && *num_c >= 0 && *num_o >= 0 && *num_j >= 0) ? 1 : 0; // Verifica si todas las lecturas fueron exitosas
}

//Precondicion: recibe un puntero a un jugador, el ID de la sala actual y los arrays de objetos, conexiones y puzles con su respectivo numero de elementos
//Postcondicion: carga el progreso del jugador desde partida.txt, actualizando la sala actual, la ubicacion de los objetos, el estado de las conexiones y el estado de los puzles. Devuelve 1 si se cargo correctamente o 0 si hubo error al abrir partida.txt (en cuyo caso se mantiene el estado base limpio)
int cargarPartida(Jugadores **jugadores, int indice_jugador, int *id_sala_actual, Inventario *inv, Objetos **lista_objetos, int num_objetos, Conexiones **lista_conexiones, int num_conexiones, Puzles **lista_puzles, int num_puzles){
  Jugadores *jug = &((*jugadores)[indice_jugador]);
  FILE *f;
  char line[512];

  // 1. Cargar el inventario del jugador desde jugadores.txt y aplicarlo a lista_objetos
  // Liberamos el inventario previo si lo hubiera
  if (inv->Inventario) {
    free(inv->Inventario); // Libera la memoria del inventario previo
    inv->Inventario = NULL; // Inicializa el puntero a NULL
  }
  inv->num_objetos = 0; // Inicializa el contador de objetos

  FILE *archivo_jugadores = fopen("jugadores.txt", "r"); // Abre el fichero de jugadores
  if (archivo_jugadores) {
    char linea_jugador[512]; // Lee cada linea del fichero
    while (fgets(linea_jugador, sizeof(linea_jugador), archivo_jugadores)) {
      linea_jugador[strcspn(linea_jugador, "\r\n")] = '\0'; // Elimina saltos de linea
      if (linea_jugador[0] == '/' || linea_jugador[0] == '\0') continue; // Salta comentarios y lineas vacias
      char copia_linea[512];
      strncpy(copia_linea, linea_jugador, sizeof(copia_linea) - 1); // Copia la linea a un buffer temporal
      copia_linea[sizeof(copia_linea) - 1] = '\0'; // Asegura que el buffer este terminado en nulo
      char *id_leido  = strtok(copia_linea, "-"); // Separa el ID del jugador
      strtok(NULL, "-"); // nombre
      strtok(NULL, "-"); // nick
      strtok(NULL, "-"); // password
      char *inv_leido = strtok(NULL, ""); // inventario (puede ser NULL si no tiene objetos)
      if (id_leido && atoi(id_leido) == jug->id_jugador) {
        if (inv_leido) {
          char *id_obj_inv = strtok(inv_leido, ","); // Divide el inventario en objetos
          while (id_obj_inv) {
            Objetos *nuevo_array = realloc(inv->Inventario, (inv->num_objetos + 1) * sizeof(Objetos)); // Reasigna memoria para el inventario
            if (nuevo_array) {
              inv->Inventario = nuevo_array; // Asigna el nuevo puntero al array de objetos
              memset(&inv->Inventario[inv->num_objetos], 0, sizeof(Objetos));
              // Buscamos el objeto en la lista global para copiar sus datos (Nombre, Desc)
              int encontrado = 0;
              for (int i = 0; i < num_objetos && !encontrado; i++) {
                if (strcmp((*lista_objetos)[i].id_objeto, id_obj_inv) == 0) {
                  inv->Inventario[inv->num_objetos] = (*lista_objetos)[i]; // Copiamos el struct completo
                  inv->Inventario[inv->num_objetos].localizacion_objeto = 0; // Por si acaso
                  (*lista_objetos)[i].localizacion_objeto = 0; // Marcar como en inventario en la lista global
                  encontrado = 1;
                }
              }
              if (!encontrado) {
                  CPY(inv->Inventario[inv->num_objetos].id_objeto, id_obj_inv); // Fallback si no está en la base (no debería pasar)
              }
              inv->num_objetos++;
            }
            id_obj_inv = strtok(NULL, ",");
          }
        }
        break; // jugador encontrado, no seguir leyendo
      }
    }
    fclose(archivo_jugadores);
  }

  // 2. Sobreescribir el resto del mundo con el progreso guardado en partida.txt:
  if ((f = fopen("partida.txt", "r")) == NULL)
    return 1; // Si no hay archivo, devolvemos 1 (comienza limpio)

  int bloque_encontrado = 0;
  while (fgets(line, sizeof(line), f)) {
    line[strcspn(line, "\r\n")] = '\0'; // Elimina saltos de linea
    if (line[0] == '/' || line[0] == '\0')
      continue;

    // Creamos una copia de la linea para tokenizarla y obtener la clave y el valor
    char linea_partida[512];
    strncpy(linea_partida, line, sizeof(linea_partida) - 1); // Copia la línea a un buffer temporal para tokenizar
    linea_partida[sizeof(linea_partida) - 1] = '\0'; // Asegura la terminación nula del string

    char *tipo_dato = strtok(linea_partida, ":");
    char *valor_dato = strtok(NULL, "");
    if (!tipo_dato || !valor_dato)
      continue;

    // Eliminamos espacio inicial en el valor si lo hay
    if (valor_dato[0] == ' ')
      valor_dato++;

    if (strcmp(tipo_dato, "JUGADOR") == 0) {
      bloque_encontrado = (atoi(valor_dato) == jug->id_jugador); // Activa la lectura solo si es el bloque del jugador
      continue;
    }
    if (!bloque_encontrado) // Si no estamos en el bloque del jugador, ignoramos el resto de lineas
      continue;

    if (strcmp(tipo_dato, "SALA") == 0) {
      *id_sala_actual = atoi(valor_dato); // Actualiza la sala actual del jugador
      continue;
    }
    if (strcmp(tipo_dato, "OBJETO") == 0) {
      // Sobreescribe la ubicacion del objeto en las estructuras globales
      char *id_objeto_str = strtok(valor_dato, "-"), *loc_str = strtok(NULL, "-");
      if (id_objeto_str && loc_str) {
        int loc = atoi(loc_str); // Convierte la ubicación a entero
        for (int i = 0; i < num_objetos; i++) {
          if (strcmp((*lista_objetos)[i].id_objeto, id_objeto_str) == 0) {
            (*lista_objetos)[i].localizacion_objeto = loc; // Actualiza la ubicación del objeto (0 = inventario, >0 = sala)
            break;
          }
        }
      }
      continue;
    }
    if (strcmp(tipo_dato, "CONEXION") == 0) {
      // Sobreescribe el estado de la conexion en las estructuras globales
      char *id_conexion_str = strtok(valor_dato, "-"), *estado_str = strtok(NULL, "-");
      if (id_conexion_str && estado_str) {
        for (int i = 0; i < num_conexiones; i++) {
          if (strcmp((*lista_conexiones)[i].id_conexion, id_conexion_str) == 0) {
            (*lista_conexiones)[i].estado_conexion = (strcmp(estado_str, "Activa") == 0) ? 1 : 0; // Actualiza el estado de la conexión (1 = activa, 0 = bloqueada)
            break;
          }
        }
      }
      continue;
    }
    if (strcmp(tipo_dato, "PUZZLE") == 0) {
      //Sobreescribe el estado del puzle en las estructuras globales
      char *id_puzle_str = strtok(valor_dato, "-"), *estado_str = strtok(NULL, "-");
      if (id_puzle_str && estado_str) {
        for (int i = 0; i < num_puzles; i++) {
          if (strcmp((*lista_puzles)[i].id_puzle, id_puzle_str) == 0) {
            (*lista_puzles)[i].resuelto = (strcmp(estado_str, "Resuelto") == 0) ? 1 : 0; // Actualiza el estado del puzle (1 = resuelto, 0 = pendiente)
            break;
          }
        }
      }
      continue;
    }
  }
  fclose(f);
  return 1;
}

//Precondicion: recibe un puntero a un jugador, el ID de la sala actual y los arrays de objetos, conexiones y puzles con su respectivo numero de elementos
//Postcondicion: guarda el progreso del jugador en partida.txt, actualizando solo las entradas correspondientes al jugador para mantener intacto el progreso de los demas jugadores. Devuelve 1 si se guardo correctamente o 0 si hubo error al abrir partida.txt (en cuyo caso no se modifica el archivo)
void guardarPartida(Jugadores **jugadores, int indice_jugador, int *id_sala_actual, Inventario *inv, Objetos *lista_objetos, Conexiones *lista_conexiones, Puzles *lista_puzles) {
  Jugadores *jug = &((*jugadores)[indice_jugador]);
  FILE *fin, *fout;
  char line[512];

  // Cargamos en memoria los listados limpios base directamente para extraer cuentas puras
  Objetos *objetos_archivo = NULL;
  Conexiones *conexiones_archivo = NULL;
  Puzles *puzles_archivo = NULL;

  int num_objetos = leer_objetos(&objetos_archivo); // Carga el listado base de objetos para comparar cambios de ubicación
  int num_conexiones = leer_conexiones(&conexiones_archivo); // Carga el listado base de conexiones para comparar cambios de estado
  int num_puzles = leer_puzles(&puzles_archivo); // Carga el listado base de puzles para comparar cambios de estado

  /* 1. Actualizacion de partida.txt:
   *    En lugar de modificar en linea, se copia todo el contenido original
   * excepto las entradas del jugador en cuestion, que se reescriben al final
   * del nuevo fichero tmp. */
  if ((fout = fopen("partida_tmp.txt", "w")) != NULL) {
    int skip = 0;
    if ((fin = fopen("partida.txt", "r")) != NULL) {
      while (fgets(line, sizeof(line), fin)) {
        char t[512];
        strncpy(t, line, sizeof(t)); // Copia la línea a un buffer temporal para analizarla
        t[sizeof(t) - 1] = '\0'; // Asegura la terminación nula del string
        t[strcspn(t, "\r\n")] = '\0'; // Elimina saltos de linea
        if (!strncmp(t, "JUGADOR:", 8) && strlen(t) > 9)
          skip = (atoi(t + 9) == jug->id_jugador); // Omite las lineas del jugador actual
        if (!skip)
          fputs(line, fout); // Mantiene intacta la partida de los demas jugadores
      }
      fclose(fin);
    }
    // Escribe la base del jugador
    fprintf(fout, "JUGADOR: %d\nSALA: %02d\n", jug->id_jugador,
            *id_sala_actual);

    // Guarda solo los objetos que han cambiado de ubicacion
    for (int i = 0; i < num_objetos; i++) {
      for (int j = 0; j < num_objetos; j++) {
        if (strcmp(lista_objetos[i].id_objeto, objetos_archivo[j].id_objeto) == 0) {
          if (lista_objetos[i].localizacion_objeto != objetos_archivo[j].localizacion_objeto) {
            fprintf(fout, "OBJETO: %s-%02d\n", lista_objetos[i].id_objeto, lista_objetos[i].localizacion_objeto); // Guarda el ID del objeto y su nueva ubicación (0 = inventario, >0 = sala)
          }
          break;
        }
      }
    }

    // Guarda solo las conexiones cuyo estado ha cambiado
    for (int i = 0; i < num_conexiones; i++) {
      for (int j = 0; j < num_conexiones; j++) {
        if (strcmp(lista_conexiones[i].id_conexion, conexiones_archivo[j].id_conexion) == 0) {
          if (lista_conexiones[i].estado_conexion != conexiones_archivo[j].estado_conexion) {
            fprintf(fout, "CONEXION: %s-%s\n", lista_conexiones[i].id_conexion, lista_conexiones[i].estado_conexion ? "Activa" : "Bloqueada"); // Guarda el ID de la conexión y su nuevo estado (Activa o Bloqueada)
          }
          break;
        }
      }
    }

    // Guarda solo los puzles cuyo estado ha cambiado
    for (int i = 0; i < num_puzles; i++) {
      for (int j = 0; j < num_puzles; j++) {
        if (strcmp(lista_puzles[i].id_puzle, puzles_archivo[j].id_puzle) == 0) {
          if (lista_puzles[i].resuelto != puzles_archivo[j].resuelto) {
            fprintf(fout, "PUZZLE: %s-%s\n", lista_puzles[i].id_puzle, lista_puzles[i].resuelto ? "Resuelto" : "Pendiente"); // Guarda el ID del puzle y su nuevo estado (Resuelto o Pendiente)
          }
          break;
        }
      }
    }

    fclose(fout);
    reemplazar("partida.txt", "partida_tmp.txt");
  }

  // Liberamos la memoria temporal puramente dinamica
  if (objetos_archivo)
    free(objetos_archivo);
  if (conexiones_archivo)
    free(conexiones_archivo);
  if (puzles_archivo)
    free(puzles_archivo);

  /* Sincronizar inv (inventario externo) con el estado actual: objetos en localizacion 0 */
  if (inv->Inventario) {
    free(inv->Inventario);
    inv->Inventario = NULL;
  }
  inv->num_objetos = 0;
  for (int i = 0; i < num_objetos; i++) {
    if (lista_objetos[i].localizacion_objeto == 0) {
      Objetos *nuevo_array_inventario = realloc(inv->Inventario, (inv->num_objetos + 1) * sizeof(Objetos)); // Reasigna memoria para el inventario
      if (!nuevo_array_inventario) {
        free(inv->Inventario); // Libera la memoria del inventario previo
        inv->Inventario = NULL; // Inicializa el puntero a NULL
        inv->num_objetos = 0; // Inicializa el contador de objetos
        break;
      }
      inv->Inventario = nuevo_array_inventario; // Asigna el nuevo puntero al array de objetos
      memset(&inv->Inventario[inv->num_objetos], 0, sizeof(Objetos)); // Inicializa el nuevo objeto
      CPY(inv->Inventario[inv->num_objetos].id_objeto, lista_objetos[i].id_objeto); // Copia el ID del objeto
      inv->num_objetos++; // Incrementa el numero de objetos en el inventario
    }
  }

  /* 2. Actualizacion de jugadores.txt:
   *    Solo se modifica el campo inventario del jugador actual;
   *    el resto de campos y los demas jugadores permanecen intactos. */
  fin = fopen("jugadores.txt", "r");
  fout = fopen("jugadores_tmp.txt", "w");
  if (fin && fout) {
    while (fgets(line, sizeof(line), fin)) {
      if (line[0] == '/' && line[1] == '/') {
        fputs(line, fout); // Copia comentarios sin modificar
        continue;
      }
      char linea_auxiliar[512];
      strncpy(linea_auxiliar, line, sizeof(linea_auxiliar) - 1); // Copia la línea a un buffer temporal para analizarla
      linea_auxiliar[sizeof(linea_auxiliar) - 1] = '\0'; // Asegura la terminación nula del string
      linea_auxiliar[strcspn(linea_auxiliar, "\r\n")] = '\0'; // Elimina saltos de linea
      char copia_linea_aux[512];
      strncpy(copia_linea_aux, linea_auxiliar, sizeof(copia_linea_aux) - 1); // Copia la línea a otro buffer para tokenizar sin modificar el original
      copia_linea_aux[sizeof(copia_linea_aux) - 1] = '\0';
      char *id = strtok(copia_linea_aux, "-"), *nom = strtok(NULL, "-"), *nick = strtok(NULL, "-"), *pw = strtok(NULL, "-"); // Tokeniza la línea para extraer los campos del jugador
      if (!id || !nom || !nick || !pw || atoi(id) != jug->id_jugador) {
        fputs(line, fout); //Mantiene intacto el resto de jugadores
        continue;
      }
      // Reescribe la linea del jugador actual con el inventario actualizado
      fprintf(fout, "%s-%s-%s-%s", id, nom, nick, pw);
      int items_escritos = 0;
      for (int i = 0; i < inv->num_objetos; i++)
        fprintf(fout, "%s%s", items_escritos++ ? "," : "-", inv->Inventario[i].id_objeto); // Guarda los objetos del inventario separados por comas
      fprintf(fout, "\n");
    }
  }

  // Reemplaza el fichero original solo si ambos archivos se manejaron correctamente; de lo contrario, elimina el temporal para evitar corrupciones
  if (fin)
    fclose(fin);
  if (fout)
    fclose(fout);
  if (fin && fout)
    reemplazar("jugadores.txt", "jugadores_tmp.txt");
  else
    remove("jugadores_tmp.txt");
}

//Precondicion: recibe un puntero a un jugador, el ID de la sala actual y los punteros a las estructuras y sus contadores.
//Postcondicion: reinicia el progreso del jugador, limpiando su inventario y recargando el estado puro del mundo desde los ficheros.
void reinicio(Jugadores **jugadores, int indice_jugador, int *id_sala_actual, Inventario *inv, Objetos **lista_objetos, int *num_objetos, Conexiones **lista_conexiones, int *num_conexiones, Puzles **lista_puzles, int *num_puzles) {
  // 1. Reiniciar sala actual
  *id_sala_actual = 1;

  // 2. Limpiar inventario
  if (inv->Inventario) {
    free(inv->Inventario);
    inv->Inventario = NULL;
  }
  inv->num_objetos = 0;

  // 3. Recargar el estado puro del mundo desde los ficheros
  if (*lista_objetos) {
    free(*lista_objetos);
    *lista_objetos = NULL;
  }
  *num_objetos = leer_objetos(lista_objetos);

  if (*lista_conexiones) {
    free(*lista_conexiones);
    *lista_conexiones = NULL;
  }
  *num_conexiones = leer_conexiones(lista_conexiones);

  if (*lista_puzles) {
    free(*lista_puzles);
    *lista_puzles = NULL;
  }
  *num_puzles = leer_puzles(lista_puzles);
}

//Precondicion: recibe el ID del jugador
//Postcondicion: devuelve 1 si el jugador tiene una partida guardada en partida.txt, 0 en caso contrario
int existePartida(int id_jugador) {
    FILE *f = fopen("partida.txt", "r");
    if (!f) return 0;

    char linea[512]; // Linea que se lee del fichero
    char busqueda1[64]; // Busqueda para el ID del jugador
    sprintf(busqueda1, "JUGADOR: %d", id_jugador);
    char busqueda2[64]; // Busqueda para el ID del jugador con ceros a la izquierda
    sprintf(busqueda2, "JUGADOR: %02d", id_jugador);

    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = '\0'; // Elimina saltos de linea
        if (strcmp(line, busqueda1) == 0 || strcmp(line, busqueda2) == 0) { // Compara la linea con las busquedas
            fclose(f); // Cierra el fichero
            return 1; // Devuelve 1 si encuentra la partida
        }
    }

    fclose(f);
    return 0;
}
