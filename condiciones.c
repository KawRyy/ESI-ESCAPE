#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "condiciones.h"

//Módulo que se encarga de gestionar las condiciones de los objetos, conexiones y puzles, así como el inventario del jugador

void ExaminarObjeto(Objetos *obj, int num_objetos, int id_sala_actual){ 
//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de examinar objetos en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de los objetos que se encuentran en la sala actual
   int j = 0; // Indica si se ha encontrado algún objeto en la sala actual para mostrar, para evitar mostrar el mensaje de "No hay objetos en esta sala" varias veces si hay varios objetos
    for(int i = 0; i < num_objetos; i++){
        if(obj[i].localizacion_objeto == id_sala_actual){ // Si el objeto está en la sala actual
            printf("%s   ->     %s\n", obj[i].nombre_objeto, obj[i].descripcion_objeto);
            j = 1; // Se ha encontrado un objeto en la sala actual
        }
    } if (j == 0){
        printf("No hay objetos en esta sala\n");
    }
}

void MostrarInventario(Objetos *obj, Inventario *inv, int num_objetos){ 
//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de ver el inventario en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de los objetos que se encuentran en el inventario del jugador
    if(inv->num_objetos == 0){ // Si el jugador no tiene objetos en el inventario
        printf("No tienes objetos en el inventario\n");
        return; // Salir de la función para evitar recorrer la lista de objetos
    } if(inv->num_objetos == 1){
        printf("Tienes 1 objeto en el inventario:\n");
    } if(inv->num_objetos > 1){
        printf("Tienes %d objetos en el inventario:\n", inv->num_objetos);
    }
   for(int i = 0; i < num_objetos; i++){
        if(obj[i].localizacion_objeto == 0){ //  Si el objeto está en el inventario
            printf("%s   ->     %s\n", obj[i].nombre_objeto, obj[i].descripcion_objeto);
        }
    }
}


void CogerObjeto(Inventario *inv, Objetos *obj, int num_objetos, int id_sala_actual){
//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de coger objetos en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de los objetos que se encuentran en la sala actual y se pregunta al jugador si desea coger alguno de ellos. 
//Si el jugador decide coger un objeto, este se añade a su inventario y se elimina de la sala.

    int i = 0; // Variable para recorrer la lista de objetos
    int j = 0; // Indica si se ha encontrado algún objeto en la sala actual para coger, para evitar mostrar el mensaje de "No hay objetos en esta sala" varias veces si hay varios objetos

    for(i = 0; i < num_objetos && j == 0; i++){ // Recorre la lista de objetos para encontrar los que están en la sala actual
        if(obj[i].localizacion_objeto == id_sala_actual){ // Si el objeto está en la sala actual
            printf("%s   ->    ¿Deseas coger este objeto? (1: Si, 0: No)\n", obj[i].nombre_objeto);
            int respuesta;
            scanf("%d", &respuesta);

            if(respuesta == 1){
                // 1. Cambiamos la localización a 0 (Inventario)
                obj[i].localizacion_objeto = 0;
                
                // 2. Incrementamos el contador y redimensionamos el array
                inv->num_objetos++;
                inv->Inventario = realloc(inv->Inventario, inv->num_objetos * sizeof(Objetos));
                
                // 3. Copiamos el struct entero al último hueco
                inv->Inventario[inv->num_objetos - 1] = obj[i];

                printf("Has cogido el objeto %s\n", obj[i].nombre_objeto);
                j = 1;
            }
        }
    } if (j == 0){
        printf("No hay objetos para coger en esta sala\n");
    }
}


 void SoltarObjeto(Inventario *inv, Objetos *obj, int num_objetos, int id_sala_actual){
//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de soltar objetos en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de los objetos que se encuentran en el inventario del jugador y se pregunta si desea soltar alguno de ellos. 
//Si el jugador decide soltar un objeto, este se elimina de su inventario y se añade a la sala actual.

    int i = 0; // Variable para recorrer la lista de objetos
    int j = 0; // Indica si se ha encontrado algún objeto en el inventario para soltar, para evitar mostrar el mensaje de "No tienes objetos en el inventario" varias veces si hay varios objetos
   if(inv->num_objetos == 0){
        printf("No tienes objetos en el inventario para soltar\n");
        int j = 1; // Indica que no hay objetos en el inventario para soltar
    }

    for(i = 0; i < inv->num_objetos && j == 0; i++){  // Recorre la lista de objetos para encontrar los que están en el inventario
        printf("%s   ->    ¿Deseas soltar este objeto? (1: Si, 0: No)\n", inv->Inventario[i].nombre_objeto);
        int respuesta;
        scanf("%d", &respuesta);

        if(respuesta == 1){
            // Guardamos el nombre antes de eliminar el objeto del inventario
            char nombre[16];
            strcpy(nombre, inv->Inventario[i].nombre_objeto);

            // 1. Cambiamos la localización en obj
            int encontrado_en_partida = 0; // Indica si se ha encontrado el objeto en la lista de la partida para actualizar su localización, para evitar mostrar el mensaje de error varias veces si hay varios objetos en el inventario
            for(int m = 0; m < num_objetos && encontrado_en_partida == 0; m++){ // Recorre la lista de objetos de la partida para encontrar el objeto que se va a soltar y cambiar su localización a la sala actual
                if(strcmp(obj[m].id_objeto, inv->Inventario[i].id_objeto) == 0){
                    obj[m].localizacion_objeto = id_sala_actual; // Cambia la localización del objeto a la sala actual
                    encontrado_en_partida = 1; 
                }
            }
            if(encontrado_en_partida == 0){ // Si no se ha encontrado el objeto en la lista de la partida, muestra un mensaje de error (esto no debería ocurrir si los datos de la partida y del jugador están bien formados)
                printf("Error: el objeto %s no se encontro en la lista de la partida\n", nombre);
            }

            // 2. Eliminamos el objeto del inventario del jugador
            int encontrado = 0;
            for(int k = 0; k < inv->num_objetos && encontrado == 0; k++){ // Recorre la lista de objetos del inventario para encontrar el objeto que se va a soltar y eliminarlo
                if(strcmp(inv->Inventario[k].id_objeto, inv->Inventario[i].id_objeto) == 0){ // Si el id del objeto del inventario coincide con el id del objeto que se va a soltar
                    // 3. Mover el último elemento al hueco (si no es el último)
                    if(k < inv->num_objetos - 1){ // Si el objeto a eliminar no es el último, se mueve el último al hueco del objeto a eliminar
                        inv->Inventario[k] = inv->Inventario[inv->num_objetos - 1];
                    }
                    inv->num_objetos--;
                    if(jug[n].num_objetos > 0){ // Redimensionar solo si quedan objetos, para evitar realloc a 0
                        jug[n].objetos = realloc(jug[n].objetos, jug[n].num_objetos * sizeof(Objetos));
                    } else {
                        free(jug[n].objetos);
                        jug[n].objetos = NULL;
                    }
                    encontrado = 1;
                }
            }

            printf("Has soltado el objeto %s\n", nombre); // ✅ Fuera del bucle k
            j = 1; // ✅ Fuera del bucle k
        }
    }
}



void UsarObjeto(Objetos *obj, Inventario *inv, int num_conexiones, Conexiones *con, int id_sala_actual){
//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de usar objetos en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de los objetos que se encuentran en el inventario del jugador y se pregunta si desea usar alguno de ellos. 
//Si el jugador decide usar un objeto, se comprueba si este puede ser usado en la situación actual (por ejemplo, 
//si puede abrir una conexión o resolver un puzle) y se aplica el efecto correspondiente.
    
    int i = 0; // Variable para recorrer la lista de objetos
    int j = 0; // Indica si se ha encontrado algún objeto en el inventario
    int k = 0; // Variable para recorrer la lista de conexiones
    int abierto = 0; // Indica si se ha abierto alguna conexión usando el objeto
       
            for(i = 0; i < inv->num_objetos && j == 0; i++){ // Recorre la lista de objetos para encontrar los que están en el inventario
                printf("%s   ->    ¿Deseas usar este objeto? (1: Si, 0: No)\n", inv->Inventario[i].nombre_objeto); // Muestra los objetos del inventario para preguntar si se desea usar alguno
                int respuesta;
                scanf("%d", &respuesta);

                if(respuesta == 1){
                    for(k = 0; k < num_conexiones  && j == 0; k++){ // Recorre la lista de conexiones para comprobar si el objeto puede ser usado en alguna de ellas
                        if(ComprobarConexion(con, k, id_sala_actual, inv->Inventario[i].id_objeto) == 1){ 
                            // Si la conexión es desde la sala actual, tiene una condición de tipo objeto y el id del objeto coincide con el id del condicionante de la conexión
                            printf("Has usado el objeto %s \n", inv->Inventario[i].nombre_objeto);
                            AbrirConexion(con, k); // Cambia el estado de la conexión a abierta
                            abierto = 1; // Indica que se ha abierto una conexión
                            j = 1; // Indica que se ha usado un objeto y se sale del bucle de objetos para evitar mostrar el mensaje de "El objeto no se puede usar en esta situación" varias veces si hay varios objetos en el inventario
                } 
            }
            } if (abierto == 0){
                printf("El objeto %s no se puede usar en esta situacion\n", inv->Inventario[i].nombre_objeto);
        }
    }if (j == 0){
        printf("No hay conexiones que puedan ser abiertas con los objetos en el inventario\n");
        }
}

void ResolverPuzle(int num_conexiones, Puzles *puz, int num_puzles, Conexiones *con, int id_sala_actual){
//Precondición: Deben haber sido cargados los datos de la partida y haber sido seleccionada la opción de resolver puzles en el menú de acciones del jugador
//Postcondición: Se muestra la descripción de los puzles que se encuentran en la sala actual y se pregunta al jugador si desea intentar resolver alguno de ellos.
//Si el jugador decide intentar resolver un puzle, se le pide que introduzca la solución. Si la solución introducida es correcta, se muestra un mensaje de éxito y se marca el puzle como resuelto.

    int i = 0; // Variable para recorrer la lista de puzles
    int j = 0; // Indica si se ha encontrado algún puzle en la sala actual
    int k = 0; // Variable para recorrer la lista de conexiones
     
        for(k = 0; k < num_conexiones && j == 0; k++){ //Recorre la lista de conexiones para encontrar las que parten de la sala actual y tienen como condicionante el puzle que se está evaluando
        for(i = 0; i < num_puzles && j == 0; i++){ //Recorre la lista de puzles para encontrar los que están en la sala actual
            if(ComprobarConexion(con, k,id_sala_actual, puz[i].id_puzle) == 1){ // Si la conexión es desde la sala actual, tiene una condición de tipo puzle y el id del puzle coincide con el id del condicionante de la conexión
                printf("%s   ->    ¿Deseas intentar resolver este puzle? (1: Si, 0: No)\n", puz[i].descripcion_puzle);
                int respuesta;
                scanf("%d", &respuesta);

                if(respuesta == 1){
                    char solucion[51];
                    printf("%s:\n", puz[i].descripcion_puzle);
                    printf("Introduce la solucion:\n");
                    // Limpiar el buffer de entrada antes de leer la solución, para evitar problemas con scanf y fgets
                    while(getchar() != '\n'); // Limpiar buffer
                        fgets(solucion, 51, stdin); // Lee la solución introducida por el jugador
                        solucion[strcspn(solucion, "\n")] = '\0'; // Elimina el carácter de nueva línea
                        if(strcmp(solucion, puz[i].solucion_puzle) == 0){ // Si la solución introducida coincide con la solución del puzle
                            puz[i].resuelto = 1; // Marca el puzle como resuelto
                            printf("Has resuelto el puzle %s\n", puz[i].descripcion_puzle);
                    AbrirConexion(con, k); // Cambia el estado de la conexión a abierta
                    
                
                    j = 1; // Indica que se ha resuelto un puzle y se sale del bucle
                } else {
                    printf("La solucion introducida no es correcta para el puzle %s\n", puz[i].descripcion_puzle);
                }
            
            }
        }
    }

} if (j == 0){
        printf("No hay puzles en esta sala para resolver\n");
    }
}