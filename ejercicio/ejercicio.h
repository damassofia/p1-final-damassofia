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


nodo_t *agregar_producto(nodo_t* lista, producto_t nuevo_producto);
nodo_t *cargar_inventario(const char* nombre_archivo);

void procesar_transacciones(nodo_t** lista, const char* nombre_archivo);
nodo_t *buscar_producto(nodo_t *lista, char* codigo_producto);


#endif
