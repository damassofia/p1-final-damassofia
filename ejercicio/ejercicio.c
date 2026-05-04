/*
 * Programación 1 - Ingenieria en Computación - UNRN Andina
 */

#include "ejercicio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

nodo_t *agregar_producto(nodo_t* lista, producto_t nuevo_producto)
{
    nodo_t *cabeza_resultado = lista;

    nodo_t *nuevo_nodo = (nodo_t*)malloc(sizeof(nodo_t));

    if (nuevo_nodo != NULL)
    {
        nuevo_nodo->datos.codigo_producto = strdup(nuevo_producto.codigo_producto);
        nuevo_nodo->datos.nombre_producto = strdup(nuevo_producto.nombre_producto);
        nuevo_nodo->datos.stock_actual = nuevo_producto.stock_actual;
        nuevo_nodo->datos.punto_reposicion = nuevo_producto.punto_reposicion;
        nuevo_nodo->siguiente = NULL;

        if (lista == NULL || strcmp(lista->datos.codigo_producto, nuevo_producto.codigo_producto) > 0)
        {
            nuevo_nodo->siguiente = lista;
            cabeza_resultado = nuevo_nodo;
        }
        else
        {
            nodo_t *actual = lista;

            while (actual->siguiente != NULL && strcmp(actual->siguiente->datos.nombre_producto, nuevo_producto.nombre_producto) < 0)
            {
                actual = actual->siguiente;
            }

            nuevo_nodo->siguiente = actual->siguiente;
            actual->siguiente = nuevo_nodo;
        }
    }
    else
    {
        //funcion q muestra mensaje de error: malloc fallo
    }

    return cabeza_resultado;
}

nodo_t *cargar_inventario(const char* nombre_archivo)
{
    FILE* archivo = NULL;
    Nodo* lista = NULL;

    archivo = fopen(nombre_archivo , "r");

    if (archivo != NULL)
    {
        char linea[200];

        while(fgets(linea, sizeof(linea) , archivo) != NULL)
        {
            //Al final de cada linea de texto agrega un terminador.
            linea[strcspn(linea , "\n")] = 0;

            char* codigo_producto = strtok(NULL , ";");
            char* nombre_producto = strtok(NULL , ";");
            char* stock_texto = strtok(NULL , ";");
            char* punto_repo_texto = strtok(NULL , ";");

            if(formato != NULL && codigo_producto != NULL && nombre_producto != NULL && stock_actual != NULL && punto_reposicion != NULL)
            {
                producto_t nuevo_producto;
                strcpy(nuevo_producto.codigo_producto , codigo_producto);
                strcpy(nuevo_producto.nombre_producto , nombre_producto);
                nuevo_producto.stock_actual = atoi(stock_texto);
                nuevo_producto.punto_reposicion = atoi(punto_repo_texto);

                //Aun hay q implementar agregar producto a la lista enlazada.
                lista = agregar_producto(lista, nuevo_producto);
            }
        }
        fclose(archivo);
        //Aun hay q agregar alguna funcion para mostrar mensajes.
    }
    else
    {
        //mostrar mensaje de error:
    }
    return lista;
}