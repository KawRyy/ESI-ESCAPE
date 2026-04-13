#include "ficheros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estructuras.h"
#include "gestion.h"

#define CPY(d, s) (strncpy((d), (s), sizeof(d) - 1), (d)[sizeof(d) - 1] = '\0')

static void reemplazar(const char *orig, const char *tmp) {
  remove(orig);
  rename(tmp, orig);
}

/* ── leer_salas ─────────────────────────────────────────────────── */
static int leer_salas(Salas **salas) {
  const int MAX_SALAS = 10; /* Limite maximo de salas a cargar en memoria */
  FILE *f = fopen("ficheros/salas.txt", "r");
  char line[512];
  int n = 0;
  *salas = malloc(MAX_SALAS * sizeof(Salas)); /* Reserva memoria para el array de salas */
  if (!*salas || !f) {
    if (f)
      fclose(f);
    return -1;
  }
  memset(*salas, 0, MAX_SALAS * sizeof(Salas)); /* Inicializa la memoria con ceros */
  while (fgets(line, sizeof(line), f) && n < MAX_SALAS) {
    line[strcspn(line, "\r\n")] = '\0'; /* Elimina saltos de linea */
    if (line[0] == '/' || line[0] == '\0')
      continue; /* Ignora lineas vacias o comentarios */
    char *id = strtok(line, "-"), *nom = strtok(NULL, "-"), *tipo = strtok(NULL, "-"), *desc = strtok(NULL, "-"); /* Separa campos por '-' */
    if (!id || !nom || !tipo || !desc)
      continue;
    (*salas)[n].id_sala = atoi(id); /* Convierte el ID a numero entero */
    CPY((*salas)[n].nombre_sala, nom);
    (*salas)[n].tipo_sala = !strcmp(tipo, "INICIAL")  ? 1 : !strcmp(tipo, "NORMAL") ? 2 : !strcmp(tipo, "SALIDA") ? 3 : 0; /* Asigna el valor numerico segun el tipo */
    CPY((*salas)[n].descripcion_sala, desc);
    n++;
  }
  fclose(f);
  return n;
}

/* ── leer_puzles ────────────────────────────────────────────────── */
int leer_puzles(Puzles **puzles) {
  const int MAX_PUZLES = 5; /* Limite maximo de puzles a cargar en memoria */
  FILE *f = fopen("ficheros/puzles.txt", "r");
  char line[512];
  int n = 0;
  *puzles = malloc(MAX_PUZLES * sizeof(Puzles)); /* Reserva memoria para array de puzles */
  if (!*puzles || !f) {
    if (f)
      fclose(f);
    return -1;
  }
  memset(*puzles, 0, MAX_PUZLES * sizeof(Puzles)); /* Inicializa la memoria con ceros */
  while (fgets(line, sizeof(line), f) && n < MAX_PUZLES) {
    line[strcspn(line, "\r\n")] = '\0'; /* Elimina saltos de linea */
    if (line[0] == '/' || line[0] == '\0')
      continue; /* Ignora lineas vacias o comentarios */
    char *id = strtok(line, "-");
    strtok(NULL, "-"); /* skip nombre */
    char *sala = strtok(NULL, "-"), *desc = strtok(NULL, "-"), *sol = strtok(NULL, "-"); /* Separa campos por '-' */
    if (!id || !sala || !desc || !sol)
      continue;
    CPY((*puzles)[n].id_puzle, id);
    (*puzles)[n].id_sala_puzle = atoi(sala); /* Convierte el ID de sala a entero */
    (*puzles)[n].tipo_elemento = !strncmp(sol, "OB", 2) ? 1 : 2; /* Determina el tipo de elemento */
    CPY((*puzles)[n].descripcion_puzle, desc);
    CPY((*puzles)[n].solucion_puzle, sol);
    n++;
  }
  fclose(f);
  return n;
}

/* ── leer_conexiones ────────────────────────────────────────────── */
int leer_conexiones(Conexiones **conexiones) {
  const int MAX_CONEXIONES = 9; /* Limite maximo de conexiones a cargar en memoria */
  FILE *f = fopen("ficheros/conexiones.txt", "r");
  char line[512];
  int n = 0;
  *conexiones = malloc(MAX_CONEXIONES * sizeof(Conexiones)); /* Reserva memoria para array de conexiones */
  if (!*conexiones || !f) {
    if (f)
      fclose(f);
    return -1;
  }
  memset(*conexiones, 0, MAX_CONEXIONES * sizeof(Conexiones)); /* Inicializa la memoria con ceros */
  while (fgets(line, sizeof(line), f) && n < MAX_CONEXIONES) {
    line[strcspn(line, "\r\n")] = '\0'; /* Elimina saltos de linea */
    if (line[0] == '/' || line[0] == '\0')
      continue; /* Ignora lineas vacias o comentarios */
    char *id = strtok(line, "-"), *orig = strtok(NULL, "-"), *dest = strtok(NULL, "-"), *estado = strtok(NULL, "-"), *cond = strtok(NULL, "-"); /* Separa campos por '-' */
    if (!id || !orig || !dest || !estado || !cond)
      continue;
    CPY((*conexiones)[n].id_conexion, id);
    (*conexiones)[n].id_sala_orig = atoi(orig); /* Convierte el ID origen a entero */
    (*conexiones)[n].id_sala_dest = atoi(dest); /* Convierte el ID destino a entero */
    (*conexiones)[n].estado_conexion = !strcmp(estado, "Activa") ? 1 : 0; /* Determina si la conexion esta activa */
    (*conexiones)[n].condicion_conexion = !strcmp(cond, "0") ? 0 : !strncmp(cond, "OB", 2) ? 1 : !strncmp(cond, "P", 1)  ? 2 : 0; /* Asigna numericamente la condicion */
    n++;
  }
  fclose(f);
  return n;
}

/* ── leer_objetos ───────────────────────────────────────────────── */
int leer_objetos(Objetos **objetos) {
  const int MAX_OBJETOS = 5; /* Limite maximo de objetos a cargar en memoria */
  FILE *f = fopen("ficheros/objetos.txt", "r");
  char line[512];
  int n = 0;
  *objetos = malloc(MAX_OBJETOS * sizeof(Objetos)); /* Reserva memoria para array de objetos */
  if (!*objetos || !f) {
    if (f)
      fclose(f);
    return -1;
  }
  memset(*objetos, 0, MAX_OBJETOS * sizeof(Objetos)); /* Inicializa la memoria con ceros */
  while (fgets(line, sizeof(line), f) && n < MAX_OBJETOS) {
    line[strcspn(line, "\r\n")] = '\0'; /* Elimina saltos de linea */
    if (line[0] == '/' || line[0] == '\0')
      continue; /* Ignora lineas vacias o comentarios */
    char *id = strtok(line, "-"), *nom = strtok(NULL, "-"), *desc = strtok(NULL, "-"), *loc = strtok(NULL, "-"); /* Separa campos por '-' */
    if (!id || !nom || !desc || !loc)
      continue;
    CPY((*objetos)[n].id_objeto, id);
    CPY((*objetos)[n].nombre_objeto, nom);
    CPY((*objetos)[n].descripcion_objeto, desc);
    (*objetos)[n].localizacion_objeto = atoi(loc); /* ID de sala o ubicacion */
    n++;
  }
  fclose(f);
  return n;
}

/* ── leer_jugadores ─────────────────────────────────────────────── */
static int leer_jugadores(Jugadores **jugadores) {
  FILE *f = fopen("ficheros/jugadores.txt", "r");
  char line[512];
  int n = 0;
  *jugadores = NULL;
  if (!f) return -1;

  while (fgets(line, sizeof(line), f)) {
    line[strcspn(line, "\r\n")] = '\0'; /* Elimina saltos de linea */
    if (line[0] == '/' || line[0] == '\0')
      continue; /* Ignora lineas vacias o comentarios */
    char *id = strtok(line, "-"), *nom = strtok(NULL, "-"), *nick = strtok(NULL, "-"), *pw = strtok(NULL, "-"), *inv = strtok(NULL, "-");
    if (!id || !nom || !nick || !pw)
      continue;

    Jugadores *tmp = realloc(*jugadores, (n + 1) * sizeof(Jugadores));
    if (!tmp) {
        fclose(f);
        return -1;
    }
    *jugadores = tmp;
    memset(&((*jugadores)[n]), 0, sizeof(Jugadores));
    
    CPY((*jugadores)[n].id_jugador, id);
    CPY((*jugadores)[n].nombre_jugador, nom);
    CPY((*jugadores)[n].nickname, nick);
    CPY((*jugadores)[n].contrasena, pw);
    if (inv) {
      char *obj = strtok(inv, ",");
      while (obj) {
        Objetos *tmp_obj = realloc((*jugadores)[n].objetos, ((*jugadores)[n].num_objetos + 1) * sizeof(Objetos));
        if (!tmp_obj) break;
        (*jugadores)[n].objetos = tmp_obj;
        memset(&((*jugadores)[n].objetos[(*jugadores)[n].num_objetos]), 0, sizeof(Objetos));
        CPY((*jugadores)[n].objetos[(*jugadores)[n].num_objetos].id_objeto, obj);
        (*jugadores)[n].num_objetos++;
        obj = strtok(NULL, ",");
      }
    }
    n++;
  }
  fclose(f);
  return n;
}

/* ── volcado ────────────────────────────────────────────────────── */
int volcado(Salas **s, Conexiones **c, Puzles **p, Objetos **o, Jugadores **j, int* num_jugadores) {

  int ns = leer_salas(s);
  int np = leer_puzles(p);
  int nc = leer_conexiones(c);
  int no = leer_objetos(o);
  int nj = leer_jugadores(j);
  *num_jugadores = nj;

  return (ns >= 0 && np >= 0 && nc >= 0 && no >= 0 && nj >= 0) ? 1 : 0; /* Verifica si todas las lecturas fueron exitosas */
}