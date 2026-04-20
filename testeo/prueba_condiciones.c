#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// PRUEBA PARA COMPROBAR ERRORES DE CONDICIONES.C
typedef struct {
  char id_objeto[5];
  char nombre_objeto[16];
  char descripcion_objeto[51];
  int localizacion_objeto; // 0: Inventario del jugador, si es distinto de 0
                           // indica el ID de la sala donde se encuentra el
                           // objeto
} Objetos;

typedef struct {
  char id_puzle[5];
  int id_sala_puzle; // Sala donde se encuentra
  int tipo_elemento; // Elemento que genera resolver el puzle, 1: CÓDIGO ; 2:
                     // PALABRA
  char descripcion_puzle[151];
  char solucion_puzle[51];
  int resuelto; // 0: PENDIENTE ; 1: RESUELTO
} Puzles;

typedef struct {
  char id_conexion[5];
  int id_sala_orig;
  int id_sala_dest;
  int estado_conexion;      // 0: CERRADA ; 1: ABIERTA
  char id_condicionante[5]; // ID del objeto o puzle que condiciona la conexión,
                            // si no tiene condición se pone "0"
} Conexiones;

typedef struct {
  Objetos *Inventario; // Puntero a vector dinámico de objetos (o NULL si no hay
                       // ninguno).
  int num_objetos; // Número de objetos.
} Inventario;

void ExaminarObjeto(Objetos *obj, int num_objetos, int id_sala_actual);
void MostrarInventario(Objetos *obj, Inventario *inv, int num_objetos);
void CogerObjeto(Inventario *inv, Objetos *obj, int num_objetos,
                 int id_sala_actual);
void SoltarObjeto(Inventario *inv, Objetos *obj, int num_objetos,
                  int id_sala_actual);
void UsarObjeto(Objetos *obj, Inventario *inv, int num_conexiones,
                Conexiones *con, int id_sala_actual);
void ResolverPuzle(int num_conexiones, Puzles *puz, int num_puzles,
                   Conexiones *con, int id_sala_actual);
int ComprobarConexion(Conexiones *con, int k, int id_sala_actual, char *n);
void AbrirConexion(Conexiones *con, int k);

int main() {
  Objetos *obj;
  Conexiones *con;
  Inventario inv;
  Puzles *puz;
  int id_sala_actual = 1; // El jugador empieza en la sala 1
  int num_objetos = 3;    // Hay 3 objetos en total
  int num_conexiones = 2; // Hay 2 conexiones en total
  int num_puzles = 2;     // Hay 2 puzles en total
  inv.num_objetos = 2;    // El jugador empieza sin objetos en el inventario
  obj = malloc(num_objetos * sizeof(Objetos)); // Reserva memoria para 3 objetos
  con = malloc(num_conexiones *
               sizeof(Conexiones)); // Reserva memoria para 2 conexiones
  puz = malloc(num_puzles * sizeof(Puzles)); // Reserva memoria para 2 puzles
  inv.Inventario = malloc(
      inv.num_objetos *
      sizeof(Objetos)); // Reserva memoria para 2 objetos en el inventario

  strcpy(obj[0].id_objeto, "OB01");
  strcpy(obj[0].nombre_objeto, "Objeto 1");
  strcpy(obj[0].descripcion_objeto, "Descripcion del objeto 1");
  obj[0].localizacion_objeto = 0; // El objeto 1 está en inventario

  strcpy(obj[1].id_objeto, "OB02");
  strcpy(obj[1].nombre_objeto, "Objeto 2");
  strcpy(obj[1].descripcion_objeto, "Descripcion del objeto 2");
  obj[1].localizacion_objeto = 0; // El objeto 2 está en inventario

  strcpy(obj[2].id_objeto, "OB03");
  strcpy(obj[2].nombre_objeto, "Objeto 3");
  strcpy(obj[2].descripcion_objeto, "Descripcion del objeto 3");
  obj[2].localizacion_objeto = 1; // El objeto 3 está en la sala 1

  inv.Inventario[0] = obj[0]; // El jugador tiene el objeto 1 en su inventario
  inv.Inventario[1] = obj[1]; // El jugador tiene el objeto 2 en su inventario

  strcpy(con[0].id_conexion, "C01");
  con[0].id_sala_orig = 1;
  con[0].id_sala_dest = 2;
  con[0].estado_conexion = 0; // La conexión está cerrada
  strcpy(con[0].id_condicionante,
         "OB01"); // La conexión se abre usando el objeto 1

  strcpy(con[1].id_conexion, "C02");
  con[1].id_sala_orig = 1;
  con[1].id_sala_dest = 3;
  con[1].estado_conexion = 0; // La conexión está cerrada
  strcpy(con[1].id_condicionante,
         "P01"); // La conexión se abre resolviendo el puzle 1

  strcpy(puz[0].id_puzle, "P01");
  puz[0].id_sala_puzle = 1;
  puz[0].tipo_elemento = 2; // El puzle genera un código al resolverlo
  strcpy(puz[0].descripcion_puzle, "Descripcion del puzle 1");
  strcpy(puz[0].solucion_puzle, "SOL01");
  puz[0].resuelto = 0; // El puzle no está resuelto

  strcpy(puz[1].id_puzle, "P02");
  puz[1].id_sala_puzle = 1;
  puz[1].tipo_elemento = 2; // El puzle genera un código al resolverlo
  strcpy(puz[1].descripcion_puzle, "Descripcion del puzle 2");
  strcpy(puz[1].solucion_puzle, "SOL02");
  puz[1].resuelto = 0; // El puzle no está resuelto

  // Aquí se podrían cargar los datos de la partida y del jugador, o
  // inicializarlos manualmente para la prueba

  // Llamar a las funciones para probar su funcionamiento
  int j = 0; // Variable para controlar el bucle de prueba
  while (j ==
         0) { // Bucle para repetir la prueba hasta que el usuario decida salir
    printf("=== PRUEBA DE FUNCIONES DE CONDICIONES.C ===\n\n");
    printf("Elige una accion:\n");
    printf("1. Examinar objetos\n");
    printf("2. Ver inventario\n");
    printf("3. Coger objeto\n");
    printf("4. Soltar objeto\n");
    printf("5. Usar objeto\n");
    printf("6. Resolver puzle\n");
    printf("7. Salir\n");
    int opcion;
    scanf("%d", &opcion);
    switch (opcion) {
    case 1:
      printf("\n--- EXAMINAR OBJETOS ---\n");
      ExaminarObjeto(obj, num_objetos, id_sala_actual);
      break;
    case 2:
      printf("\n--- VER INVENTARIO ---\n");
      MostrarInventario(obj, &inv, num_objetos);
      break;
    case 3:
      printf("\n--- COGER OBJETO ---\n");
      CogerObjeto(&inv, obj, num_objetos, id_sala_actual);
      break;
    case 4:
      printf("\n--- SOLTAR OBJETO ---\n");
      SoltarObjeto(&inv, obj, num_objetos, id_sala_actual);
      break;
    case 5:
      printf("\n--- USAR OBJETO ---\n");
      UsarObjeto(obj, &inv, num_conexiones, con, id_sala_actual);
      break;
    case 6:
      printf("\n--- RESOLVER PUZLE ---\n");
      ResolverPuzle(num_conexiones, puz, num_puzles, con, id_sala_actual);
      break;
    case 7:
      printf("Saliendo de la prueba...\n");
      j = 1; // Salir del bucle de prueba
      break;
    default:
      printf("Opcion no valida. Se ejecutarán todas las funciones para la "
             "prueba.\n");
    }
  }

  return 0;
}

void ExaminarObjeto(Objetos *obj, int num_objetos, int id_sala_actual) {
  // Precondición: Deben haber sido cargados los datos de la partida y haber
  // sido seleccionada la opción de examinar objetos en el menú de acciones del
  // jugador Postcondición: Se muestra la descripción de los objetos que se
  // encuentran en la sala actual
  int j = 0; // Indica si se ha encontrado algún objeto en la sala actual para
             // mostrar, para evitar mostrar el mensaje de "No hay objetos en
             // esta sala" varias veces si hay varios objetos
  for (int i = 0; i < num_objetos; i++) {
    if (obj[i].localizacion_objeto ==
        id_sala_actual) { // Si el objeto está en la sala actual
      printf("%s   ->     %s\n", obj[i].nombre_objeto,
             obj[i].descripcion_objeto);
      j = 1; // Se ha encontrado un objeto en la sala actual
    }
  }
  if (j == 0) {
    printf("No hay objetos en esta sala\n");
  }
}

void MostrarInventario(Objetos *obj, Inventario *inv, int num_objetos) {
  // Precondición: Deben haber sido cargados los datos de la partida y haber
  // sido seleccionada la opción de ver el inventario en el menú de acciones del
  // jugador Postcondición: Se muestra la descripción de los objetos que se
  // encuentran en el inventario del jugador
  if (inv->num_objetos ==
      0) { // Si el jugador no tiene objetos en el inventario
    printf("No tienes objetos en el inventario\n");
    return; // Salir de la función para evitar recorrer la lista de objetos
  }
  if (inv->num_objetos == 1) {
    printf("Tienes 1 objeto en el inventario:\n");
  }
  if (inv->num_objetos > 1) {
    printf("Tienes %d objetos en el inventario:\n", inv->num_objetos);
  }
  for (int i = 0; i < num_objetos; i++) {
    if (obj[i].localizacion_objeto ==
        0) { //  Si el objeto está en el inventario
      printf("%s   ->     %s\n", obj[i].nombre_objeto,
             obj[i].descripcion_objeto);
    }
  }
}

void CogerObjeto(Inventario *inv, Objetos *obj, int num_objetos,
                 int id_sala_actual) {
  // Precondición: Deben haber sido cargados los datos de la partida y haber
  // sido seleccionada la opción de coger objetos en el menú de acciones del
  // jugador Postcondición: Se muestra la descripción de los objetos que se
  // encuentran en la sala actual y se pregunta al jugador si desea coger alguno
  // de ellos. Si el jugador decide coger un objeto, este se añade a su
  // inventario y se elimina de la sala.

  int i = 0; // Variable para recorrer la lista de objetos
  int j = 0; // Indica si se ha encontrado algún objeto en la sala actual para
             // coger, para evitar mostrar el mensaje de "No hay objetos en esta
             // sala" varias veces si hay varios objetos
  int encontrado =
      0; // Indica si se ha encontrado un objeto en la sala actual para coger,
         // para evitar mostrar el mensaje de "No hay objetos en esta sala"
         // varias veces si hay varios objetos
  for (i = 0; i < num_objetos && j == 0;
       i++) { // Recorre la lista de objetos para encontrar los que están en la
              // sala actual
    if (obj[i].localizacion_objeto ==
        id_sala_actual) { // Si el objeto está en la sala actual
      encontrado = 1;     // Se ha encontrado un objeto en la sala actual
      printf("%s   ->    ¿Deseas coger este objeto? (1: Si, 0: No)\n",
             obj[i].nombre_objeto);
      int respuesta;
      scanf("%d", &respuesta);

      if (respuesta == 1) {
        // 1. Cambiamos la localización a 0 (Inventario)
        obj[i].localizacion_objeto = 0;

        // 2. Incrementamos el contador y redimensionamos el array
        inv->num_objetos++;
        inv->Inventario =
            realloc(inv->Inventario, inv->num_objetos * sizeof(Objetos));

        // 3. Copiamos el struct entero al último hueco
        inv->Inventario[inv->num_objetos - 1] = obj[i];

        printf("Has cogido el objeto %s\n", obj[i].nombre_objeto);
        j = 1;
      } else { // Si el jugador decide no coger el objeto, se muestra un mensaje
               // de que ha elegido no cogerlo (solo si se ha encontrado al
               // menos un objeto en la sala para evitar mostrar el mensaje
               // varias veces si hay varios objetos)
        printf("Has elegido no coger el objeto %s\n", obj[i].nombre_objeto);
      }
    }
  }
  if (encontrado == 0) {
    printf("No hay objetos en esta sala para coger\n");
  }
}

void SoltarObjeto(Inventario *inv, Objetos *obj, int num_objetos,
                  int id_sala_actual) {
  // Precondición: Deben haber sido cargados los datos de la partida y haber
  // sido seleccionada la opción de soltar objetos en el menú de acciones del
  // jugador Postcondición: Se muestra la descripción de los objetos que se
  // encuentran en el inventario del jugador y se pregunta si desea soltar
  // alguno de ellos. Si el jugador decide soltar un objeto, este se elimina de
  // su inventario y se añade a la sala actual.

  int i = 0; // Variable para recorrer la lista de objetos
  int j = 0; // Indica si se ha encontrado algún objeto en el inventario para
             // soltar, para evitar mostrar el mensaje de "No tienes objetos en
             // el inventario" varias veces si hay varios objetos
  if (inv->num_objetos == 0) {
    printf("No tienes objetos en el inventario para soltar\n");
    j = 1; // Indica que no hay objetos en el inventario para soltar
  }

  for (i = 0; i < inv->num_objetos && j == 0;
       i++) { // Recorre la lista de objetos para encontrar los que están en el
              // inventario
    printf("%s   ->    ¿Deseas soltar este objeto? (1: Si, 0: No)\n",
           inv->Inventario[i].nombre_objeto);
    int respuesta;
    scanf("%d", &respuesta);

    if (respuesta == 1) {
      // Guardamos el nombre antes de eliminar el objeto del inventario
      char nombre[16];
      strcpy(nombre, inv->Inventario[i].nombre_objeto);

      // 1. Cambiamos la localización en obj
      int encontrado_en_partida =
          0; // Indica si se ha encontrado el objeto en la lista de la partida
             // para actualizar su localización, para evitar mostrar el mensaje
             // de error varias veces si hay varios objetos en el inventario
      for (int m = 0; m < num_objetos && encontrado_en_partida == 0;
           m++) { // Recorre la lista de objetos de la partida para encontrar el
                  // objeto que se va a soltar y cambiar su localización a la
                  // sala actual
        if (strcmp(obj[m].id_objeto, inv->Inventario[i].id_objeto) == 0) {
          obj[m].localizacion_objeto =
              id_sala_actual; // Cambia la localización del objeto a la sala
                              // actual
          encontrado_en_partida = 1;
        }
      }
      if (encontrado_en_partida ==
          0) { // Si no se ha encontrado el objeto en la lista de la partida,
               // muestra un mensaje de error (esto no debería ocurrir si los
               // datos de la partida y del jugador están bien formados)
        printf("Error: el objeto %s no se encontro en la lista de la partida\n",
               nombre);
      }

      // 2. Eliminamos el objeto del inventario del jugador
      int encontrado = 0;
      for (int k = 0; k < inv->num_objetos && encontrado == 0;
           k++) { // Recorre la lista de objetos del inventario para encontrar
                  // el objeto que se va a soltar y eliminarlo
        if (strcmp(inv->Inventario[k].id_objeto,
                   inv->Inventario[i].id_objeto) ==
            0) { // Si el id del objeto del inventario coincide con el id del
                 // objeto que se va a soltar
          // 3. Mover el último elemento al hueco (si no es el último)
          if (k < inv->num_objetos -
                      1) { // Si el objeto a eliminar no es el último, se mueve
                           // el último al hueco del objeto a eliminar
            inv->Inventario[k] = inv->Inventario[inv->num_objetos - 1];
          }
          inv->num_objetos--;
          if (inv->num_objetos > 0) { // Redimensionar solo si quedan objetos,
                                      // para evitar realloc a 0
            inv->Inventario =
                realloc(inv->Inventario, inv->num_objetos * sizeof(Objetos));
          } else {
            free(inv->Inventario);
            inv->Inventario = NULL;
          }
          encontrado = 1;
        }
      }

      printf("Has soltado el objeto %s\n", nombre); // ✅ Fuera del bucle k
      j = 1;                                        // ✅ Fuera del bucle k
    }
  }
}

void UsarObjeto(Objetos *obj, Inventario *inv, int num_conexiones,
                Conexiones *con, int id_sala_actual) {
  // Precondición: Deben haber sido cargados los datos de la partida y haber
  // sido seleccionada la opción de usar objetos en el menú de acciones del
  // jugador Postcondición: Se muestra la descripción de los objetos que se
  // encuentran en el inventario del jugador y se pregunta si desea usar alguno
  // de ellos. Si el jugador decide usar un objeto, se comprueba si este puede
  // ser usado en la situación actual (por ejemplo, si puede abrir una conexión
  // o resolver un puzle) y se aplica el efecto correspondiente.

  int i = 0; // Variable para recorrer la lista de objetos del inventario
  int j = 0; // Indica si se ha encontrado algún objeto en el inventario que
             // pueda ser usado para abrir alguna conexión, para evitar mostrar
             // el mensaje de "No hay conexiones que puedan ser abiertas con los
             // objetos en el inventario" varias veces si hay varios objetos
  int k = 0; // Variable para recorrer la lista de conexiones

  // Comprobación previa: ¿hay algún objeto en el inventario que pueda abrir
  // alguna conexión?
  int comprobar = 0; // Indica si hay algún objeto en el inventario que pueda
                     // ser usado para abrir alguna conexión, para evitar
                     // preguntar por objetos que no pueden abrir nada
  for (i = 0; i < inv->num_objetos && comprobar == 0;
       i++) { // Recorre la lista de objetos para encontrar los que están en el
              // inventario
    for (k = 0; k < num_conexiones && comprobar == 0;
         k++) { // Recorre la lista de conexiones para comprobar si el objeto
                // puede ser usado en alguna de ellas
      if (ComprobarConexion(con, k, id_sala_actual,
                            inv->Inventario[i].id_objeto) ==
          1) { // Si la conexión es desde la sala actual, tiene una condición de
               // tipo objeto y el id del objeto coincide con el id del
               // condicionante de la conexión
        comprobar = 1; // Se ha encontrado al menos un objeto en el inventario
                       // que puede abrir alguna conexión
      }
    }
  }

  if (comprobar == 1) { // Solo entrar si hay algún objeto útil
    for (i = 0; i < inv->num_objetos && j == 0;
         i++) { // Recorre la lista de objetos para encontrar los que están en
                // el inventario

      // Comprobar si este objeto concreto puede abrir algo
      int util = 0; // Indica si el objeto que se está evaluando puede abrir
                    // alguna conexión, para evitar preguntar por objetos que no
                    // pueden abrir nada
      for (k = 0; k < num_conexiones && util == 0;
           k++) { // Recorre la lista de conexiones para comprobar si el objeto
                  // puede ser usado en alguna de ellas
        if (ComprobarConexion(con, k, id_sala_actual,
                              inv->Inventario[i].id_objeto) ==
            1) { // Si la conexión es desde la sala actual, tiene una condición
                 // de tipo objeto y el id del objeto coincide con el id del
                 // condicionante de la conexión
          util = 1; // El objeto que se está evaluando puede abrir al menos una
                    // conexión
        }
      }

      if (util == 1) { // Solo preguntar si el objeto puede abrir algo
        printf("%s   ->    ¿Deseas usar este objeto? (1: Si, 0: No)\n",
               inv->Inventario[i].nombre_objeto);
        int respuesta;
        scanf("%d", &respuesta);

        if (respuesta == 1) { // Si el jugador decide usar el objeto, se
                              // comprueba si puede abrir alguna conexión
          for (k = 0; k < num_conexiones && j == 0; k++) {
            printf("Has usado el objeto %s\n",
                   inv->Inventario[i].nombre_objeto);
            AbrirConexion(con, k);
            j = 1;
          }
        } else {
          printf("Has elegido no usar el objeto \n");
        }
      }
    }
  } else {
    printf("No hay conexiones que puedan ser abiertas con los objetos en el "
           "inventario\n");
  }
}

void ResolverPuzle(int num_conexiones, Puzles *puz, int num_puzles,
                   Conexiones *con, int id_sala_actual) {
  // Precondición: Deben haber sido cargados los datos de la partida y haber
  // sido seleccionada la opción de resolver puzles en el menú de acciones del
  // jugador Postcondición: Se muestra la descripción de los puzles que se
  // encuentran en la sala actual y se pregunta al jugador si desea intentar
  // resolver alguno de ellos. Si el jugador decide intentar resolver un puzle,
  // se le pide que introduzca la solución. Si la solución introducida es
  // correcta, se muestra un mensaje de éxito y se marca el puzle como resuelto.

  int i = 0; // Variable para recorrer la lista de puzles
  int j = 0; // Indica si se ha encontrado algún puzle en la sala actual
  int k = 0; // Variable para recorrer la lista de conexiones
  int encontrado =
      0; // Indica si se ha encontrado algún puzle en la sala actual para
         // resolver, para evitar mostrar el mensaje de "No hay puzles en esta
         // sala para resolver" varias veces si hay varios puzles
  for (k = 0; k < num_conexiones && j == 0;
       k++) { // Recorre la lista de conexiones para encontrar las que parten de
              // la sala actual y tienen como condicionante el puzle que se está
              // evaluando
    for (i = 0; i < num_puzles && j == 0;
         i++) { // Recorre la lista de puzles para encontrar los que están en la
                // sala actual
      if (ComprobarConexion(con, k, id_sala_actual, puz[i].id_puzle) ==
          1) { // Si la conexión es desde la sala actual, tiene una condición de
               // tipo puzle y el id del puzle coincide con el id del
               // condicionante de la conexión
        encontrado =
            1; // Se ha encontrado un puzle en la sala actual para resolver
        printf(
            "%s   ->    ¿Deseas intentar resolver este puzle? (1: Si, 0: No)\n",
            puz[i].descripcion_puzle);
        int respuesta;
        scanf("%d", &respuesta);

        if (respuesta == 1) {
          char solucion[51];
          printf("%s:\n", puz[i].descripcion_puzle);
          printf("Introduce la solucion:\n");
          // Limpiar el buffer de entrada antes de leer la solución, para evitar
          // problemas con scanf y fgets
          while (getchar() != '\n')
            ; // Limpiar buffer
          fgets(solucion, 51,
                stdin); // Lee la solución introducida por el jugador
          solucion[strcspn(solucion, "\n")] =
              '\0'; // Elimina el carácter de nueva línea
          if (strcmp(solucion, puz[i].solucion_puzle) ==
              0) { // Si la solución introducida coincide con la solución del
                   // puzle
            puz[i].resuelto = 1; // Marca el puzle como resuelto
            printf("Has resuelto el puzle %s\n", puz[i].descripcion_puzle);
            AbrirConexion(con, k); // Cambia el estado de la conexión a abierta

            j = 1; // Indica que se ha resuelto un puzle y se sale del bucle
          } else {
            printf("La solucion introducida no es correcta para el puzle %s\n",
                   puz[i].descripcion_puzle);
          }
        }
      }
    }
  }
  if (encontrado == 0) {
    printf("No hay puzles en esta sala para resolver\n");
  }
}

int ComprobarConexion(Conexiones *con, int k, int id_sala_actual, char *n) {
  // Precondición: Deben haber sido cargados los datos de la partida y haber
  // sido seleccionada la opción de usar objetos o resolver puzles en el menú de
  // acciones del jugador Postcondición: Se comprueba si la conexión con índice
  // k en la lista de conexiones tiene como sala de origen la sala actual, tiene
  // el estado de conexión a 0 (bloqueada) y tiene como condicionante el id del
  // objeto o puzle que se está evaluando.
  if (con[k].id_sala_orig == id_sala_actual && con[k].estado_conexion == 0 &&
      strcmp(con[k].id_condicionante, n) == 0) {
    return 1; // La conexión está bloqueada por un objeto que se encuentra en el
              // inventario del jugador
  } else {
    return 0; // La conexión no está bloqueada por un objeto que se encuentra en
              // el inventario del jugador
  }
}

void AbrirConexion(Conexiones *con, int k) {
  // Precondición: Deben haber sido cargados los datos de la partida y haber
  // sido seleccionada la opción de usar objetos o resolver puzles en el menú de
  // acciones del jugador, y se ha comprobado que la conexión con índice k en la
  // lista de conexiones tiene como sala de origen la sala actual, tiene el
  // estado de conexión a 0 (bloqueada) y tiene como condicionante el id del
  // objeto o puzle que se está evaluando. Postcondición: Se cambia el estado de
  // la conexión con índice k en la lista de conexiones a 1 (abierta) y se
  // muestra un mensaje indicando que se ha abierto la conexión hacia la sala de
  // destino de dicha conexión.
  con[k].estado_conexion = 1; // Cambia el estado de la conexión a abierta
  printf("Has abierto la conexion hacia la sala %d\n", con[k].id_sala_dest);
}