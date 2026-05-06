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
        nuevo_nodo->datos.codigo_producto = nuevo_producto.codigo_producto;
        nuevo_nodo->datos.nombre_producto = nuevo_producto.nombre_producto;
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

            while (actual->siguiente != NULL && strcmp(actual->siguiente->datos.codigo_producto, nuevo_producto.codigo_producto) < 0)
            {
                actual = actual->siguiente;
            }

            nuevo_nodo->siguiente = actual->siguiente;
            actual->siguiente = nuevo_nodo;
        }
    }
    else
    {
        fprintf(stderr, "Error: malloc fallo.");
        free(nuevo_producto.codigo_producto);
        free(nuevo_producto.nombre_producto);
    }

    return cabeza_resultado;
}

nodo_t *cargar_inventario(const char* nombre_archivo)
{
    FILE* archivo = NULL;
    nodo_t* lista = NULL;

    archivo = fopen(nombre_archivo , "r");

    if (archivo != NULL)
    {
        char linea[200];

        while(fgets(linea, sizeof(linea) , archivo) != NULL)
        {
            linea[strcspn(linea , "\n")] = 0;

            char* codigo_producto = strtok(linea , ";");
            char* nombre_producto = strtok(NULL , ";");
            char* stock_texto = strtok(NULL , ";");
            char* punto_repo_texto = strtok(NULL , ";");

            if(codigo_producto != NULL && nombre_producto != NULL && stock_texto != NULL && punto_repo_texto != NULL)
            {
                producto_t nuevo_producto;
                nuevo_producto.codigo_producto = strdup(codigo_producto);
                nuevo_producto.nombre_producto = strdup(nombre_producto);
                nuevo_producto.stock_actual = atoi(stock_texto);
                nuevo_producto.punto_reposicion = atoi(punto_repo_texto);

                lista = agregar_producto(lista, nuevo_producto);
            }
        }
        fclose(archivo);
    }
    else
    {
        fprintf(stderr, "Error: no pudo abrirse el archivo.");
    }
    return lista;
}

void procesar_transacciones(nodo_t** lista, const char* nombre_archivo)
{
    FILE* archivo = NULL;

    archivo = fopen(nombre_archivo , "r");

    if (archivo != NULL)
    {
        char linea[200];

        while(fgets(linea, sizeof(linea) , archivo) != NULL)
        {
            linea[strcspn(linea , "\n")] = 0;
            char* codigo_producto = strtok(linea , ";");
            char* operacion = strtok(NULL , ";");
            char* cantidad_texto = strtok(NULL , ";");

            int cantidad = atoi(cantidad_texto);

            nodo_t *producto = buscar_producto(*lista , codigo_producto);

            if(producto != NULL)
            {
                transacciones(lista , producto, operacion, cantidad);
            }
            else
            {
                mensaje(3);
            }
        }
        fclose(archivo);
    }
    else
    {
       fprintf(stderr, "Error: no pudo abrirse el archivo.\n");
    }
}

nodo_t *buscar_producto(nodo_t *lista, char* codigo_producto)
{
    nodo_t *actual = lista;
    nodo_t *producto_encontrado = NULL;
    int busqueda = 1;

    while(actual != NULL && strcmp(actual->datos.codigo_producto, codigo_producto) <= 0)
    {
        busqueda = strcmp(actual->datos.codigo_producto , codigo_producto);
        if(busqueda == 0)
        {
            producto_encontrado = actual;
        }
        actual = actual-> siguiente;
    }
    return producto_encontrado;
}

void transacciones(nodo_t **lista , nodo_t *producto, char* operacion , int cantidad)
{
    if(*operacion == 'E')
    {
        entrada_stock(producto , cantidad);
    }
    else if(*operacion == 'S')
    {
        salida_stock(lista, producto , cantidad);
    }
}

void salida_stock(nodo_t **lista , nodo_t *producto , int cantidad)
{
    if((producto->datos.stock_actual - cantidad) < 0)
    {
        fprintf(stderr , "No hay stock suficiente. \n");
    }
    else if((producto->datos.stock_actual - cantidad) == 0)
    {
        eliminar_producto(lista, producto->datos.codigo_producto);
    }
    else
    {
        producto->datos.stock_actual = producto->datos.stock_actual - cantidad;
    }
}

void entrada_stock(nodo_t *producto , int cantidad)
{
    producto->datos.stock_actual = producto->datos.stock_actual + cantidad;
}

void eliminar_producto(nodo_t **lista , char* codigo_eliminar)
{
    if (*lista == NULL)
    {
        fprintf(stderr , "El Inventario esta vacio.\n");
    }
    nodo_t *actual = *lista;
    nodo_t *anterior = NULL;
    int buscado = 1;

    while(actual != NULL && buscado != 0)
    {
        buscado = strcmp(actual->datos.codigo_producto, codigo_eliminar);
        if (buscado != 0)
        {
            anterior = actual;
            actual = actual->siguiente;
        }
    }

    if (actual == NULL)
    {
        fprintf(stderr , "Producto no encontrado.\n");
    }
    if(actual != NULL)
    {
      if (anterior == NULL)
      {
          *lista = actual->siguiente;
      }
      else
      {
          anterior->siguiente = actual->siguiente;

      }
    }

    if(actual != NULL)
    {
        free(actual->datos.codigo_producto);
        free(actual->datos.nombre_producto);
        free(actual);
    }

}

void escribir_maestro(nodo_t* lista, const char* nombre_archivo)
{
    FILE *archivo = fopen(nombre_archivo , "w");

    if(archivo != NULL)
    {
        if(lista == NULL)
        {
          fprintf(stderr, "El Inventario esta vacio.\n");
        }
        else
        {
            nodo_t *actual = lista;
            while(actual != NULL)
            {
                fprintf(archivo, "%s;%s;%d;%d\n",
                    actual->datos.codigo_producto,
                    actual->datos.nombre_producto,
                    actual->datos.stock_actual,
                    actual->datos.punto_reposicion);

                actual = actual -> siguiente;
            }
        }
        fclose(archivo);
    }
    else
    {
        fprintf(stderr, "No se pudo abrir el archivo.");
    }
}

void escribir_compras_urgentes(nodo_t* lista, const char* nombre_archivo)
{
    FILE *archivo = fopen(nombre_archivo , "w");

    if(archivo != NULL)
    {
        if(lista == NULL)
        {
            fprintf(stderr , "El Inventario esta vacio.\n");
        }
        else
        {
            nodo_t *actual = lista;
            while(actual != NULL)
            {
                if(actual->datos.stock_actual <= actual->datos.punto_reposicion)
                {
                    int faltante = actual->datos.punto_reposicion - actual->datos.stock_actual + 1;
                    fprintf(archivo, "%s - %d\n", actual->datos.codigo_producto, faltante);
                }

                actual = actual -> siguiente;
            }
        }
        fclose(archivo);
    }
    else
    {
        fprintf(stderr , "No se pudo abrir el archivo.");
    }
}

void liberar_lista(nodo_t* lista)
{
    nodo_t* actual = lista;
    nodo_t* siguiente;

   while (actual != NULL)
   {
       siguiente = actual->siguiente;
       free(actual->datos.codigo_producto);
       free(actual->datos.nombre_producto);
       free(actual);
       actual = siguiente;
   }
}