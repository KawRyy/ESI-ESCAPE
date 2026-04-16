#include "gestion.h"
#include "estructuras.h"
#include "ficheros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CPY(d, s) (strncpy((d), (s), sizeof(d) - 1), (d)[sizeof(d) - 1] = '\0')

static void reemplazar(const char *orig, const char *tmp) {
  remove(orig);
  rename(tmp, orig);
}

int cargarPartida(Jugadores *jug, int *id_sala_actual, Objetos **lista_objetos,
                  Conexiones **lista_conexiones, Puzles **lista_puzles) {
  FILE *f;
  char line[512];

  // Recarga el estado base limpio en memoria para evitar el sangrado de estado (State Bleed)
  if (*lista_objetos) { free(*lista_objetos); *lista_objetos = NULL; }
  int num_objetos = leer_objetos(lista_objetos);

  if (*lista_conexiones) { free(*lista_conexiones); *lista_conexiones = NULL; }
  int num_conexiones = leer_conexiones(lista_conexiones);

  if (*lista_puzles) { free(*lista_puzles); *lista_puzles = NULL; }
  int num_puzles = leer_puzles(lista_puzles);

  // 1. Inicializamos la sala donde va a empezar el jugador
  *id_sala_actual = 1;

  // 2. Aplicamos el inventario del jugador como estado inicial en las
  // estructuras globales (localizacion = 0)
  for (int i = 0; i < jug->num_objetos; i++) {
    for (int j = 0; j < num_objetos; j++) {
      if (strcmp((*lista_objetos)[j].id_objeto, jug->objetos[i].id_objeto) == 0) {
        (*lista_objetos)[j].localizacion_objeto = 0;
        break;
      }
    }
  }

  // 3. Sobreescribir el resto del mundo con el progreso guardado en partida.txt:
  if ((f = fopen("ficheros/partida.txt", "r")) == NULL)
    return 1; // Si no hay archivo, devolvemos 1 (comienza limpio)

  int en_bloque = 0;
  while (fgets(line, sizeof(line), f)) {
    line[strcspn(line, "\r\n")] = '\0';
    if (line[0] == '/' || line[0] == '\0')
      continue;

    // Creamos una copia de la linea para tokenizarla y obtener la clave y el valor
    char line_copy[512];
    strncpy(line_copy, line, sizeof(line_copy) - 1);
    line_copy[sizeof(line_copy) - 1] = '\0';

    char *clave = strtok(line_copy, ":");
    char *valor = strtok(NULL, "");
    if (!clave || !valor)
      continue;

    // Eliminamos espacio inicial en el valor si lo hay
    if (valor[0] == ' ')
      valor++;

    if (strcmp(clave, "JUGADOR") == 0) {
      en_bloque = (strcmp(valor, jug->id_jugador) == 0); /* Activa la lectura solo si es el bloque del jugador */
      continue;
    }
    if (!en_bloque)
      continue;

    if (strcmp(clave, "SALA") == 0) {
      *id_sala_actual = atoi(valor);
      continue;
    }
    if (strcmp(clave, "OBJETO") == 0) {
      /* Sobreescribe la ubicacion del objeto en las estructuras globales */
      char *id_objeto_str = strtok(valor, "-"), *loc_str = strtok(NULL, "-");
      if (id_objeto_str && loc_str) {
        int loc = atoi(loc_str);
        for (int i = 0; i < num_objetos; i++) {
          if (strcmp((*lista_objetos)[i].id_objeto, id_objeto_str) == 0) {
            (*lista_objetos)[i].localizacion_objeto = loc;
            break;
          }
        }
      }
      continue;
    }
    if (strcmp(clave, "CONEXION") == 0) {
      /* Sobreescribe el estado de la conexion en las estructuras globales */
      char *id_conexion_str = strtok(valor, "-"),
           *estado_str = strtok(NULL, "-");
      if (id_conexion_str && estado_str) {
        for (int i = 0; i < num_conexiones; i++) {
          if (strcmp((*lista_conexiones)[i].id_conexion, id_conexion_str) == 0) {
            (*lista_conexiones)[i].estado_conexion =
                (strcmp(estado_str, "Activa") == 0) ? 1 : 0;
            break;
          }
        }
      }
      continue;
    }
    if (strcmp(clave, "PUZZLE") == 0) {
      /* Sobreescribe el estado del puzle en las estructuras globales */
      char *id_puzle_str = strtok(valor, "-"), *estado_str = strtok(NULL, "-");
      if (id_puzle_str && estado_str) {
        for (int i = 0; i < num_puzles; i++) {
          if (strcmp((*lista_puzles)[i].id_puzle, id_puzle_str) == 0) {
            (*lista_puzles)[i].resuelto =
                (strcmp(estado_str, "Resuelto") == 0) ? 1 : 0;
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

void guardarPartida(Jugadores *jug, int *id_sala_actual, Objetos *lista_objetos,
                    Conexiones *lista_conexiones, Puzles *lista_puzles) {
  FILE *fin, *fout;
  char line[512];

  // Cargamos en memoria los listados limpios base directamente para extraer cuentas puras
  Objetos *base_obj = NULL;
  Conexiones *base_con = NULL;
  Puzles *base_puz = NULL;

  int num_objetos = leer_objetos(&base_obj);
  int num_conexiones = leer_conexiones(&base_con);
  int num_puzles = leer_puzles(&base_puz);

  /* 1. Actualizacion de partida.txt:
   *    En lugar de modificar en linea, se copia todo el contenido original
   * excepto las entradas del jugador en cuestion, que se reescriben al final
   * del nuevo fichero tmp. */
  if ((fout = fopen("ficheros/partida_tmp.txt", "w")) != NULL) {
    int skip = 0;
    if ((fin = fopen("ficheros/partida.txt", "r")) != NULL) {
      while (fgets(line, sizeof(line), fin)) {
        char t[512];
        strncpy(t, line, sizeof(t));
        t[strcspn(t, "\r\n")] = '\0';
        if (!strncmp(t, "JUGADOR:", 8) && strlen(t) > 9)
          skip = strcmp(t + 9, jug->id_jugador) == 0; /* Omite las lineas del jugador actual */
        if (!skip)
          fputs(line, fout); /* Mantiene intacta la partida de los demas jugadores */
      }
      fclose(fin);
    }
    /* Escribe la base del jugador */
    fprintf(fout, "JUGADOR: %s\nSALA: %02d\n", jug->id_jugador, *id_sala_actual);

    /* Guarda solo los objetos que han cambiado de ubicacion */
    for (int i = 0; i < num_objetos; i++) {
      for (int j = 0; j < num_objetos; j++) {
        if (strcmp(lista_objetos[i].id_objeto, base_obj[j].id_objeto) == 0) {
          if (lista_objetos[i].localizacion_objeto != base_obj[j].localizacion_objeto) {
            fprintf(fout, "OBJETO: %s-%02d\n", lista_objetos[i].id_objeto,
                    lista_objetos[i].localizacion_objeto);
          }
          break;
        }
      }
    }

    /* Guarda solo las conexiones cuyo estado ha cambiado */
    for (int i = 0; i < num_conexiones; i++) {
      for (int j = 0; j < num_conexiones; j++) {
        if (strcmp(lista_conexiones[i].id_conexion, base_con[j].id_conexion) == 0) {
          if (lista_conexiones[i].estado_conexion != base_con[j].estado_conexion) {
            fprintf(fout, "CONEXION: %s-%s\n", lista_conexiones[i].id_conexion,
                    lista_conexiones[i].estado_conexion ? "Activa" : "Bloqueada");
          }
          break;
        }
      }
    }

    /* Guarda solo los puzles cuyo estado ha cambiado */
    for (int i = 0; i < num_puzles; i++) {
      for (int j = 0; j < num_puzles; j++) {
        if (strcmp(lista_puzles[i].id_puzle, base_puz[j].id_puzle) == 0) {
          if (lista_puzles[i].resuelto != base_puz[j].resuelto) {
            fprintf(fout, "PUZZLE: %s-%s\n", lista_puzles[i].id_puzle,
                    lista_puzles[i].resuelto ? "Resuelto" : "Pendiente");
          }
          break;
        }
      }
    }

    fclose(fout);
    reemplazar("ficheros/partida.txt", "ficheros/partida_tmp.txt");
  }

  /* Liberamos la memoria temporal puramente dinamica */
  if (base_obj) free(base_obj);
  if (base_con) free(base_con);
  if (base_puz) free(base_puz);

  /* 2. Actualizacion de jugadores.txt:
   *    Solo se modifica el campo inventario del jugador actual;
   *    el resto de campos y los demas jugadores permanecen intactos. */
  fin = fopen("ficheros/jugadores.txt", "r");
  fout = fopen("ficheros/jugadores_tmp.txt", "w");
  if (fin && fout) {
    while (fgets(line, sizeof(line), fin)) {
      if (line[0] == '/' && line[1] == '/') {
        fputs(line, fout);
        continue;
      }
      char t[512];
      strncpy(t, line, sizeof(t) - 1);
      t[sizeof(t) - 1] = '\0';
      t[strcspn(t, "\r\n")] = '\0';
      char t_copy[512];
      strncpy(t_copy, t, sizeof(t_copy) - 1);
      t_copy[sizeof(t_copy) - 1] = '\0';
      char *id = strtok(t_copy, "-"), *nom = strtok(NULL, "-"),
           *nick = strtok(NULL, "-"), *pw = strtok(NULL, "-");
      if (!id || !nom || !nick || !pw || strcmp(id, jug->id_jugador) != 0) {
        fputs(line, fout); /* Mantiene intacto el resto de jugadores */
        continue;
      }
      /* Reescribe la linea del jugador actual con el inventario actualizado */
      fprintf(fout, "%s-%s-%s-%s", id, nom, nick, pw);
      int cnt = 0;
      for (int i = 0; i < num_objetos; i++)
        if (lista_objetos[i].localizacion_objeto == 0)
          fprintf(fout, "%s%s", cnt++ ? "," : "-", lista_objetos[i].id_objeto);
      fprintf(fout, "\n");
    }
  }
  if (fin)
    fclose(fin);
  if (fout)
    fclose(fout);
  if (fin && fout)
    reemplazar("ficheros/jugadores.txt", "ficheros/jugadores_tmp.txt");
  else
    remove("ficheros/jugadores_tmp.txt");
}

void borrarPartida(char *id_jugador) {
  FILE *fin, *fout;
  char line[512];

  // 1. Limpiar el archivo de la partida global
  if ((fout = fopen("ficheros/partida_tmp.txt", "w")) != NULL) {
    int skip = 0;
    if ((fin = fopen("ficheros/partida.txt", "r")) != NULL) {
      while (fgets(line, sizeof(line), fin)) {
        char t[512];
        strncpy(t, line, sizeof(t));
        t[strcspn(t, "\r\n")] = '\0';
        if (!strncmp(t, "JUGADOR:", 8) && strlen(t) > 9)
          skip = strcmp(t + 9, id_jugador) == 0;
        if (!skip)
          fputs(line, fout);
      }
      fclose(fin);
    }
    fclose(fout);
    reemplazar("ficheros/partida.txt", "ficheros/partida_tmp.txt");
  }

  // 2. Limpiar el inventario residual del jugador en el fichero de usuarios
  fin = fopen("ficheros/jugadores.txt", "r");
  fout = fopen("ficheros/jugadores_tmp.txt", "w");
  if (fin && fout) {
    while (fgets(line, sizeof(line), fin)) {
      if (line[0] == '/' && line[1] == '/') {
        fputs(line, fout);
        continue;
      }
      char t[512];
      strncpy(t, line, sizeof(t) - 1);
      t[sizeof(t) - 1] = '\0';
      t[strcspn(t, "\r\n")] = '\0';
      
      char t_copy[512];
      strncpy(t_copy, t, sizeof(t_copy) - 1);
      t_copy[sizeof(t_copy) - 1] = '\0';
      
      char *id = strtok(t_copy, "-"), *nom = strtok(NULL, "-"),
           *nick = strtok(NULL, "-"), *pw = strtok(NULL, "-");

      if (!id || !nom || !nick || !pw || strcmp(id, id_jugador) != 0) {
        fputs(line, fout); // Conserva los demás jugadores sin tocar su inventario
        continue;
      }
      // Reescribe la linea del jugador actual PERO SIN incluir objetos adicionales
      fprintf(fout, "%s-%s-%s-%s\n", id, nom, nick, pw);
    }
  }
  if (fin) fclose(fin);
  if (fout) fclose(fout);
  if (fin && fout) {
    reemplazar("ficheros/jugadores.txt", "ficheros/jugadores_tmp.txt");
  } else {
    remove("ficheros/jugadores_tmp.txt");
  }
}

void nueva_partida(Jugadores *jugador, int *id_sala_actual,
                   Objetos **lista_objetos, Conexiones **lista_conexiones,
                   Puzles **lista_puzles) {
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