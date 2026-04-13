#include "estructuras.h"
#include "gestion.h"
#include "ficheros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CPY(d, s) (strncpy((d), (s), sizeof(d) - 1), (d)[sizeof(d) - 1] = '\0')

static void reemplazar(const char *orig, const char *tmp) {
  remove(orig);
  rename(tmp, orig);
}

int cargarPartida(Partida *par, char *id_jugador) {
  FILE *f;
  char line[512];

  /* 1. Buscar jugador en jugadores.txt:
   *    Se abre el fichero y se busca linea a linea un usuario cuyo id_jugador
   *    coincida con el 'id_jugador' proporcionado como parametro. */
  Jugadores jug;
  memset(&jug, 0, sizeof(Jugadores));
  int hallado = 0;
  if ((f = fopen("ficheros/jugadores.txt", "r")) == NULL)
    return 0;
  while (fgets(line, sizeof(line), f)) {
    line[strcspn(line, "\r\n")] = '\0';
    if (line[0] == '/' || line[0] == '\0')
      continue;
    char line_copy[512];
    strncpy(line_copy, line, sizeof(line_copy) - 1);
    line_copy[sizeof(line_copy) - 1] = '\0';
    char *id = strtok(line_copy, "-"), *nom = strtok(NULL, "-"), *nick = strtok(NULL, "-"), *pw = strtok(NULL, "-"), *inv = strtok(NULL, "-"); /* Extrae campos separados por '-' */
    if (!id || !nom || !nick || strcmp(id, id_jugador) != 0) /* Ignora si faltan datos o el ID no coincide */
      continue;
    CPY(jug.id_jugador, id);
    CPY(jug.nombre_jugador, nom);
    if (pw)
      CPY(jug.contrasena, pw);
    if (inv) {
      /* Procesa los items en el inventario que vienen separados por comas */
      char *obj = strtok(inv, ",");
      while (obj) {
        /* Se usa realloc para extender dinamicamente la lista de Objetos */
        Objetos *tmp = realloc(jug.objetos, (jug.num_objetos + 1) * sizeof(Objetos));
        if (!tmp)
          break;
        jug.objetos = tmp;
        memset(&(jug.objetos[jug.num_objetos]), 0, sizeof(Objetos));
        CPY(jug.objetos[jug.num_objetos].id_objeto, obj);
        jug.num_objetos++;
        obj = strtok(NULL, ",");
      }
    }
    hallado = 1;
    break;
  }
  fclose(f);
  if (!hallado)
    return 0;

  /* 2. Inicializar la partida con el estado base de objetos, conexiones y puzles:
   *    Se cargan todos los arrays desde sus ficheros de datos originales. */
  memset(par, 0, sizeof(Partida));
  CPY(par->id_jugador, jug.id_jugador);
  par->id_sala_actual = 1; /* El id 0 se utiliza para el inventario, el inicio es 1 */

  /* Carga el estado base de objetos desde objetos.txt */
  Objetos *base_objs = NULL;
  int num_base = leer_objetos(&base_objs);
  if (num_base > 0) {
    par->lista_objetos = base_objs;
    par->num_objetos = num_base;
  }

  /* Carga el estado base de conexiones desde conexiones.txt */
  Conexiones *base_conex = NULL;
  int num_conex = leer_conexiones(&base_conex);
  if (num_conex > 0) {
    par->lista_conexiones = base_conex;
    par->num_conexiones = num_conex;
  }

  /* Carga el estado base de puzles desde puzles.txt */
  Puzles *base_puzles = NULL;
  int num_puzles_base = leer_puzles(&base_puzles);
  if (num_puzles_base > 0) {
    par->lista_puzles = base_puzles;
    par->num_puzles = num_puzles_base;
  }

  /* Aplica el inventario del jugador desde jugadores.txt como estado inicial de los objetos:
   * los objetos en su bolsa se marcan con localizacion 0 antes de que partida.txt los sobreescriba */
  for (int i = 0; i < jug.num_objetos; i++) {
    for (int j = 0; j < par->num_objetos; j++) {
      if (!strcmp(par->lista_objetos[j].id_objeto, jug.objetos[i].id_objeto)) {
        par->lista_objetos[j].localizacion_objeto = 0; /* Lo marcamos como inventario */
        break;
      }
    }
  }
  free(jug.objetos);

  /* 3. Sobreescribir con el progreso guardado en partida.txt:
   *    Si existe un bloque para este jugador, sus datos sobreescriben el estado base
   *    campo a campo: ubicacion de objetos, estado de conexiones y estado de puzles. */
  if ((f = fopen("ficheros/partida.txt", "r")) == NULL)
    return 1;
  int en_bloque = 0;
  while (fgets(line, sizeof(line), f)) {
    line[strcspn(line, "\r\n")] = '\0';
    if (line[0] == '/' || line[0] == '\0')
      continue;

    if (!strncmp(line, "JUGADOR:", 8) && strlen(line) > 9) {
      en_bloque = strcmp(line + 9, jug.id_jugador) == 0; /* Activa la lectura solo si es el bloque del jugador */
      continue;
    }
    if (!en_bloque)
      continue;

    if (!strncmp(line, "SALA:", 5)) {
      par->id_sala_actual = atoi(line + 6);
      continue;
    }
    if (!strncmp(line, "OBJETO:", 7)) {
      /* Sobreescribe la ubicacion del objeto en lista_objetos segun el id guardado */
      char obj_copy[512];
      strncpy(obj_copy, line + 8, sizeof(obj_copy) - 1);
      obj_copy[sizeof(obj_copy) - 1] = '\0';
      char *id = strtok(obj_copy, "-"), *loc_str = strtok(NULL, "-");
      if (id && loc_str) {
        int loc = atoi(loc_str);
        for (int i = 0; i < par->num_objetos; i++) {
          if (!strcmp(par->lista_objetos[i].id_objeto, id)) {
            par->lista_objetos[i].localizacion_objeto = loc;
            break;
          }
        }
      }
      continue;
    }
    if (!strncmp(line, "CONEXION:", 9)) {
      /* Sobreescribe el estado de la conexion en lista_conexiones segun el id guardado */
      char conexion_copy[512];
      strncpy(conexion_copy, line + 10, sizeof(conexion_copy) - 1);
      conexion_copy[sizeof(conexion_copy) - 1] = '\0';
      char *id = strtok(conexion_copy, "-"), *est = strtok(NULL, "-");
      if (!id) continue;
      for (int i = 0; i < par->num_conexiones; i++) {
        if (!strcmp(par->lista_conexiones[i].id_conexion, id)) {
          par->lista_conexiones[i].estado_conexion = (est && !strcmp(est, "Activa")) ? 1 : 0;
          break;
        }
      }
      continue;
    }
    if (!strncmp(line, "PUZZLE:", 7)) {
      /* Sobreescribe el estado del puzle en lista_puzles segun el id guardado */
      char puzzle_copy[512];
      strncpy(puzzle_copy, line + 8, sizeof(puzzle_copy) - 1);
      puzzle_copy[sizeof(puzzle_copy) - 1] = '\0';
      char *id = strtok(puzzle_copy, "-"), *est = strtok(NULL, "-");
      if (!id) continue;
      for (int i = 0; i < par->num_puzles; i++) {
        if (!strcmp(par->lista_puzles[i].id_puzle, id)) {
          par->lista_puzles[i].resuelto = (est && !strcmp(est, "Resuelto")) ? 1 : 0;
          break;
        }
      }
      continue;
    }
  }
  fclose(f);
  return 1;
}

void guardarPartida(Partida *par) {
  FILE *fin, *fout;
  char line[512];

  /* 1. Actualizacion de partida.txt:
   *    En lugar de modificar en linea, se copia todo el contenido original excepto
   *    las entradas del jugador en cuestion, que se reescriben al final del nuevo fichero tmp. */
  if ((fout = fopen("ficheros/partida_tmp.txt", "w")) != NULL) {
    int skip = 0;
    if ((fin = fopen("ficheros/partida.txt", "r")) != NULL) {
      while (fgets(line, sizeof(line), fin)) {
        char t[512];
        strncpy(t, line, sizeof(t));
        t[strcspn(t, "\r\n")] = '\0';
        if (!strncmp(t, "JUGADOR:", 8) && strlen(t) > 9)
          skip = strcmp(t + 9, par->id_jugador) == 0; /* Omite las lineas del jugador actual en el guardado original (se van a reescribir) */
        if (!skip)
          fputs(line,fout); /* Mantiene intacta la partida de los demas jugadores */
      }
      fclose(fin);
    }
    /* Escribe todos los datos del estado actual del jugador: sala, objetos, conexiones y puzles */
    fprintf(fout, "JUGADOR: %s\nSALA: %02d\n", par->id_jugador, par->id_sala_actual);
    /* Guarda todos los objetos con su ubicacion actual (0 = inventario del jugador) */
    for (int i = 0; i < par->num_objetos; i++)
      fprintf(fout, "OBJETO: %s-%02d\n", par->lista_objetos[i].id_objeto, par->lista_objetos[i].localizacion_objeto);
    /* Guarda todas las conexiones con su estado actual */
    for (int i = 0; i < par->num_conexiones; i++)
      fprintf(fout, "CONEXION: %s-%s\n", par->lista_conexiones[i].id_conexion, par->lista_conexiones[i].estado_conexion ? "Activa" : "Bloqueada");
    /* Guarda todos los puzles con su estado actual */
    for (int i = 0; i < par->num_puzles; i++)
      fprintf(fout, "PUZZLE: %s-%s\n", par->lista_puzles[i].id_puzle, par->lista_puzles[i].resuelto ? "Resuelto" : "Pendiente");
    fclose(fout);
    reemplazar("ficheros/partida.txt", "ficheros/partida_tmp.txt");
  }

  /* 2. Actualizacion de jugadores.txt:
   *    Solo se modifica el campo inventario del jugador actual;
   *    el resto de campos y los demas jugadores permanecen intactos. */
  fin = fopen("ficheros/jugadores.txt", "r");
  fout = fopen("ficheros/jugadores_tmp.txt", "w");
  if (fin && fout) {
    while (fgets(line, sizeof(line), fin)) {
      if (line[0] == '/' && line[1] == '/') { fputs(line, fout); continue; }
      char t[512];
      strncpy(t, line, sizeof(t) - 1);
      t[sizeof(t) - 1] = '\0';
      t[strcspn(t, "\r\n")] = '\0';
      char t_copy[512];
      strncpy(t_copy, t, sizeof(t_copy) - 1);
      t_copy[sizeof(t_copy) - 1] = '\0';
      char *id = strtok(t_copy, "-"), *nom = strtok(NULL, "-"), *nick = strtok(NULL, "-"), *pw = strtok(NULL, "-");
      if (!id || !nom || !nick || !pw || strcmp(id, par->id_jugador) != 0) {
        fputs(line, fout); /* Mantiene intacto el resto de jugadores */
        continue;
      }
      /* Reescribe la linea del jugador actual con el inventario actualizado */
      fprintf(fout, "%s-%s-%s-%s", id, nom, nick, pw);
      int cnt = 0;
      for (int i = 0; i < par->num_objetos; i++)
        if (par->lista_objetos[i].localizacion_objeto == 0)
          fprintf(fout, "%s%s", cnt++ ? "," : "-", par->lista_objetos[i].id_objeto);
      fprintf(fout, "\n");
    }
  }
  if (fin) fclose(fin);
  if (fout) fclose(fout);
  if (fin && fout)
    reemplazar("ficheros/jugadores.txt", "ficheros/jugadores_tmp.txt");
  else
    remove("ficheros/jugadores_tmp.txt");
}

void borrarPartida(char *id_jugador) {
  FILE *fin, *fout;
  char line[512];

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
}

// Crea una nueva partida.
//
// Inicializa la estructura partida para el jugador indicado.
//
// jugador (E)  Jugador que ha inciado sesión
// partida (S)  Partida nueva sin inicializar.

void nueva_partida(Jugadores *jugador, Partida *partida) {
  strcpy(partida->id_jugador, jugador->id_jugador);
  partida->id_sala_actual = 0;
  partida->lista_objetos = NULL;
  partida->num_objetos = 0;
  partida->lista_conexiones = NULL;
  partida->num_conexiones = 0;
  partida->lista_puzles = NULL;
  partida->num_puzles = 0;
}