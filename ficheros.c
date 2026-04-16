#include "ficheros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CPY(d, s) (strncpy((d), (s), sizeof(d) - 1), (d)[sizeof(d) - 1] = '\0')

static void reemplazar(const char *orig, const char *tmp) {
  remove(orig);
  rename(tmp, orig);
}
//Precondicion: recibe la ruta de un fichero de texto
//Postcondicion: devuelve el numero de lineas validas (no vacias ni comentarios) o -1 si no se pudo abrir el fichero 
int contar_lineas(const char *ruta) {
  FILE *f = fopen(ruta, "r"); //Abrir fichero modo lectura
  if (!f)
    return -1; // Si no se pudo abrir, devuelve -1
  char line[512]; // Buffer para leer lineas
  int count = 0;
  while (fgets(line, sizeof(line), f)) {
    line[strcspn(line, "\r\n")] = '\0'; // Elimina saltos de linea
    if (line[0] == '/' || line[0] == '\0') // Ignora lineas vacias o comentarios
      continue;
    count++; // Cuenta lineas validas
  }
  fclose(f);
  return count;
}

//Precondicion: recibe un puntero a un array de Salas (inicialmente NULL)
//Postcondicion: lee el fichero de salas, asigna dinamicamente el array y lo llena con los datos. Devuelve el numero de salas leidas o -1 si hubo error. El formato del fichero debe ser: ID-NOMBRE-TIPO-DESCRIPCION (ej: 01-Sala Inicial-INICIAL-Descripcion de la sala)
static int leer_salas(Salas **salas) {
  int total = contar_lineas("ficheros/salas.txt"); // Cuenta el numero de lineas validas para dimensionar el array
  *salas = NULL;
  if (total <= 0)
    return total < 0 ? -1 : 0; // Si no hay lineas validas, devuelve 0; si hubo error al abrir, devuelve -1

  *salas = calloc(total, sizeof(Salas)); // Asigna memoria para el array de salas
  if (!*salas)
    return -1; // Si no se pudo asignar memoria, devuelve -1

  FILE *f = fopen("ficheros/salas.txt", "r"); // Abre el fichero de salas para lectura
  if (!f)
    free(*salas); // Si no se pudo abrir el fichero, libera la memoria asignada y devuelve -1
    *salas = NULL;
    return -1;

  char line[512]; // Buffer para leer lineas
  int n = 0;
  while (fgets(line, sizeof(line), f)) {
    line[strcspn(line, "\r\n")] = '\0'; // Elimina saltos de linea
    if (line[0] == '/' || line[0] == '\0')
      continue; // Ignora lineas vacias o comentarios
    char *id_sala_str = strtok(line, "-"), *nombre_sala_str = strtok(NULL, "-"), *tipo_sala_str = strtok(NULL, "-"), *descripcion_sala_str = strtok(NULL, "-"); // Separa campos por '-'
    if (!id_sala_str || !nombre_sala_str || !tipo_sala_str || !descripcion_sala_str)
      continue;

    (*salas)[n].id_sala = atoi(id_sala_str); // Convierte el ID a numero entero
    CPY((*salas)[n].nombre_sala, nombre_sala_str); // Copia el nombre de la sala
    (*salas)[n].tipo_sala = (strcmp(tipo_sala_str, "INICIAL") == 0)  ? 1 : (strcmp(tipo_sala_str, "NORMAL") == 0) ? 2 : (strcmp(tipo_sala_str, "SALIDA") == 0) ? 3 : 0; // Asigna el valor numerico segun el tipo
    CPY((*salas)[n].descripcion_sala, descripcion_sala_str); // Copia la descripcion de la sala
    n++;
  }
  fclose(f);
  return n;
}

//Precondicion: recibe un puntero a un array de Puzles (inicialmente NULL)
//Postcondicion: lee el fichero de puzles, asigna dinamicamente el array y lo llena con los datos. Devuelve el numero de puzles leidos o -1 si hubo error. El formato del fichero debe ser: ID-SALA-TIPO-DESCRIPCION-SOLUCION (ej: P01-01-CODIGO-Descripcion del puzle-1234)
static int leer_puzles(Puzles **puzles) {
  int total = contar_lineas("ficheros/puzles.txt"); // Cuenta el numero de lineas validas para dimensionar el array
  *puzles = NULL;
  if (total <= 0)
    return total < 0 ? -1 : 0; // Si no hay lineas validas, devuelve 0; si hubo error al abrir, devuelve -1

  *puzles = calloc(total, sizeof(Puzles)); // Asigna memoria para el array de puzles
  if (!*puzles)
    return -1; // Si no se pudo asignar memoria, devuelve -1

  FILE *f = fopen("ficheros/puzles.txt", "r"); // Abre el fichero de puzles
  if (!f)
    free(*puzles); // Si no se pudo abrir el fichero, libera la memoria asignada y devuelve -1
    *puzles = NULL;
    return -1;

  char line[512];
  int n = 0;
  while (fgets(line, sizeof(line), f)) {
    line[strcspn(line, "\r\n")] = '\0'; // Elimina saltos de linea
    if (line[0] == '/' || line[0] == '\0')
      continue; // Ignora lineas vacias o comentarios
    char *id_puzle_str = strtok(line, "-");
    strtok(NULL, "-"); // Ignora el campo tipo del fichero, se determina dinámicamente por el contenido de la solución
    char *id_sala_puzle_str = strtok(NULL, "-"), *descripcion_puzle_str = strtok(NULL, "-"), *solucion_puzle_str = strtok(NULL, "-"); // Separa campos por '-'
    if (!id_puzle_str || !id_sala_puzle_str || !descripcion_puzle_str || !solucion_puzle_str)
      continue;

    CPY((*puzles)[n].id_puzle, id_puzle_str); // Copia el ID del puzle
    (*puzles)[n].id_sala_puzle = atoi(id_sala_puzle_str); // Convierte el ID de sala a entero

    // Determina el tipo de elemento del puzle (1: CÓDIGO ; 2: PALABRA)
    // Usamos el contenido de la solución. Si posee letras, será una PALABRA.
    (*puzles)[n].tipo_elemento = (strspn(solucion_puzle_str, "0123456789") == strlen(solucion_puzle_str)) ? 1 : 2;

    CPY((*puzles)[n].descripcion_puzle, descripcion_puzle_str); // Copia la descripción del puzle
    CPY((*puzles)[n].solucion_puzle, solucion_puzle_str); // Copia la solución del puzle
    n++;
  }
  fclose(f);
  return n;
}

//Precondicion: recibe un puntero a un array de Conexiones (inicialmente NULL)
//Postcondicion: lee el fichero de conexiones, asigna dinamicamente el array y lo llena con los datos. Devuelve el numero de conexiones leidas o -1 si hubo error. El formato del fichero debe ser: ID-SALA_ORIGEN-SALA_DESTINO-ESTADO-CONICIONANTE (ej: C01-01-02-Activa-OB01)
static int leer_conexiones(Conexiones **conexiones) {
  int total = contar_lineas("ficheros/conexiones.txt"); // Cuenta el numero de lineas validas para dimensionar el array
  *conexiones = NULL;
  if (total <= 0)
    return total < 0 ? -1 : 0; // Si no hay lineas validas, devuelve 0; si hubo error al abrir, devuelve -1

  *conexiones = calloc(total, sizeof(Conexiones)); // Asigna memoria para el array de conexiones
  if (!*conexiones)
    return -1; // Si no se pudo asignar memoria, devuelve -1

  FILE *f = fopen("ficheros/conexiones.txt", "r"); // Abre el fichero de conexiones
  if (!f)
    free(*conexiones); // Si no se pudo abrir el fichero, libera la memoria asignada y devuelve -1
    *conexiones = NULL;
    return -1;

  char line[512];
  int n = 0;
  while (fgets(line, sizeof(line), f)) {
    line[strcspn(line, "\r\n")] = '\0'; // Elimina saltos de linea
    if (line[0] == '/' || line[0] == '\0')
      continue; // Ignora lineas vacias o comentarios
    char *id_conexion_str = strtok(line, "-"), *id_sala_orig_str = strtok(NULL, "-"), *id_sala_dest_str = strtok(NULL, "-"), *estado_conexion_str = strtok(NULL, "-"), *id_condicionante_str = strtok(NULL, "-"); // Separa campos por '-'
    if (!id_conexion_str || !id_sala_orig_str || !id_sala_dest_str || !estado_conexion_str || !id_condicionante_str)
      continue;

    CPY((*conexiones)[n].id_conexion, id_conexion_str); // Copia el ID de la conexión
    (*conexiones)[n].id_sala_orig = atoi(id_sala_orig_str); // Convierte el ID origen a entero
    (*conexiones)[n].id_sala_dest = atoi(id_sala_dest_str); // Convierte el ID destino a entero
    (*conexiones)[n].estado_conexion = (strcmp(estado_conexion_str, "Activa") == 0) ? 1 : 0; // Determina si la conexion esta activa
    CPY((*conexiones)[n].id_condicionante, id_condicionante_str); // Copia el ID del condicionante (ej: "OB01", "P01", "0")
    n++;
  }
  fclose(f);
  return n;
}

//Precondicion: recibe un puntero a un array de Objetos (inicialmente NULL)
//Postcondicion: lee el fichero de objetos, asigna dinamicamente el array y lo
static int leer_objetos(Objetos **objetos) {
  int total = contar_lineas("ficheros/objetos.txt"); // Cuenta el numero de lineas validas para dimensionar el array
  *objetos = NULL;
  if (total <= 0)
    return total < 0 ? -1 : 0; // Si no hay lineas validas, devuelve 0; si hubo error al abrir, devuelve -1

  *objetos = calloc(total, sizeof(Objetos)); // Asigna memoria para el array de objetos
  if (!*objetos)
    return -1; // Si no se pudo asignar memoria, devuelve -1

  FILE *f = fopen("ficheros/objetos.txt", "r"); // Abre el fichero de objetos
  if (!f)
    free(*objetos); // Si no se pudo abrir el fichero, libera la memoria asignada y devuelve -1
    *objetos = NULL;
    return -1;

  char line[512];
  int n = 0;
  while (fgets(line, sizeof(line), f)) {
    line[strcspn(line, "\r\n")] = '\0'; // Elimina saltos de linea
    if (line[0] == '/' || line[0] == '\0')
      continue; // Ignora lineas vacias o comentarios
    char *id_objeto_str = strtok(line, "-"), *nombre_objeto_str = strtok(NULL, "-"), *descripcion_objeto_str = strtok(NULL, "-"), *localizacion_objeto_str = strtok(NULL, "-"); // Separa campos por '-'
    if (!id_objeto_str || !nombre_objeto_str || !descripcion_objeto_str || !localizacion_objeto_str)
      continue;

    CPY((*objetos)[n].id_objeto, id_objeto_str); // Copia el ID del objeto
    CPY((*objetos)[n].nombre_objeto, nombre_objeto_str); // Copia el nombre del objeto
    CPY((*objetos)[n].descripcion_objeto, descripcion_objeto_str); // Copia la descripcion del objeto
    (*objetos)[n].localizacion_objeto = atoi(localizacion_objeto_str); // Convierte la localizacion a entero
    n++;
  }
  fclose(f);
  return n;
}

//Precondicion: recibe un puntero a un array de Jugadores (inicialmente NULL)
//Postcondicion: lee el fichero de jugadores, asigna dinamicamente el array y lo llena con los datos. Devuelve el numero de jugadores leidos o -1 si hubo error. El formato del fichero debe ser: ID-NOMBRE-NICKNAME-CONTRASENA-INVENTARIO (ej: J01-Jugador Uno-jug1-pass1-OB01,OB02)
static int leer_jugadores(Jugadores **jugadores) {
  int total = contar_lineas("ficheros/jugadores.txt"); // Cuenta el numero de lineas validas para dimensionar el array
  *jugadores = NULL;
  if (total <= 0)
    return total < 0 ? -1 : 0; // Si no hay lineas validas, devuelve 0; si hubo error al abrir, devuelve -1

  *jugadores = calloc(total, sizeof(Jugadores)); // Asigna memoria para el array de jugadores
  if (!*jugadores)
    return -1; // Si no se pudo asignar memoria, devuelve -1

  FILE *f = fopen("ficheros/jugadores.txt", "r"); // Abre el fichero de jugadores
  if (!f)
    free(*jugadores); // Si no se pudo abrir el fichero, libera la memoria asignada y devuelve -1
    *jugadores = NULL;
    return -1;

  char line[512];
  int n = 0;
  while (fgets(line, sizeof(line), f)) {
    line[strcspn(line, "\r\n")] = '\0'; // Elimina saltos de linea
    if (line[0] == '/' || line[0] == '\0')
      continue; // Ignora lineas vacias o comentarios
    char *id_jugador_str = strtok(line, "-"), *nombre_jugador_str = strtok(NULL, "-"), *nickname_str = strtok(NULL, "-"), *contrasena_str = strtok(NULL, "-"), *inv_str = strtok(NULL, "-");
    if (!id_jugador_str || !nombre_jugador_str || !nickname_str || !contrasena_str)
      continue;

    CPY((*jugadores)[n].id_jugador, id_jugador_str); // Copia el ID del jugador
    CPY((*jugadores)[n].nombre_jugador, nombre_jugador_str); // Copia el nombre del jugador
    CPY((*jugadores)[n].nickname, nickname_str); // Copia el nickname del jugador
    CPY((*jugadores)[n].contrasena, contrasena_str); // Copia la contraseña del jugador

    // El inventario de cada jugador mantiene su asignación dinámica por ser local anidada
    if (inv_str) {
      char *obj_str = strtok(inv_str, ",");
      while (obj_str) {
        Objetos *tmp_obj = realloc((*jugadores)[n].objetos, ((*jugadores)[n].num_objetos + 1) * sizeof(Objetos)); // Redimensiona el array de objetos del jugador para agregar un nuevo objeto
        if (!tmp_obj)
          break;
        (*jugadores)[n].objetos = tmp_obj; // Asigna el nuevo puntero al array de objetos del jugador
        memset(&((*jugadores)[n].objetos[(*jugadores)[n].num_objetos]), 0, sizeof(Objetos)); // Limpia la nueva entrada de objeto
        CPY((*jugadores)[n].objetos[(*jugadores)[n].num_objetos].id_objeto, obj_str); // Copia el ID del objeto al inventario del jugador
        (*jugadores)[n].num_objetos++;
        obj_str = strtok(NULL, "-"); 
      }
    }
    n++;
  }
  fclose(f);
  return n;
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
int cargarPartida(Jugadores *jug, int *id_sala_actual, Objetos **lista_objetos, Conexiones **lista_conexiones, Puzles **lista_puzles) {
  FILE *f;
  char line[512];

  // Recarga el estado base limpio en memoria para evitar el sangrado de estado
  // (State Bleed)
  if (*lista_objetos) {
    free(*lista_objetos); // Si ya hay un array de objetos cargado, lo libera para evitar fugas de memoria
    *lista_objetos = NULL;
  }
  int num_objetos = leer_objetos(lista_objetos); // Carga el listado base de objetos para luego aplicar el progreso del jugador

  if (*lista_conexiones) {
    free(*lista_conexiones); // Si ya hay un array de conexiones cargado, lo libera para evitar fugas de memoria
    *lista_conexiones = NULL;
  }
  int num_conexiones = leer_conexiones(lista_conexiones); // Carga el listado base de conexiones para luego aplicar el progreso del jugador

  if (*lista_puzles) {
    free(*lista_puzles); // Si ya hay un array de puzles cargado, lo libera para evitar fugas de memoria
    *lista_puzles = NULL;
  }
  int num_puzles = leer_puzles(lista_puzles); // Carga el listado base de puzles para luego aplicar el progreso del jugador

  // 1. Inicializamos la sala donde va a empezar el jugador
  *id_sala_actual = 1;

  // 2. Aplicamos el inventario del jugador como estado inicial en las
  // estructuras globales (localizacion = 0)
  for (int i = 0; i < jug->num_objetos; i++) {
    for (int j = 0; j < num_objetos; j++) {
      if (strcmp((*lista_objetos)[j].id_objeto, jug->objetos[i].id_objeto) == 0) {
        (*lista_objetos)[j].localizacion_objeto = 0; // Coloca el objeto en el inventario del jugador (localizacion 0)
        break;
      }
    }
  }

  // 3. Sobreescribir el resto del mundo con el progreso guardado en partida.txt:
  if ((f = fopen("ficheros/partida.txt", "r")) == NULL)
    return 1; // Si no hay archivo, devolvemos 1 (comienza limpio)

  int en_bloque = 0;
  while (fgets(line, sizeof(line), f)) {
    line[strcspn(line, "\r\n")] = '\0'; // Elimina saltos de linea
    if (line[0] == '/' || line[0] == '\0')
      continue;

    // Creamos una copia de la linea para tokenizarla y obtener la clave y el valor
    char line_copy[512];
    strncpy(line_copy, line, sizeof(line_copy) - 1); // Copia la línea a un buffer temporal para tokenizar
    line_copy[sizeof(line_copy) - 1] = '\0'; // Asegura la terminación nula del string

    char *clave = strtok(line_copy, ":");
    char *valor = strtok(NULL, "");
    if (!clave || !valor)
      continue;

    // Eliminamos espacio inicial en el valor si lo hay
    if (valor[0] == ' ')
      valor++;

    if (strcmp(clave, "JUGADOR") == 0) {
      en_bloque = (strcmp(valor, jug->id_jugador) == 0); // Activa la lectura solo si es el bloque del jugador
      continue;
    }
    if (!en_bloque) // Si no estamos en el bloque del jugador, ignoramos el resto de lineas
      continue;

    if (strcmp(clave, "SALA") == 0) {
      *id_sala_actual = atoi(valor); // Actualiza la sala actual del jugador
      continue;
    }
    if (strcmp(clave, "OBJETO") == 0) {
      // Sobreescribe la ubicacion del objeto en las estructuras globales
      char *id_objeto_str = strtok(valor, "-"), *loc_str = strtok(NULL, "-");
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
    if (strcmp(clave, "CONEXION") == 0) {
      // Sobreescribe el estado de la conexion en las estructuras globales
      char *id_conexion_str = strtok(valor, "-"), *estado_str = strtok(NULL, "-");
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
    if (strcmp(clave, "PUZZLE") == 0) {
      //Sobreescribe el estado del puzle en las estructuras globales
      char *id_puzle_str = strtok(valor, "-"), *estado_str = strtok(NULL, "-");
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
void guardarPartida(Jugadores *jug, int *id_sala_actual, Objetos *lista_objetos, Conexiones *lista_conexiones, Puzles *lista_puzles) {
  FILE *fin, *fout;
  char line[512];

  // Cargamos en memoria los listados limpios base directamente para extraer cuentas puras
  Objetos *base_obj = NULL;
  Conexiones *base_con = NULL;
  Puzles *base_puz = NULL;

  int num_objetos = leer_objetos(&base_obj); // Carga el listado base de objetos para comparar cambios de ubicación
  int num_conexiones = leer_conexiones(&base_con); // Carga el listado base de conexiones para comparar cambios de estado
  int num_puzles = leer_puzles(&base_puz); // Carga el listado base de puzles para comparar cambios de estado

  /* 1. Actualizacion de partida.txt:
   *    En lugar de modificar en linea, se copia todo el contenido original
   * excepto las entradas del jugador en cuestion, que se reescriben al final
   * del nuevo fichero tmp. */
  if ((fout = fopen("ficheros/partida_tmp.txt", "w")) != NULL) {
    int skip = 0;
    if ((fin = fopen("ficheros/partida.txt", "r")) != NULL) {
      while (fgets(line, sizeof(line), fin)) {
        char t[512];
        strncpy(t, line, sizeof(t)); // Copia la línea a un buffer temporal para analizarla
        t[sizeof(t) - 1] = '\0'; // Asegura la terminación nula del string
        t[strcspn(t, "\r\n")] = '\0'; // Elimina saltos de linea
        if (!strncmp(t, "JUGADOR:", 8) && strlen(t) > 9)
          skip = strcmp(t + 9, jug->id_jugador) == 0; // Omite las lineas del jugador actual
        if (!skip)
          fputs(line, fout); // Mantiene intacta la partida de los demas jugadores
      }
      fclose(fin);
    }
    // Escribe la base del jugador
    fprintf(fout, "JUGADOR: %s\nSALA: %02d\n", jug->id_jugador,
            *id_sala_actual);

    // Guarda solo los objetos que han cambiado de ubicacion
    for (int i = 0; i < num_objetos; i++) {
      for (int j = 0; j < num_objetos; j++) {
        if (strcmp(lista_objetos[i].id_objeto, base_obj[j].id_objeto) == 0) {
          if (lista_objetos[i].localizacion_objeto != base_obj[j].localizacion_objeto) {
            fprintf(fout, "OBJETO: %s-%02d\n", lista_objetos[i].id_objeto, lista_objetos[i].localizacion_objeto); // Guarda el ID del objeto y su nueva ubicación (0 = inventario, >0 = sala)
          }
          break;
        }
      }
    }

    // Guarda solo las conexiones cuyo estado ha cambiado
    for (int i = 0; i < num_conexiones; i++) {
      for (int j = 0; j < num_conexiones; j++) {
        if (strcmp(lista_conexiones[i].id_conexion, base_con[j].id_conexion) == 0) {
          if (lista_conexiones[i].estado_conexion != base_con[j].estado_conexion) {
            fprintf(fout, "CONEXION: %s-%s\n", lista_conexiones[i].id_conexion, lista_conexiones[i].estado_conexion ? "Activa" : "Bloqueada"); // Guarda el ID de la conexión y su nuevo estado (Activa o Bloqueada)
          }
          break;
        }
      }
    }

    // Guarda solo los puzles cuyo estado ha cambiado
    for (int i = 0; i < num_puzles; i++) {
      for (int j = 0; j < num_puzles; j++) {
        if (strcmp(lista_puzles[i].id_puzle, base_puz[j].id_puzle) == 0) {
          if (lista_puzles[i].resuelto != base_puz[j].resuelto) {
            fprintf(fout, "PUZZLE: %s-%s\n", lista_puzles[i].id_puzle, lista_puzles[i].resuelto ? "Resuelto" : "Pendiente"); // Guarda el ID del puzle y su nuevo estado (Resuelto o Pendiente)
          }
          break;
        }
      }
    }

    fclose(fout);
    reemplazar("ficheros/partida.txt", "ficheros/partida_tmp.txt");
  }

  // Liberamos la memoria temporal puramente dinamica
  if (base_obj)
    free(base_obj);
  if (base_con)
    free(base_con);
  if (base_puz)
    free(base_puz);

  /* Sincronizar memoria en caliente de Jugadores (inventario)
   * Asegura que el estado coincida si el usuario hace recargas en caliente */
  if (jug->objetos) {
    free(jug->objetos);
    jug->objetos = NULL;
  }
  jug->num_objetos = 0;
  for (int i = 0; i < num_objetos; i++) {
    if (lista_objetos[i].localizacion_objeto == 0) {
      Objetos *tmp = realloc(jug->objetos, (jug->num_objetos + 1) * sizeof(Objetos)); // Redimensiona el array de objetos del jugador para agregar un nuevo objeto
      if (!tmp) {
        free(jug->objetos); // Si no se pudo asignar memoria, libera el array actual y deja el inventario vacío
        jug->objetos = NULL;
        jug->num_objetos = 0;
        break;
      }
      jug->objetos = tmp;
      memset(&jug->objetos[jug->num_objetos], 0, sizeof(Objetos)); // Limpia la nueva entrada de objeto
      CPY(jug->objetos[jug->num_objetos].id_objeto, lista_objetos[i].id_objeto); // Copia el ID del objeto al inventario del jugador
      jug->num_objetos++;
    }
  }

  /* 2. Actualizacion de jugadores.txt:
   *    Solo se modifica el campo inventario del jugador actual;
   *    el resto de campos y los demas jugadores permanecen intactos. */
  fin = fopen("ficheros/jugadores.txt", "r");
  fout = fopen("ficheros/jugadores_tmp.txt", "w");
  if (fin && fout) {
    while (fgets(line, sizeof(line), fin)) {
      if (line[0] == '/' && line[1] == '/') {
        fputs(line, fout); // Copia comentarios sin modificar
        continue;
      }
      char t[512];
      strncpy(t, line, sizeof(t) - 1); // Copia la línea a un buffer temporal para analizarla
      t[sizeof(t) - 1] = '\0'; // Asegura la terminación nula del string
      t[strcspn(t, "\r\n")] = '\0'; // Elimina saltos de linea
      char t_copy[512];
      strncpy(t_copy, t, sizeof(t_copy) - 1); // Copia la línea a otro buffer para tokenizar sin modificar el original
      t_copy[sizeof(t_copy) - 1] = '\0';
      char *id = strtok(t_copy, "-"), *nom = strtok(NULL, "-"), *nick = strtok(NULL, "-"), *pw = strtok(NULL, "-"); // Tokeniza la línea para extraer los campos del jugador
      if (!id || !nom || !nick || !pw || strcmp(id, jug->id_jugador) != 0) {
        fputs(line, fout); //Mantiene intacto el resto de jugadores
        continue;
      }
      // Reescribe la linea del jugador actual con el inventario actualizado
      fprintf(fout, "%s-%s-%s-%s", id, nom, nick, pw);
      int cnt = 0;
      for (int i = 0; i < num_objetos; i++)
        if (lista_objetos[i].localizacion_objeto == 0)
          fprintf(fout, "%s%s", cnt++ ? "," : "-", lista_objetos[i].id_objeto); // Agrega al inventario del jugador los objetos que están en localización 0, separados por comas
      fprintf(fout, "\n");
    }
  }

  // Reemplaza el fichero original solo si ambos archivos se manejaron correctamente; de lo contrario, elimina el temporal para evitar corrupciones
  if (fin)
    fclose(fin);
  if (fout)
    fclose(fout);
  if (fin && fout)
    reemplazar("ficheros/jugadores.txt", "ficheros/jugadores_tmp.txt");
  else
    remove("ficheros/jugadores_tmp.txt");
}

//Precondicion: recibe un puntero a un jugador, el ID de la sala actual y los arrays de objetos, conexiones y puzles con su respectivo numero de elementos
//Postcondicion: reinicia el progreso del jugador, limpiando su inventario y recargando el estado puro del mundo desde los ficheros para comenzar una nueva partida. Devuelve 1 si se reinicio correctamente o 0 si hubo error al leer los ficheros (en cuyo caso se mantiene el estado actual)
void nueva_partida(Jugadores *jugador, int *id_sala_actual, Objetos **lista_objetos, Conexiones **lista_conexiones, Puzles **lista_puzles) {
  // 1. Reiniciar sala actual
  *id_sala_actual = 1;

  // 2. Limpiar inventario del jugador
  if (jugador->objetos) {
    free(jugador->objetos);
    jugador->objetos = NULL;
  }
  jugador->num_objetos = 0;

  // 3. Recargar el estado puro del mundo desde los ficheros
  if (*lista_objetos) {
    free(*lista_objetos);
    *lista_objetos = NULL;
  }
  leer_objetos(lista_objetos);

  if (*lista_conexiones) {
    free(*lista_conexiones);
    *lista_conexiones = NULL;
  }
  leer_conexiones(lista_conexiones);

  if (*lista_puzles) {
    free(*lista_puzles);
    *lista_puzles = NULL;
  }
  leer_puzles(lista_puzles);
}