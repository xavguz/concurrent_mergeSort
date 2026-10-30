#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "mergesort.h"

int main() {

    int opcion;
    int *numeros = NULL;
    size_t cantidad = 0;
    int resultado = 0;

    printf("\n=================================\n");
    printf("       MERGE SORT PARALELO\n");
    printf("=================================\n");

    printf("1. Ingresar numeros por teclado\n");
    printf("2. Leer numeros desde archivo CSV\n");
    printf("3. Salir\n");

    printf("\nSeleccione una opcion: ");

    if (scanf("%d", &opcion) != 1) {
        printf("\nOpcion no valida.\n");
        return 1;
    }

    // Limpiar el salto de linea pendiente
    int caracter;

    while ((caracter = getchar()) != '\n' && caracter != EOF);

    switch (opcion) {

        case 1:
            resultado = leerDesdeTeclado(&numeros, &cantidad);
            break;

        case 2: {
            char ruta[256];

            printf("\nIngrese la ruta del archivo CSV: ");

            if (fgets(ruta, sizeof(ruta), stdin) == NULL) {
                printf("No se pudo leer la ruta.\n");
                return 1;
            }

            // Eliminar el salto de linea
            ruta[strcspn(ruta, "\n")] = '\0';

            resultado = leerDesdeCSV(ruta, &numeros, &cantidad);
            break;
        }

        case 3:
            printf("\nSaliendo del programa...\n");
            return 0;

        default:
            printf("\nOpcion no valida.\n");
            return 1;
    }

    // Verificar que la lectura haya sido exitosa
    if (!resultado) {
        printf("\nNo se pudieron cargar los numeros.\n");
        free(numeros);
        return 1;
    }

    // Mostrar el arreglo original
    printf("\nNumeros leidos correctamente: %zu\n", cantidad);

    printf("\nArreglo original:\n");

    for (size_t i = 0; i < cantidad; i++) {
        printf("%d ", numeros[i]);
    }

    printf("\n");

    // Ejecutar Merge Sort paralelo
    printf("\nOrdenando con Merge Sort paralelo (%d hilos)...\n",
           NUM_HILOS);

    mergeSortParalelo(numeros, cantidad);

    // Mostrar el arreglo ordenado
    printf("\nArreglo ordenado:\n");

    for (size_t i = 0; i < cantidad; i++) {
        printf("%d ", numeros[i]);
    }

    printf("\n");

    // Liberar memoria
    free(numeros);

    printf("\nOrdenamiento finalizado correctamente.\n");

    return 0;
}

