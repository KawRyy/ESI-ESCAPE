#include "ficheros.h"
#include "estructuras.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define CPY(d, s) (strncpy((d), (s), sizeof(d) - 1), (d)[sizeof(d) - 1] = '\0')

static void reemplazar(const char *orig, const char *tmp) {
  remove(orig);
  rename(tmp, orig);
}

/* ── leer_salas ─────────────────────────────────────────────────── */
static int leer_salas(Salas **out) {
  const int MAX_SALAS = 10; /* Limite maximo de salas a cargar en memoria */
  FILE *f = fopen("ficheros/salas.txt", "r");
  char line[512];
  int n = 0;
  *out = malloc(MAX_SALAS * sizeof(Salas)); /* Reserva memoria para el array de salas */
  if (!*out || !f) {
    if (f)
      fclose(f);
    return -1;
  }
  memset(*out, 0, MAX_SALAS * sizeof(Salas)); /* Inicializa la memoria con ceros */
  while (fgets(line, sizeof(line), f) && n < MAX_SALAS) {
    line[strcspn(line, "\r\n")] = '\0'; /* Elimina saltos de linea */
    if (line[0] == '/' || line[0] == '\0')
      continue; /* Ignora lineas vacias o comentarios */
    char *id = strtok(line, "-"), *nom = strtok(NULL, "-"), *tipo = strtok(NULL, "-"), *desc = strtok(NULL, "-"); /* Separa campos por '-' */
    if (!id || !nom || !tipo || !desc)
      continue;
    (*out)[n].id_sala = atoi(id); /* Convierte el ID a numero entero */
    CPY((*out)[n].nombre_sala, nom);
    (*out)[n].tipo_sala = !strcmp(tipo, "INICIAL")  ? 1 : !strcmp(tipo, "NORMAL") ? 2 : !strcmp(tipo, "SALIDA") ? 3 : 0; /* Asigna el valor numerico segun el tipo */
    CPY((*out)[n].descripcion_sala, desc);
    n++;
  }
  fclose(f);
  return n;
}

/* ── leer_puzles ────────────────────────────────────────────────── */
static int leer_puzles(Puzles **out) {
  const int MAX_PUZLES = 5; /* Limite maximo de puzles a cargar en memoria */
  FILE *f = fopen("ficheros/puzles.txt", "r");
  char line[512];
  int n = 0;
  *out = malloc(MAX_PUZLES * sizeof(Puzles)); /* Reserva memoria para array de puzles */
  if (!*out || !f) {
    if (f)
      fclose(f);
    return -1;
  }
  memset(*out, 0, MAX_PUZLES * sizeof(Puzles)); /* Inicializa la memoria con ceros */
  while (fgets(line, sizeof(line), f) && n < MAX_PUZLES) {
    line[strcspn(line, "\r\n")] = '\0'; /* Elimina saltos de linea */
    if (line[0] == '/' || line[0] == '\0')
      continue; /* Ignora lineas vacias o comentarios */
    char *id = strtok(line, "-");
    strtok(NULL, "-"); /* skip nombre */
    char *sala = strtok(NULL, "-"), *desc = strtok(NULL, "-"), *sol = strtok(NULL, "-"); /* Separa campos por '-' */
    if (!id || !sala || !desc || !sol)
      continue;
    CPY((*out)[n].id_puzle, id);
    (*out)[n].id_sala_puzle = atoi(sala); /* Convierte el ID de sala a entero */
    (*out)[n].tipo_elemento = !strncmp(sol, "OB", 2) ? 1 : 2; /* Determina el tipo de elemento */
    CPY((*out)[n].descripcion_puzle, desc);
    CPY((*out)[n].solucion_puzle, sol);
    n++;
  }
  fclose(f);
  return n;
}

/* ── leer_conexiones ────────────────────────────────────────────── */
static int leer_conexiones(Conexiones **out) {
  const int MAX_CONEXIONES = 9; /* Limite maximo de conexiones a cargar en memoria */
  FILE *f = fopen("ficheros/conexiones.txt", "r");
  char line[512];
  int n = 0;
  *out = malloc(MAX_CONEXIONES * sizeof(Conexiones)); /* Reserva memoria para array de conexiones */
  if (!*out || !f) {
    if (f)
      fclose(f);
    return -1;
  }
  memset(*out, 0, MAX_CONEXIONES * sizeof(Conexiones)); /* Inicializa la memoria con ceros */
  while (fgets(line, sizeof(line), f) && n < MAX_CONEXIONES) {
    line[strcspn(line, "\r\n")] = '\0'; /* Elimina saltos de linea */
    if (line[0] == '/' || line[0] == '\0')
      continue; /* Ignora lineas vacias o comentarios */
    char *id = strtok(line, "-"), *orig = strtok(NULL, "-"), *dest = strtok(NULL, "-"), *state = strtok(NULL, "-"), *cond = strtok(NULL, "-"); /* Separa campos por '-' */
    if (!id || !orig || !dest || !state || !cond)
      continue;
    CPY((*out)[n].id_conexion, id);
    (*out)[n].id_sala_orig = atoi(orig); /* Convierte el ID origen a entero */
    (*out)[n].id_sala_dest = atoi(dest); /* Convierte el ID destino a entero */
    (*out)[n].estado_conexion = !strcmp(state, "Activa") ? 1 : 0; /* Determina si la conexion esta activa */
    (*out)[n].condicion_conexion = !strcmp(cond, "0") ? 0 : !strncmp(cond, "OB", 2) ? 1 : !strncmp(cond, "P", 1)  ? 2 : 0; /* Asigna numericamente la condicion */
    n++;
  }
  fclose(f);
  return n;
}

/* ── leer_objetos ───────────────────────────────────────────────── */
static int leer_objetos(Objetos **out) {
  const int MAX_OBJETOS = 5; /* Limite maximo de objetos a cargar en memoria */
  FILE *f = fopen("ficheros/objetos.txt", "r");
  char line[512];
  int n = 0;
  *out = malloc(MAX_OBJETOS * sizeof(Objetos)); /* Reserva memoria para array de objetos */
  if (!*out || !f) {
    if (f)
      fclose(f);
    return -1;
  }
  memset(*out, 0, MAX_OBJETOS * sizeof(Objetos)); /* Inicializa la memoria con ceros */
  while (fgets(line, sizeof(line), f) && n < MAX_OBJETOS) {
    line[strcspn(line, "\r\n")] = '\0'; /* Elimina saltos de linea */
    if (line[0] == '/' || line[0] == '\0')
      continue; /* Ignora lineas vacias o comentarios */
    char *id = strtok(line, "-"), *nom = strtok(NULL, "-"), *desc = strtok(NULL, "-"), *loc = strtok(NULL, "-"); /* Separa campos por '-' */
    if (!id || !nom || !desc || !loc)
      continue;
    CPY((*out)[n].id_objeto, id);
    CPY((*out)[n].nombre_objeto, nom);
    CPY((*out)[n].descripcion_objeto, desc);
    (*out)[n].localizacion_objeto = atoi(loc); /* ID de sala o ubicacion */
    n++;
  }
  fclose(f);
  return n;
}

/* ── leer_jugadores ─────────────────────────────────────────────── */
static int leer_jugadores(Jugadores **out) {
  FILE *f = fopen("ficheros/jugadores.txt", "r");
  char line[512];
  int n = 0;
  *out = NULL;
  if (!f) return -1;

  while (fgets(line, sizeof(line), f)) {
    line[strcspn(line, "\r\n")] = '\0'; /* Elimina saltos de linea */
    if (line[0] == '/' || line[0] == '\0')
      continue; /* Ignora lineas vacias o comentarios */
    char *id = strtok(line, "-"), *nom = strtok(NULL, "-"), *nick = strtok(NULL, "-"), *pw = strtok(NULL, "-"), *inv = strtok(NULL, "-");
    if (!id || !nom || !nick || !pw)
      continue;

    Jugadores *tmp = realloc(*out, (n + 1) * sizeof(Jugadores));
    if (!tmp) {
        fclose(f);
        return -1;
    }
    *out = tmp;
    memset(&((*out)[n]), 0, sizeof(Jugadores));

    (*out)[n].id_jugador = atoi(id);
    CPY((*out)[n].nombre_jugador, nom);
    CPY((*out)[n].jugador, nick);
    CPY((*out)[n].contraseña, pw);
    if (inv) {
      char *obj = strtok(inv, ",");
      while (obj) {
        char **tmp_obj = realloc((*out)[n].id_objeto, ((*out)[n].num_items + 1) * sizeof(char *));
        if (!tmp_obj) break;
        (*out)[n].id_objeto = tmp_obj;
        (*out)[n].id_objeto[(*out)[n].num_items] = malloc(strlen(obj) + 1);
        if ((*out)[n].id_objeto[(*out)[n].num_items])
          strcpy((*out)[n].id_objeto[(*out)[n].num_items++], obj);
        obj = strtok(NULL, ",");
      }
    }
    n++;
  }
  fclose(f);
  return n;
}

/* ── volcado ────────────────────────────────────────────────────── */
int volcado(Salas **s, Conexiones **c, Puzles **p, Objetos **o, Jugadores **j) {

  int ns = leer_salas(s);
  int np = leer_puzles(p);
  int nc = leer_conexiones(c);
  int no = leer_objetos(o);
  int nj = leer_jugadores(j);

  return (ns >= 0 && np >= 0 && nc >= 0 && no >= 0 && nj >= 0) ? 1 : 0; /* Verifica si todas las lecturas fueron exitosas */
}

/* ── cargarPartida ──────────────────────────────────────────────── */
int cargarPartida(Partida *par, char *nickname) {
  FILE *f;
  char line[512];

  /* 1. Buscar jugador en jugadores.txt:
   *    Se abre el fichero y se busca linea a linea un usuario que coincida
   *    con el 'nickname' proporcionado como parametro. */
  Jugadores jug;
  memset(&jug, 0, sizeof(Jugadores));
  int hallado = 0;
  if ((f = fopen("ficheros/jugadores.txt", "r")) == NULL)
    return 0;
  while (fgets(line, sizeof(line), f)) {
    line[strcspn(line, "\r\n")] = '\0';
    if (line[0] == '/' || line[0] == '\0')
      continue;
    char *id = strtok(line, "-"), *nom = strtok(NULL, "-"), *nick = strtok(NULL, "-"), *pw = strtok(NULL, "-"), *inv = strtok(NULL, "-"); /* Extrae campos separados por '-' */
    if (!id || !nom || !nick || strcmp(nick, nickname) != 0) /* Ignora si faltan datos o no coinciden con el usuario */
      continue;
    jug.id_jugador = atoi(id);
    CPY(jug.nombre_jugador, nom);
    CPY(jug.jugador, nick);
    if (pw)
      CPY(jug.contraseña, pw);
    if (inv) {
      /* Procesa los items en el inventario que vienen separados por comas */
      char *obj = strtok(inv, ",");
      while (obj) {
        /* Se usa realloc para extender dinamicamente la lista de punteros id_objeto */
        char **tmp = realloc(jug.id_objeto, (jug.num_items + 1) * sizeof(char *));
        if (!tmp)
          break;
        jug.id_objeto = tmp;
        jug.id_objeto[jug.num_items] = malloc(strlen(obj) + 1);
        if (jug.id_objeto[jug.num_items])
          strcpy(jug.id_objeto[jug.num_items++], obj);
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
  par->id_jugador = jug.id_jugador;
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
  for (int i = 0; i < jug.num_items; i++) {
    for (int j = 0; j < par->num_objetos; j++) {
      if (!strcmp(par->lista_objetos[j].id_objeto, jug.id_objeto[i])) {
        par->lista_objetos[j].localizacion_objeto = 0; /* Lo marcamos como inventario */
        break;
      }
    }
    free(jug.id_objeto[i]);
  }
  free(jug.id_objeto);

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

    if (!strncmp(line, "JUGADOR:", 8)) {
      en_bloque = atoi(line + 9) == jug.id_jugador; /* Activa la lectura solo si es el bloque del jugador */
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
      char *id = strtok(line + 8, "-"), *loc_str = strtok(NULL, "-");
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
      char *id = strtok(line + 10, "-"), *est = strtok(NULL, "-");
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
      char *id = strtok(line + 8, "-"), *est = strtok(NULL, "-");
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


/* ── guardarPartida ─────────────────────────────────────────────── */
void guardarPartida(Partida *par, Jugadores *jug) {
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
        if (!strncmp(t, "JUGADOR:", 8))
          skip = atoi(t + 9) == par->id_jugador; /* Omite las lineas del jugador actual en el guardado original (se van a reescribir) */
        if (!skip)
          fputs(line,fout); /* Mantiene intacta la partida de los demas jugadores */
      }
      fclose(fin);
    }
    /* Escribe todos los datos del estado actual del jugador: sala, objetos, conexiones y puzles */
    fprintf(fout, "JUGADOR: %02d\nSALA: %02d\n", par->id_jugador, par->id_sala_actual);
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
      strncpy(t, line, sizeof(t));
      t[strcspn(t, "\r\n")] = '\0';
      char *id = strtok(t, "-"), *nom = strtok(NULL, "-"), *nick = strtok(NULL, "-"), *pw = strtok(NULL, "-");
      if (!id || !nom || !nick || !pw || atoi(id) != par->id_jugador) {
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