/*
 * Programación 1 - Ingenieria en Computación - UNRN Andina
 */

#ifndef EJERCICIO_H
#define EJERCICIO_H

/* Declaración de las estructuras*/

/**
 *Representa los datos operativos de un insumo médico.
 *
 * Invariantes:
 *  - stock_actual debe ser estrictamente mayor a cero (si es 0, el producto no debe existir en la lista).
 *  - codigo_producto y nombre_producto deben apuntar a memoria dinámica válida (no NULL).
 */
typedef struct producto
{
    char* codigo_producto;   // Código alfanumérico del insumo.
    char* nombre_producto;   // Nombre descriptivo del insumo.
    int stock_actual;        // Cantidad física disponible.
    int punto_reposicion;    //
}producto_t;

/**
 * Representa un nodo de la lista simplemente enlazada de inventario.
 * Invariantes:
 *      -'siguiente' debe apuntar al una dirección de memoria válida de otro nodo_t o valer NULL.
 */
typedef struct nodo
{
    producto_t datos;       //Struct tipo producto_t
    struct nodo *siguiente; //Puntero al siguiente nodo de la lista.
}nodo_t;

/**
 * Inserta un nuevo producto en la lista enlazada manteniendo el orden alfabético.
 *
 * @param lista Puntero a la cabeza de la lista enlazada actual.
 * @param nuevo_producto Estructura que contiene los datos del producto a insertar.
 *
 * @pre
 *  - La 'lista' original recibida debe estar previamente ordenada alfabéticamente por código.
 *
 * @post
 *  - Se creará e insertará un nuevo nodo, garantizando que la lista siga ordenada alfabéticamente por 'codigo_producto'.
 *  - Si falla la asignación de memoria para el nuevo nodo, la lista original se mantiene intacta.
 *
 * @return Puntero a la cabeza de la lista actualizada (puede cambiar si el nodo se insertó al principio).
 */
nodo_t *agregar_producto(nodo_t* lista, producto_t nuevo_producto);

/**
 * Lee el archivo de inventario maestro y construye la lista enlazada ordenada.
 *
 * @param *nombre_archivo es el archivo desde donde se cargará la información.
 * @pre
 *  - El archivo debe existir previamente.
 *
 * @post
 *  - El archivo permanecerá intacto post ejecución.
 *  - Se reservará memoria dinámica para cada nodo.
 *
 * @return Puntero a la cabeza de la lista actualizada o NULL si el archivo estab vacío.
 */
nodo_t *cargar_inventario(const char* nombre_archivo);

/**
 * Procesa el lote secuencial de movimientos y actualiza el estado del inventario.
 *
 * @param lista Puntero doble a la cabeza de la lista.
 * @param nombre_archivo Ruta del archivo de transacciones.
 *
 * @post
 *  - Se leen las transacciones y se derivan a la máquina de estados correspondiente.
 *  - Emite advertencias por stderr si el producto no existe, el formato es inválido o la operacion es inválida.
 */
void procesar_transacciones(nodo_t** lista, const char* nombre_archivo);

/**
 * Busca un producto en la lista enlazada utilizando interrupción temprana.
 *
 * @param lista Puntero simple a la cabeza de la lista.
 * @param codigo_producto Cadena de texto con el código a buscar.
 *
 * @pre La lista debe estar ordenada alfabéticamente por código de producto.
 *
 * @return Puntero al nodo encontrado, o NULL si el producto no existe o la búsqueda se interrumpió antes de lo previsto.
 */
nodo_t *buscar_producto(nodo_t *lista, char* codigo_producto);

/**
 * Máquina de estados que deriva la operación a la función correspondiente.
 *
 * @param lista Puntero doble a la cabeza de la lista.
 * @param producto Puntero al nodo sobre el cual operar.
 * @param operacion Carácter que indica 'E' (Entrada) o 'S' (Salida).
 * @param cantidad Valor entero a sumar o restar del stock.
 */
void transacciones(nodo_t **lista , nodo_t *producto, char* operacion , int cantidad);

/**
 * Procesa una salida de stock, validando integridad y eliminando el nodo si es necesario.
 *
 * @param lista Puntero doble a la cabeza de la lista).
 * @param producto Puntero al nodo a actualizar.
 * @param cantidad Valor entero a restar.
 *
 * @post
 *  - Si el stock resultante es negativo, la transacción se aborta y se reporta el error.
 *  - Si el stock llega exactamente a 0, el producto se elimina de la lista y se libera su memoria.
 */
void salida_stock(nodo_t **lista , nodo_t *producto , int cantidad);

/**
 *  Procesa una entrada de stock sumando la cantidad al producto existente].
 *
 * @param producto Puntero al nodo a actualizar.
 * @param cantidad Valor entero a sumar.
 */
void entrada_stock(nodo_t *producto , int cantidad);

/**
 * Elimina un nodo específico de la lista y libera todos sus recursos de memoria asociados.
 *
 * @param lista Puntero doble a la cabeza de la lista.
 * @param codigo_eliminar Cadena de texto con el código del producto a borrar.
 *
 * @post
 *  - El nodo se desenlaza manteniendo la integridad de la lista.
 *  - Se ejecuta free() sobre las cadenas dinámicas internas y luego sobre la estructura base del nodo.
 */
void eliminar_producto(nodo_t **lista , char* codigo_eliminar);

/**
 * Persiste el estado actual de la lista en un archivo de texto secuencial[cite: 1].
 *
 * @param lista Puntero simple a la cabeza de la lista.
 * @param nombre_archivo Ruta del archivo de salida a generar.
 *
 * @post
 *  - Se genera un archivo (o se sobrescribe si ya existe) respetando el formato delimitado por punto y coma.
 */
void escribir_maestro(nodo_t* lista, const char* nombre_archivo);

/**
 * Genera un reporte analítico con los insumos que requieren reposición.
 *
 * @param lista Puntero simple a la cabeza de la lista.
 * @param nombre_archivo Ruta del reporte de salida a generar.
 *
 * @post Filtra y escribe únicamente los productos donde el stock actual sea menor o igual al punto de reposición, calculando el faltante exacto.
 */
void escribir_compras_urgentes(nodo_t* lista, const char* nombre_archivo);

/**
 * Libera la lista completa de la memoria.
 *
 * @param lista Puntero simple a la cabeza de la lista a destruir.
 *
 * @post Recorre iterativamente la lista liberando el código, el nombre y el nodo.
 */
void liberar_lista(nodo_t* lista);

#endif