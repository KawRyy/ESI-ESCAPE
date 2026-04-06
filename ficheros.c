#include "ficheros.h"
#include "estructuras.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Añadir mas comentarios en cargarPartida y guardarPartida.

#define MAX_SALAS 10
#define MAX_CONEXIONES 9
#define MAX_OBJETOS 5
#define MAX_PUZLES 5

#define CPY(d, s) (strncpy((d), (s), sizeof(d) - 1), (d)[sizeof(d) - 1] = '\0')

static void reemplazar(const char *orig, const char *tmp) {
  remove(orig);
  rename(tmp, orig);
}

/* ── leer_salas ─────────────────────────────────────────────────── */
static int leer_salas(Salas **out) {
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

  /* 1. Buscar jugador en jugadores.txt */
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
      char *obj = strtok(inv, ",");
      while (obj) {
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

  /* 2. Inicializar partida con inventario del jugador */
  memset(par, 0, sizeof(Partida));  /* Limpia la estructura de partida */
  par->id_jugador = jug.id_jugador; /* Asigna el id del jugador cargado */
  par->id_sala_actual = 1; /* El id 0 se utiliza para el inventario, el inicio es 1 */
  if (jug.num_items > 0) {
    par->lista_objetos = malloc(jug.num_items * sizeof(Objetos)); /* Reserva memoria para inventario */
    if (par->lista_objetos) {
      memset(par->lista_objetos, 0, jug.num_items * sizeof(Objetos));
      for (int i = 0; i < jug.num_items; i++) {
        CPY(par->lista_objetos[i].id_objeto, jug.id_objeto[i]);
        par->lista_objetos[i].localizacion_objeto = 0; /* 0 = en inventario */
      }
      par->num_objetos = jug.num_items;
    }
  }
  for (int i = 0; i < jug.num_items; i++)
    free(jug.id_objeto[i]);
  free(jug.id_objeto);

  /* 3. Cargar progreso desde partida.txt */
  if ((f = fopen("ficheros/partida.txt", "r")) == NULL)
    return 1;
  int en_bloque = 0;
  while (fgets(line, sizeof(line), f)) {
    line[strcspn(line, "\r\n")] = '\0';
    if (line[0] == '/' || line[0] == '\0')
      continue;

    if (!strncmp(line, "JUGADOR:", 8)) {
      en_bloque = atoi(line + 9) == jug.id_jugador; /* Activa la lectura solo si es el bloque de la partida del jugador */
      continue;
    }
    if (!en_bloque)
      continue;
    if (!strncmp(line, "SALA:", 5)) {
      par->id_sala_actual = atoi(line + 6);
      continue;
    }

    if (!strncmp(line, "OBJETO:", 7)) {
      /* Ignoramos OBJETO de partida.txt para evitar inventario duplicado (ahora usa solo jugadores.txt) */
      continue;
    }
    if (!strncmp(line, "CONEXION:", 9)) {
      char *id = strtok(line + 10, "-"), *est = strtok(NULL, "-");
      Conexiones *tmp = realloc(par->lista_conexiones, (par->num_conexiones + 1) * sizeof(Conexiones));
      if (!tmp) {
        fclose(f);
        return 0;
      }
      par->lista_conexiones = tmp;
      memset(&par->lista_conexiones[par->num_conexiones], 0, sizeof(Conexiones));
      if (id)
        CPY(par->lista_conexiones[par->num_conexiones].id_conexion, id);
      par->lista_conexiones[par->num_conexiones].estado_conexion = (est && !strcmp(est, "Activa")) ? 1 : 0;
      par->num_conexiones++;
      continue;
    }
    if (!strncmp(line, "PUZZLE:", 7)) {
      char *id = strtok(line + 8, "-");
      Puzles *tmp = realloc(par->lista_puzles, (par->num_puzles + 1) * sizeof(Puzles));
      if (!tmp) {
        fclose(f);
        return 0;
      }
      par->lista_puzles = tmp;
      memset(&par->lista_puzles[par->num_puzles], 0, sizeof(Puzles));
      if (id)
        CPY(par->lista_puzles[par->num_puzles].id_puzle, id);
      par->num_puzles++;
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

  /* 1. partida.txt */
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
    fprintf(fout, "JUGADOR: %02d\nSALA: %02d\n", par->id_jugador, par->id_sala_actual);
    /* Se omite guardar OBJETO: aquí para no duplicar datos (el inventario se aloja en jugadores.txt) */
    for (int i = 0; i < par->num_conexiones; i++)
      fprintf(fout, "CONEXION: %s-%s\n", par->lista_conexiones[i].id_conexion,par->lista_conexiones[i].estado_conexion ? "Activa" : "Bloqueada");
    for (int i = 0; i < par->num_puzles; i++)
      fprintf(fout, "PUZZLE: %s-Resuelto\n", par->lista_puzles[i].id_puzle);
    fclose(fout);
    reemplazar("ficheros/partida.txt", "ficheros/partida_tmp.txt");
  }

  /* 2. conexiones.txt */
  fin = fopen("ficheros/conexiones.txt", "r");
  fout = fopen("ficheros/conexiones_tmp.txt", "w");
  if (fin && fout) {
    while (fgets(line, sizeof(line), fin)) {
      if (line[0] == '/' && line[1] == '/') {
        fputs(line, fout);
        continue;
      }
      char t[512];
      strncpy(t, line, sizeof(t));
      t[strcspn(t, "\r\n")] = '\0';
      char *id = strtok(t, "-"), *orig = strtok(NULL, "-"),*dest = strtok(NULL, "-"), *state = strtok(NULL, "-"),*cond = strtok(NULL, "-");
      if (!id || !orig || !dest || !state || !cond) {
        fputs(line, fout);
        continue;
      }
      const char *nuevo = state;
      for (int i = 0; i < par->num_conexiones; i++)
        if (!strcmp(par->lista_conexiones[i].id_conexion, id)) {
          nuevo = par->lista_conexiones[i].estado_conexion ? "Activa" : "Bloqueada";
          break;
        }
      fprintf(fout, "%s-%s-%s-%s-%s\n", id, orig, dest, nuevo, cond);
    }
  }
  if (fin)
    fclose(fin);
  if (fout)
    fclose(fout);
  if (fin && fout)
    reemplazar("ficheros/conexiones.txt", "ficheros/conexiones_tmp.txt");
  else
    remove("ficheros/conexiones_tmp.txt");

  /* 3. jugadores.txt */
  fin = fopen("ficheros/jugadores.txt", "r");
  fout = fopen("ficheros/jugadores_tmp.txt", "w");
  if (fin && fout) {
    while (fgets(line, sizeof(line), fin)) {
      if (line[0] == '/' && line[1] == '/') {
        fputs(line, fout);
        continue;
      }
      char t[512];
      strncpy(t, line, sizeof(t));
      t[strcspn(t, "\r\n")] = '\0';
      char *id = strtok(t, "-"), *nom = strtok(NULL, "-"), *nick = strtok(NULL, "-"), *pw = strtok(NULL, "-");
      if (!id || !nom || !nick || !pw || atoi(id) != par->id_jugador) {
        fputs(line, fout);
        continue;
      }
      fprintf(fout, "%s-%s-%s-%s", id, nom, nick, pw);
      int cnt = 0;
      /* Utilizamos siempre la lista de objetos de la Partida filtrando el inventario activo */
      for (int i = 0; i < par->num_objetos; i++) {
        if (par->lista_objetos[i].localizacion_objeto == 0) {
          fprintf(fout, "%s%s", cnt++ ? "," : "-", par->lista_objetos[i].id_objeto);
        }
      }
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