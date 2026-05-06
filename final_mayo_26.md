# Proyecto Práctico: Sistema de Conciliación de Inventario Médico

## 1. Contexto Operativo

Una distribuidora de suministros médicos requiere automatizar la conciliación de su inventario diario mediante un software desarrollado en lenguaje C. El ciclo de vida del proceso consiste en cargar el estado inicial del stock (catálogo maestro), procesar un lote transaccional secuencial (movimientos de entrada y salida reportados por sucursales) y persistir el estado actualizado junto con un reporte analítico de compras críticas.

Dada la volatilidad del catálogo y la necesidad de optimización de recursos, el sistema debe implementarse utilizando estructuras de datos dinámicas (Listas Simplemente Enlazadas), minimizando el *footprint* de memoria mediante asignación dinámica estricta en tiempo de ejecución.

### 1.1. Condiciones de Entrega (Mandatorias)

* **Repositorio Plantilla:** El desarrollo debe realizarse obligatoriamente sobre el repositorio plantilla proporcionado por la cátedra. El alumno debe realizar un *fork* o clonar dicho repositorio y trabajar sobre él. La evaluación se ejecutará exclusivamente sobre la estructura de directorios allí definida.

* **Prohibición de Inteligencia Artificial:** Queda estrictamente prohibido el uso de asistentes de Inteligencia Artificial (ej. ChatGPT, GitHub Copilot, Claude, etc.) para la generación de código. El algoritmo, la estructura y la lógica de punteros deben ser de autoría 100% propia del alumno. Se utilizarán herramientas de análisis de plagio y heurísticas de detección de IA.

## 2. Especificaciones Técnicas Detalladas

### 2.1. Entradas (Archivos Secuenciales de Texto)

El sistema consumirá dos archivos con codificación ASCII/UTF-8. El delimitador de campos es estrictamente el carácter punto y coma (`;`). Se debe contemplar el manejo adecuado de los saltos de línea (`\n` o `\r\n`) al parsear los registros.

* **`inventario_maestro.txt` (Estado Inicial):**

  * **Formato:** `CODIGO_PRODUCTO;NOMBRE_PRODUCTO;STOCK_ACTUAL;PUNTO_REPOSICION`

  * **Ejemplo:** `JRG-10;Jeringa Descartable 10ml;500;100`

  * **Tipos de dato inferidos:** Cadena alfanumérica; Cadena alfanumérica con espacios; Entero; Entero.

* **`transacciones.txt` (Lote de Movimientos):**

  * **Formato:** `CODIGO_PRODUCTO;OPERACION;CANTIDAD`

  * **Operaciones válidas:** `'E'` (Entrada/Suma) y `'S'` (Salida/Resta).

  * **Ejemplo:** `JRG-10;S;150`

### 2.2. Estructuras de Datos y Modelado en Memoria

Deberá definirse un TDA (Tipo de Dato Abstracto) basado en una **Lista Simplemente Enlazada**.

* **Tipado de Nodos:**
  Los campos de texto (`codigo_producto`, `nombre_producto`) **no deben ser arreglos estáticos** (ej. `char nombre[50]`). Se exige el uso de punteros a `char` (`char*`). Al instanciar un nodo, se debe determinar la longitud exacta del string leído y utilizar `malloc()` o funciones derivadas (como `strdup()`) para asignar exactamente los bytes necesarios más el terminador nulo (`\0`).

* **Requisito de Inserción Ordenada (O(n)):**
  La función de inserción a la lista enlazada debe realizar un recorrido para ubicar la posición correspondiente utilizando la función `strcmp()` (ordenamiento lexicográfico por `CODIGO_PRODUCTO`).
  *Restricción absoluta:* Está prohibido insertar todos los elementos al inicio/final de la lista para luego aplicar un algoritmo de ordenamiento (ej. Bubble Sort o Merge Sort) a posteriori. La invariante de la lista es que *siempre* se mantiene ordenada durante las inserciones.

### 2.3. Lógica de Procesamiento Algorítmico

* **Fase 1: Parsing y Carga**

  * Apertura de `inventario_maestro.txt` en modo lectura.

  * Lectura línea por línea, tokenización y llamada a la función de inserción ordenada por cada registro válido.

* **Fase 2: Actualización y Manejo de Punteros**

  * Apertura de `transacciones.txt` en modo lectura. Lectura secuencial.

  * Por cada iteración, búsqueda lineal (búsqueda con interrupción temprana, aprovechando que la lista está ordenada) del nodo correspondiente.

  * **Máquina de estados para transacciones:**

    * `OPERACION == 'E'`: `nodo->stock_actual += cantidad`.

    * `OPERACION == 'S'`: Evaluación pre-resta.

      * Si `stock_actual - cantidad < 0`: Abortar transacción, emitir error por `stderr` (ej. "Error: Stock negativo para {CODIGO}"), y continuar.

      * Si `stock_actual - cantidad > 0`: `nodo->stock_actual -= cantidad`.

      * Si `stock_actual - cantidad == 0`: **Borrado de Nodo**. Desconexión del nodo en la lista enlazada (enlazando `anterior->siguiente` con `actual->siguiente`) y liberar su memoria asociada mediante `free()` de sus cadenas internas y luego de la estructura base.

    * **Excepción de Integridad Referencial:** Si el código no existe en la lista, emitir *Warning* por consola y descartar transacción.

### 2.4. Salidas (Reportes y Persistencia)

Una vez finalizado el EOF de `transacciones.txt`, iterar la lista desde la cabeza hasta `NULL` para generar:

1. **`nuevo_maestro.txt`**: Mismo formato y delimitadores que el archivo maestro de entrada. Representa el nuevo estado del sistema. Garantizado de estar ordenado alfabéticamente por topología.

2. **`compras_urgentes.txt`**: Subconjunto de datos filtrados (`stock_actual <= punto_reposicion`).

   * **Formato de salida:** `CODIGO_PRODUCTO - FALTANTE_PARA_REPOSICION`

   * **Cálculo:** `faltante = punto_reposicion - stock_actual + 1`.

### 2.5. Gestión Cíclica de Recursos (Teardown)

* **Limpieza de Memoria:** Implementar función `liberar_lista(Nodo* cabeza)` que recorra la lista completa, liberando iterativamente `codigo_producto`, `nombre_producto` y la estructura `Nodo`.

* **Cierre de I/O:** Verificación de que todo puntero `FILE*` ha sido cerrado con `fclose()`. Ningún descriptor de archivo debe quedar huérfano.

### 2.6. Arquitectura y Modularidad

El diseño del software debe obedecer a principios de **alta cohesión y bajo acoplamiento**. Se requiere el encapsulamiento de la lógica en funciones atómicas e independientes.

* Las funciones deben ser reutilizables (ej. una función `insertar_nodo_ordenado()` no debe contener lógica de lectura de archivos; debe recibir los datos ya parseados).

* Se debe minimizar el uso de variables globales. El estado de la lista debe propagarse mediante punteros (`Nodo**` o devolviendo el nuevo puntero a la cabeza).


