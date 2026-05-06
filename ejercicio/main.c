/*
 * Programación 1 - Ingenieria en Computación - UNRN Andina
 */

#include <assert.h>
#include <stdio.h>

#include "ejercicio.h"

int main()
{

    nodo_t *inventario = NULL;

    const char *archivo_maestro = "inventario_maestro.txt";
    const char *archivo_transacciones = "transacciones.txt";
    const char *archivo_nuevo_maestro = "nuevo_maestro.txt";
    const char *archivo_urgentes = "compras_urgentes.txt";


    printf("Cargando el inventario maestro...");
    inventario = cargar_inventario(archivo_maestro);
    printf("Inventario cargado!");

    if (inventario == NULL)
    {
        fprintf(stderr, "Error crítico: No se pudo inicializar el inventario desde %s.\n", archivo_maestro);
    }
    else
    {
      printf("Procesando las transacciones ...\n");
      procesar_transacciones(&inventario , archivo_transacciones);

      escribir_maestro(inventario, archivo_nuevo_maestro);
      printf("Se escrbió el nuevo inventario maestro!\n");

      escribir_compras_urgentes(inventario, archivo_urgentes);
      printf("Se creó el archivo de %s.\n" , archivo_urgentes);

      liberar_lista(inventario);

    }
    return 0;
}