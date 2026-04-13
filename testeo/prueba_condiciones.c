#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//PRUEBA PARA COMPROBAR ERRORES DE CONDICIONES.C
typedef struct {
    char id_objeto[5];
    char nombre_objeto[16];
    char descripcion_objeto[51];
    int localizacion_objeto; // 0: Inventario del jugador, si es distinto de 0 indica el ID de la sala donde se encuentra el objeto
} Objetos;

typedef struct {
    char id_puzle[5];
    int id_sala_puzle; // Sala donde se encuentra
    int tipo_elemento; // Elemento que genera resolver el puzle, 1: CÓDIGO ; 2: PALABRA
    char descripcion_puzle[151];
    char solucion_puzle[51];
    int resuelto; // 0: PENDIENTE ; 1: RESUELTO
} Puzles;

typedef struct {
    char id_conexion[5];
    int id_sala_orig;
    int id_sala_dest;
    int estado_conexion; // 0: CERRADA ; 1: ABIERTA
    char id_condicionante[5]; // ID del objeto o puzle que condiciona la conexión, si no tiene condición se pone "0"
} Conexiones;

typedef struct {
    int id_sala_actual;    // Dónde se quedó el jugador

    // Listas dinámicas para reflejar los cambios en el mundo
    Objetos *lista_objetos;      // Array con la ubicación actual de CADA objeto
    int num_objetos;

    Conexiones *lista_conexiones; // Array con el estado (Abierta/Bloqueada) de cada una
    int num_conexiones;

    Puzles *lista_puzles;        // Array con el estado (Resuelto/Pendiente) de cada uno
    int num_puzles;
} Partida;

typedef struct {
    Objetos *objetos;           // Puntero a vector dinámico de objetos (o NULL si no hay ninguno).
    int num_objetos;            // Número de objetos.
} Jugadores;

void ExaminarObjeto(Partida *par);  
void Inventario(Partida *par, Jugadores *jug);
void CogerObjeto(Jugadores *jug, Partida *par);
void SoltarObjeto(Jugadores *jug, Partida *par);
void UsarObjeto(Partida *par, Jugadores *jug);
void ResolverPuzle(Partida *par);

int main() {
    Partida par;
    Jugadores jug;
    par.id_sala_actual = 1; // El jugador empieza en la sala 1
    par.num_objetos = 3; // Hay 3 objetos en total
    par.num_conexiones = 2; // Hay 2 conexiones en total
    par.num_puzles = 2; // Hay 2 puzles en total
    jug.num_objetos = 2; // El jugador empieza sin objetos en el inventario
    par.lista_objetos = malloc(par.num_objetos * sizeof(Objetos)); // Reserva memoria para 3 objetos
    par.lista_conexiones = malloc(par.num_conexiones * sizeof(Conexiones)); // Reserva memoria para 2 conexiones
    par.lista_puzles = malloc(par.num_puzles * sizeof(Puzles)); // Reserva memoria para 2 puzles
    jug.objetos = malloc(jug.num_objetos * sizeof(Objetos)); // Reserva memoria para 2 objetos en el inventario

    strcpy(par.lista_objetos[0].id_objeto, "OB01");
    strcpy(par.lista_objetos[0].nombre_objeto, "Objeto 1");
    strcpy(par.lista_objetos[0].descripcion_objeto, "Descripcion del objeto 1");
    par.lista_objetos[0].localizacion_objeto = 0; // El objeto 1 está en inventario

    strcpy(par.lista_objetos[1].id_objeto, "OB02");
    strcpy(par.lista_objetos[1].nombre_objeto, "Objeto 2");
    strcpy(par.lista_objetos[1].descripcion_objeto, "Descripcion del objeto 2");
    par.lista_objetos[1].localizacion_objeto = 0; // El objeto 2 está en inventario

    strcpy(par.lista_objetos[2].id_objeto, "OB03");
    strcpy(par.lista_objetos[2].nombre_objeto, "Objeto 3");
    strcpy(par.lista_objetos[2].descripcion_objeto, "Descripcion del objeto 3");
    par.lista_objetos[2].localizacion_objeto =  1; // El objeto 3 está en la sala 1

    jug.objetos[0] = par.lista_objetos[0]; // El jugador tiene el objeto 1 en su inventario
    jug.objetos[1] = par.lista_objetos[1]; // El jugador tiene el objeto 2 en su inventario

    strcpy(par.lista_conexiones[0].id_conexion, "C01");
    par.lista_conexiones[0].id_sala_orig = 1;
    par.lista_conexiones[0].id_sala_dest = 2;
    par.lista_conexiones[0].estado_conexion = 0; // La conexión está cerrada
    strcpy(par.lista_conexiones[0].id_condicionante, "OB01"); // La conexión se abre usando el objeto 1

    strcpy(par.lista_conexiones[1].id_conexion, "C02");
    par.lista_conexiones[1].id_sala_orig = 1;
    par.lista_conexiones[1].id_sala_dest = 3;
    par.lista_conexiones[1].estado_conexion = 0; // La conexión está cerrada
    strcpy(par.lista_conexiones[1].id_condicionante, "P01"); // La conexión se abre resolviendo el puzle 1

    strcpy(par.lista_puzles[0].id_puzle, "P01");
    par.lista_puzles[0].id_sala_puzle = 1;
    par.lista_puzles[0].tipo_elemento = 2; // El puzle genera un código al resolverlo
    strcpy(par.lista_puzles[0].descripcion_puzle, "Descripcion del puzle 1");
    strcpy(par.lista_puzles[0].solucion_puzle, "SOL01");
    par.lista_puzles[0].resuelto = 0; // El puzle no está resuelto

    strcpy(par.lista_puzles[1].id_puzle, "P02");
    par.lista_puzles[1].id_sala_puzle = 1;
    par.lista_puzles[1].tipo_elemento = 2; // El puzle genera un código al resolverlo
    strcpy(par.lista_puzles[1].descripcion_puzle, "Descripcion del puzle 2");
    strcpy(par.lista_puzles[1].solucion_puzle, "SOL02");
    par.lista_puzles[1].resuelto = 0; // El puzle no está resuelto

    // Aquí se podrían cargar los datos de la partida y del jugador, o inicializarlos manualmente para la prueba

    // Llamar a las funciones para probar su funcionamiento
    int j = 0; // Variable para controlar el bucle de prueba
 while(j == 0){ // Bucle para repetir la prueba hasta que el usuario decida salir
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
    switch(opcion){
        case 1:
            printf("\n--- EXAMINAR OBJETOS ---\n");
             ExaminarObjeto(&par);
            break;
        case 2:
            printf("\n--- VER INVENTARIO ---\n");
            Inventario(&par, &jug);
            break;
        case 3:
            printf("\n--- COGER OBJETO ---\n");
            CogerObjeto(&jug, &par);
            break;
        case 4:
            printf("\n--- SOLTAR OBJETO ---\n");
             SoltarObjeto(&jug, &par);
            break;
        case 5:
            printf("\n--- USAR OBJETO ---\n");
              UsarObjeto(&par, &jug);
            break;
        case 6:
            printf("\n--- RESOLVER PUZLE ---\n");
            ResolverPuzle(&par);
            break;
        case 7:
            printf("Saliendo de la prueba...\n");
            j = 1; // Salir del bucle de prueba
            break;
        default:
            printf("Opcion no valida. Se ejecutarán todas las funciones para la prueba.\n");
    
 }
}

    return 0;
}

void ExaminarObjeto(Partida *par){ 
//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de examinar objetos en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de los objetos que se encuentran en la sala actual
   int j = 0; // Indica si se ha encontrado algún objeto en la sala actual para mostrar, para evitar mostrar el mensaje de "No hay objetos en esta sala" varias veces si hay varios objetos
    for(int i = 0; i < par->num_objetos; i++){
        if(par->lista_objetos[i].localizacion_objeto == par->id_sala_actual){ // Si el objeto está en la sala actual
            printf("%s   ->     %s\n", par->lista_objetos[i].nombre_objeto, par->lista_objetos[i].descripcion_objeto);
            j = 1; // Se ha encontrado un objeto en la sala actual
        }
    } if (j == 0){
        printf("No hay objetos en esta sala\n");
    }
}

void Inventario(Partida *par, Jugadores *jug){ 
//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de ver el inventario en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de los objetos que se encuentran en el inventario del jugador
   
    if(jug->num_objetos == 0){ // Si el jugador no tiene objetos en el inventario
        printf("No tienes objetos en el inventario\n");
        return; // Salir de la función para evitar recorrer la lista de objetos
    } if(jug->num_objetos == 1){
        printf("Tienes 1 objeto en el inventario:\n");
    } if(jug->num_objetos > 1){
        printf("Tienes %d objetos en el inventario:\n", jug->num_objetos);
    }
   for(int i = 0; i < par->num_objetos; i++){
        if(par->lista_objetos[i].localizacion_objeto == 0){ //  Si el objeto está en el inventario
            printf("%s   ->     %s\n", par->lista_objetos[i].nombre_objeto, par->lista_objetos[i].descripcion_objeto);
        }
    }
}


void CogerObjeto(Jugadores *jug, Partida *par){
//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de coger objetos en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de los objetos que se encuentran en la sala actual y se pregunta al jugador si desea coger alguno de ellos. 
//Si el jugador decide coger un objeto, este se añade a su inventario y se elimina de la sala.

    int i = 0; // Variable para recorrer la lista de objetos
    int j = 0; // Indica si se ha encontrado algún objeto en la sala actual para coger, para evitar mostrar el mensaje de "No hay objetos en esta sala" varias veces si hay varios objetos

    for(i = 0; i < par->num_objetos && j == 0; i++){ // Recorre la lista de objetos para encontrar los que están en la sala actual
        if(par->lista_objetos[i].localizacion_objeto == par->id_sala_actual){ // Si el objeto está en la sala actual
            printf("%s   ->    ¿Deseas coger este objeto? (1: Si, 0: No)\n", par->lista_objetos[i].nombre_objeto);
            int respuesta;
            scanf("%d", &respuesta);

            if(respuesta == 1){
                // 1. Cambiamos la localización a 0 (Inventario)
                par->lista_objetos[i].localizacion_objeto = 0;
                
                // 2. Incrementamos el contador y redimensionamos el array
                jug->num_objetos++;
                jug->objetos = realloc(jug->objetos, jug->num_objetos * sizeof(Objetos));
                
                // 3. Copiamos el struct entero al último hueco
                jug->objetos[jug->num_objetos - 1] = par->lista_objetos[i];

                printf("Has cogido el objeto %s\n", par->lista_objetos[i].nombre_objeto);
                j = 1;
            }
        }
    } if (j == 0){
        printf("No hay objetos para coger en esta sala\n");
    }
}


 void SoltarObjeto(Jugadores *jug, Partida *par){
//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de soltar objetos en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de los objetos que se encuentran en el inventario del jugador y se pregunta si desea soltar alguno de ellos. 
//Si el jugador decide soltar un objeto, este se elimina de su inventario y se añade a la sala actual.

    int i = 0; // Variable para recorrer la lista de objetos
    int j = 0; // Indica si se ha encontrado algún objeto en el inventario para soltar, para evitar mostrar el mensaje de "No tienes objetos en el inventario" varias veces si hay varios objetos
   if(jug->num_objetos == 0){
        printf("No tienes objetos en el inventario para soltar\n");
        int j = 1; // Indica que no hay objetos en el inventario para soltar
    }

    for(i = 0; i < jug->num_objetos && j == 0; i++){  // Recorre la lista de objetos para encontrar los que están en el inventario
        printf("%s   ->    ¿Deseas soltar este objeto? (1: Si, 0: No)\n", jug->objetos[i].nombre_objeto);
        int respuesta;
        scanf("%d", &respuesta);

        if(respuesta == 1){
            // Guardamos el nombre antes de eliminar el objeto del inventario
            char nombre[16];
            strcpy(nombre, jug->objetos[i].nombre_objeto);

            // 1. Cambiamos la localización en par->lista_objetos
            int encontrado_en_partida = 0; // Indica si se ha encontrado el objeto en la lista de la partida para actualizar su localización, para evitar mostrar el mensaje de error varias veces si hay varios objetos en el inventario
            for(int m = 0; m < par->num_objetos && encontrado_en_partida == 0; m++){ // Recorre la lista de objetos de la partida para encontrar el objeto que se va a soltar y cambiar su localización a la sala actual
                if(strcmp(par->lista_objetos[m].id_objeto, jug->objetos[i].id_objeto) == 0){
                    par->lista_objetos[m].localizacion_objeto = par->id_sala_actual; // Cambia la localización del objeto a la sala actual
                    encontrado_en_partida = 1; 
                }
            }
            if(encontrado_en_partida == 0){ // Si no se ha encontrado el objeto en la lista de la partida, muestra un mensaje de error (esto no debería ocurrir si los datos de la partida y del jugador están bien formados)
                printf("Error: el objeto %s no se encontro en la lista de la partida\n", nombre);
            }

            // 2. Eliminamos el objeto del inventario del jugador
            int encontrado = 0;
            for(int k = 0; k < jug->num_objetos && encontrado == 0; k++){ // Recorre la lista de objetos del inventario para encontrar el objeto que se va a soltar y eliminarlo
                if(strcmp(jug->objetos[k].id_objeto, jug->objetos[i].id_objeto) == 0){ // Si el id del objeto del inventario coincide con el id del objeto que se va a soltar
                    // 3. Mover el último elemento al hueco (si no es el último)
                    if(k < jug->num_objetos - 1){ // Si el objeto a eliminar no es el último, se mueve el último al hueco del objeto a eliminar
                        jug->objetos[k] = jug->objetos[jug->num_objetos - 1];
                    }
                    jug->num_objetos--;
                    if(jug->num_objetos > 0){ // Redimensionar solo si quedan objetos, para evitar realloc a 0
                        jug->objetos = realloc(jug->objetos, jug->num_objetos * sizeof(Objetos));
                    } else {
                        free(jug->objetos);
                        jug->objetos = NULL;
                    }
                    encontrado = 1;
                }
            }

            printf("Has soltado el objeto %s\n", nombre); // ✅ Fuera del bucle k
            j = 1; // ✅ Fuera del bucle k
        }
    }
}







void UsarObjeto(Partida *par, Jugadores *jug){
//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de usar objetos en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de los objetos que se encuentran en el inventario del jugador y se pregunta si desea usar alguno de ellos. 
//Si el jugador decide usar un objeto, se comprueba si este puede ser usado en la situación actual (por ejemplo, 
//si puede abrir una conexión o resolver un puzle) y se aplica el efecto correspondiente.
    
    int i = 0; // Variable para recorrer la lista de objetos
    int j = 0; // Indica si se ha encontrado algún objeto en el inventario
    int k = 0; // Variable para recorrer la lista de conexiones
    int abierto = 0; // Indica si se ha abierto alguna conexión usando el objeto
       
            for(i = 0; i < jug->num_objetos && j == 0; i++){ // Recorre la lista de objetos para encontrar los que están en el inventario
                printf("%s   ->    ¿Deseas usar este objeto? (1: Si, 0: No)\n", jug->objetos[i].nombre_objeto); // Muestra los objetos del inventario para preguntar si se desea usar alguno
                int respuesta;
                scanf("%d", &respuesta);

                if(respuesta == 1){
                    for(k = 0; k < par->num_conexiones  && j == 0; k++){ // Recorre la lista de conexiones para comprobar si el objeto puede ser usado en alguna de ellas
                        if(par->lista_conexiones[k].id_sala_orig == par->id_sala_actual && par->lista_conexiones[k].estado_conexion == 0 && strcmp(par->lista_conexiones[k].id_condicionante, jug->objetos[i].id_objeto) == 0){ 
                         // Si la conexión es desde la sala actual, tiene una condición de tipo objeto y el id del objeto coincide con el id del condicionante de la conexión
                        par->lista_conexiones[k].estado_conexion = 1; // Cambia el estado de la conexión a abierta
                    printf("Has usado %s para abrir la conexion hacia la sala %d\n", jug->objetos[i].nombre_objeto, par->lista_conexiones[k].id_sala_dest);
                    abierto = 1; // Indica que se ha abierto una conexión
                    j = 1; // Indica que se ha usado un objeto y se sale del bucle de objetos para evitar mostrar el mensaje de "El objeto no se puede usar en esta situación" varias veces si hay varios objetos en el inventario
                } 
            }
            } if (abierto == 0){
                printf("El objeto %s no se puede usar en esta situacion\n", jug->objetos[i].nombre_objeto);
        }
    }if (j == 0){
        printf("No hay conexiones que puedan ser abiertas con los objetos en el inventario\n");
        }
}

void ResolverPuzle(Partida *par){
//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de resolver puzles en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de los puzles que se encuentran en la sala actual y se pregunta al jugador si desea intentar resolver alguno de ellos.
//Si el jugador decide intentar resolver un puzle, se le pide que introduzca la solución. Si la solución introducida es correcta, se muestra un mensaje de éxito y se marca el puzle como resuelto.

    int i = 0; // Variable para recorrer la lista de puzles
    int j = 0; // Indica si se ha encontrado algún puzle en la sala actual
    int k = 0; // Variable para recorrer la lista de conexiones
     
        for(k = 0; k < par->num_conexiones && j == 0; k++){ //Recorre la lista de conexiones para encontrar las que parten de la sala actual y tienen como condicionante el puzle que se está evaluando
        for(i = 0; i < par->num_puzles && j == 0; i++){ //Recorre la lista de puzles para encontrar los que están en la sala actual
        if(par->lista_conexiones[k].id_sala_orig == par->id_sala_actual && par->lista_conexiones[k].estado_conexion == 0 && strcmp(par->lista_conexiones[k].id_condicionante, par->lista_puzles[i].id_puzle) == 0){ // Detecta los puzles que están en la sala actual
            printf("%s   ->    ¿Deseas intentar resolver este puzle? (1: Si, 0: No)\n", par->lista_puzles[i].descripcion_puzle);
            int respuesta;
            scanf("%d", &respuesta);

            if(respuesta == 1){
                char solucion[51];
                printf("%s:\n", par->lista_puzles[i].descripcion_puzle);
                printf("Introduce la solucion:\n");
                // Limpiar el buffer de entrada antes de leer la solución, para evitar problemas con scanf y fgets
                while(getchar() != '\n'); // Limpiar buffer
                fgets(solucion, 51, stdin); // Lee la solución introducida por el jugador
                solucion[strcspn(solucion, "\n")] = '\0'; // Elimina el carácter de nueva línea
                if(strcmp(solucion, par->lista_puzles[i].solucion_puzle) == 0){ // Si la solución introducida coincide con la solución del puzle
                    par->lista_puzles[i].resuelto = 1; // Marca el puzle como resuelto
                    par->lista_conexiones[k].estado_conexion = 1; // Cambia el estado de la conexión a abierta
                    printf("Has resuelto el puzle %s\n", par->lista_puzles[i].descripcion_puzle);
                
                    j = 1; // Indica que se ha resuelto un puzle y se sale del bucle
                } else {
                    printf("La solucion introducida no es correcta para el puzle %s\n", par->lista_puzles[i].descripcion_puzle);
                }
            
            }
        }
    }

} if (j == 0){
        printf("No hay puzles en esta sala para resolver\n");
    }
}