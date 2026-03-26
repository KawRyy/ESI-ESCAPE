# MÓDULO DE PRUEBA PARA juego.c

## 📋 TABLA DE CONTENIDOS
1. [Qué es este módulo](#qué-es-este-módulo)
2. [Cómo usar el módulo](#cómo-usar-el-módulo)
3. [Explicación de cada test](#explicación-de-cada-test)
4. [Tipos de errores que encontrará](#tipos-de-errores-que-encontrará)
5. [Problemas detectados en juego.c](#problemas-detectados-en-juegoc)

---

## ¿QUÉ ES ESTE MÓDULO?

El módulo de prueba `prueba_juego.c` es una **herramienta de testing** que te permite:

- ✓ Probar **cada función por separado**
- ✓ Identificar **errores específicos** sin interferencias
- ✓ Entender **qué hace cada función**
- ✓ Validar **estructuras de datos**
- ✓ Depurar **problemas paso a paso**

### Archivos generados:
- `prueba_juego.c` - Código del módulo de prueba
- `compilar_prueba.bat` - Script para compilar y ejecutar

---

## CÓMO USAR EL MÓDULO

### PASO 1: Compilar y ejecutar
```bash
# En Windows, simplemente ejecuta:
compilar_prueba.bat
```

O manualmente:
```bash
gcc prueba_juego.c -o prueba_juego.exe
prueba_juego.exe
```

### PASO 2: Seleccionar un test
Verás un menú como este:
```
╔════════════════════════════════════════╗
║     MÓDULO DE PRUEBA - JUEGO.C       ║
╚════════════════════════════════════════╝

Selecciona un test para ejecutar:

1. TEST_DESCRIBIR_SALA
   └─ Prueba mostrar descripción de sala actual

2. TEST_EXAMINAR
   └─ Prueba listar objetos y salidas de la sala
...
```

### PASO 3: Ejecutar el test
- Ingresa el número (1-7)
- El test se ejecutará y mostrará resultados
- Presiona ENTER para volver al menú

---

## EXPLICACIÓN DE CADA TEST

### TEST 1: DESCRIBIR_SALA()
**Función original:** `void Describir_Sala(Partida *par)`

**Qué hace:**
- Muestra la descripción textual de la sala donde está el jugador
- Útil para inmersión del jugador en el juego

**Por qué es importante:**
- Verifica que las descripciones se leen correctamente del archivo
- Comprueba que se accede a la sala actual correctamente
- Importante para la experiencia del usuario

**Datos de prueba:**
```
Sala 0 (Entrada): "Estás en una sala oscura. Hay una puerta al norte."
Sala 1 (Pasillo): "Un pasillo largo con paredes de piedra..."
Sala 2 (Salida): "¡Has encontrado la salida!..."
```

**Resultado esperado:**
✓ Muestra el nombre y descripción de la sala actual

---

### TEST 2: EXAMINAR()
**Función original:** `void Examinar(Partida *par)`

**Qué hace:**
- Lista todos los **objetos** en la sala actual
- Muestra todas las **salidas** disponibles (abiertas/bloqueadas)

**Por qué es importante:**
- El jugador necesita saber qué objetos puede coger
- Debe ver qué puertas puede abrir
- Es base para exploración del juego

**Datos de prueba:**
```
Sala 0:
  Objetos: Llave dorada, Mapa antiguo
  Salidas: Sala 1 (ABIERTA)

Sala 1:
  Objetos: Linterna
  Salidas: Sala 2 (BLOQUEADA)
```

**Resultado esperado:**
✓ Lista objetos y puertas correctamente
✓ Indica si están abiertas o bloqueadas

---

### TEST 3: INVENTARIO()
**Función original:** `void Inventario()`

**Qué hace:**
- Muestra todos los objetos que el jugador lleva
- Indicador de cuántos items se pueden llevar (5 máximo)

**Por qué es importante:**
- El jugador necesita ver qué tiene
- Necesita gestionar su inventario limitado
- Base para las mecánicas de coger/soltar objetos

**Datos de prueba:**
```
Jugador inicial:
  • Linterna (en inventario)
  Total: 1/5
```

**Resultado esperado:**
✓ Muestra solo objetos del inventario (localizacion = 0)
✓ Cuenta correctamente

---

### TEST 4: COGER_OBJETO()
**Función original:** `void CogerObjeto(Objetos *obj, Jugadores *jug, Partida *par)`

**Qué hace:**
- Toma un objeto de la sala actual
- Lo añade al inventario del jugador
- Incrementa el contador de items

**Por qué es importante:**
- El jugador puede recopilar objetos
- Los objetos son necesarios para resolver puzles
- Mecánica fundamental del juego

**Pasos de la prueba:**
1. Mostrar objetos disponibles en la sala (que no están en inventario)
2. Cambiar `localizacion_objeto` de sala a 0 (inventario)
3. Incrementar `num_items` del jugador
4. Verificar el cambio

**Resultado esperado:**
```
Items antes: 1
Cogiendo: Llave dorada
Items después: 2
✓ Test completado exitosamente.
```

---

### TEST 5: SOLTAR_OBJETO()
**Función original:** `void SoltarObjeto(Objetos *obj, Jugadores *jug, Partida *par)`

**Qué hace:**
- Toma un objeto del inventario
- Lo coloca en la sala actual
- Disminuye el contador de items

**Por qué es importante:**
- El inventario está limitado (5 items)
- A veces necesitas dejar un objeto para coger otro
- Permite que otros jugadores encuentren el objeto

**Pasos de la prueba:**
1. Mostrar objetos en el inventario
2. Cambiar `localizacion_objeto` a ID de sala actual
3. Decrementar `num_items`
4. Verificar el cambio

**Resultado esperado:**
```
Items antes: 2
Soltando: Linterna
Items después: 1
✓ Test completado exitosamente.
```

---

### TEST 6: RESOLVER_PUZLE()
**Función original:** `void ResolverPuzle(Puzles *puz, Partida *par)`

**Qué hace:**
- Muestra un puzle de la sala actual
- El jugador intenta proporcionar la solución
- Verifica si la respuesta es correcta

**Por qué es importante:**
- Los puzles son la mecánica principal del juego
- Desbloquean nuevas áreas o conseguir items
- Mofan el progreso

**Datos de prueba:**
```
Puzle: "¿Cuál es la contraseña para abrir el cofre?"
Solución correcta: "CASTILLO"
```

**Pasos de la prueba:**
1. **Test 1:** Respuesta incorrecta ("PUERTA")
   - Resultado: ✗ Incorrecto
2. **Test 2:** Respuesta correcta ("CASTILLO")
   - Resultado: ✓ ¡Correcto!

**Resultado esperado:**
```
--- TEST 1: Respuesta incorrecta ---
✗ Incorrecto. La solución 'PUERTA' no es correcta.

--- TEST 2: Respuesta correcta ---
✓ ¡Correcto! Has resuelto el puzle.
```

---

## TIPOS DE ERRORES QUE ENCONTRARÁ

### 1. **Errores de Compilación**
Cuando el código C tiene sintaxis inválida:
```
error: expected ';' before '}'
```
**Solución:** Revisar paréntesis, llaves y puntos y comas

### 2. **Errores de Acceso a Memoria**
Cuando intenta acceder a memoria no asignada:
```
Segmentation fault (core dumped)
```
**Causas comunes:**
- Punteros no inicializados
- Acceso a índices fuera de rango
- Memoria no liberada

### 3. **Errores Lógicos**
El programa compila pero da resultados incorrectos:
```
Items antes: 1
cogiendo: Llave dorada  <-- ¡No actualiza el contador!
Items después: 1 <-- ¡Debería ser 2!
```

### 4. **Comparaciones Incorrectas**
```c
// INCORRECTO (comparar con dos ceros)
if(objetos[i].localizacion_objeto == 00)  // 00 es lo mismo que 0

// CORRECTO
if(objetos[i].localizacion_objeto == 0)
```

---

## PROBLEMAS DETECTADOS EN juego.c

### ⚠️ PROBLEMA 1: Uso incorrecto de punteros
**Ubicación:** Línea ~15
```c
printf("Menu:\n Sala: %s \n", Salas.nombre_sala[Partida.id_sala_actual]);
```
**Error:** `Salas` y `Partida` no son definidas aquí. Debería ser:
```c
printf("Menu:\n Sala: %s \n", sal->nombre_sala[par->id_sala_actual]);
```

### ⚠️ PROBLEMA 2: Parámetros inconsistentes
**Ubicación:** Línea ~21
```c
Describir_Sala(*par->id_sala_actual);  // Pasa un int
```
**Error:** La función espera `Partida *par`, no un int

### ⚠️ PROBLEMA 3: Comparación con doble cero
**Ubicación:** Líneas ~118, 141
```c
if(*par->lista_objetos[i].localizacion_objeto == 00)  // ✗
if(*par->lista_objetos[i].localizacion_objeto == 0)   // ✓
```

### ⚠️ PROBLEMA 4: Funciones no declaradas
**Ubicación:** Línea ~30
```c
accionMover();  // ¡No toma parámetros! Debería ser accionMover(par)
UsarObjeto(*obj, *con, *par);
ResolverPuzle(*puz, *par);
```

### ⚠️ PROBLEMA 5: Cambios de punteros en SoltarObjeto
**Ubicación:** Línea ~155
```c
*par->lista_objetos = realloc(par->lista_objetos, ...);
// Esto es incorrecto. Debería ser:
par->lista_objetos = realloc(par->lista_objetos, ...);
```

---

## CÓMO CORREGIR ERRORES

### Paso 1: Ejecutar el test
```
Ejecuta: compilar_prueba.bat
Selecciona: 4 (TEST_COGER_OBJETO)
```

### Paso 2: Revisar la salida
```
✗ Error: El objeto ya está en el inventario.
```

### Paso 3: Identificar la causa
- Revisar la condición de la prueba
- Comprobar los datos inicializados
- Validar la estructura de datos

### Paso 4: Corregir juego.c
- Abrir juego.c
- Encontrar la línea indicada
- Corregir el error
- Recompilar

### Paso 5: Re-ejecutar el test
- Volver a ejecutar compilar_prueba.bat
- Verificar que el test pase

---

## CHECKLIST DE VALIDACIÓN

Cuando hayas corregido juego.c, verifica que pasen TODOS estos tests:

- [ ] TEST 1: DESCRIBIR_SALA ✓
- [ ] TEST 2: EXAMINAR ✓
- [ ] TEST 3: INVENTARIO ✓
- [ ] TEST 4: COGER_OBJETO ✓
- [ ] TEST 5: SOLTAR_OBJETO ✓
- [ ] TEST 6: RESOLVER_PUZLE ✓

Si todos pasan: **¡Tu código está listo para usar!**

---

## RECURSOS ADICIONALES

- `estructuras.h` - Definiciones de estructuras
- `juego.h` - Declaraciones de funciones
- `juego.c` - Implementación a probar
- `prueba_juego.c` - Este módulo de prueba

---

## PREGUNTAS FRECUENTES

**P: ¿Qué significan `localizacion_objeto == 0`?**
R: 0 = inventario del jugador; otro número = ID de sala donde está

**P: ¿Por qué algunos tests no usan `scanf()`?**
R: Los tests automatizados no pueden esperar input. Para input real, modifica juego.c

**P: ¿Cómo sé si un test pasó?**
R: Busca "✓ Test completado" al final. Si lo ves, pasó.

**P: ¿Puedo modificar los datos de prueba?**
R: ¡Sí! Modifica `inicializar_*_prueba()` para probar diferentes escenarios

---

**Hecho con ❤️ para ayudarte a encontrar y corregir errores en tu código.**
