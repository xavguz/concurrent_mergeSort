
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>

#include "input.h"

// Procesar una linea y almacenar los enteros
static int procesarLinea(char *linea, int **numeros, size_t *cantidad) {
    char *token;
    char *guardar;

    token = strtok_r(linea, ", \t\r\n", &guardar);

    while (token != NULL) {
        char *fin;
        errno = 0;

        long valor = strtol(token, &fin, 10);

        // Validar que sea un entero correcto
        if (token == fin || *fin != '\0' ||
            errno == ERANGE || valor < INT_MIN || valor > INT_MAX) {
            fprintf(stderr, "Valor entero no valido: %s\n", token);
            return 0;
        }

        // Ampliar el arreglo dinamicamente
        int *temporal = realloc(
            *numeros,
            (*cantidad + 1) * sizeof(int)
        );

        if (temporal == NULL) {
            perror("Error reservando memoria");
            return 0;
        }

        *numeros = temporal;
        (*numeros)[*cantidad] = (int)valor;
        (*cantidad)++;

        token = strtok_r(NULL, ", \t\r\n", &guardar);
    }

    return 1;
}

// Leer numeros ingresados por teclado
int leerDesdeTeclado(int **numeros, size_t *cantidad) {
    char *linea = NULL;
    size_t capacidad = 0;

    *numeros = NULL;
    *cantidad = 0;

    printf("\nIngrese los numeros enteros separados por comas:\n");

    ssize_t longitud = getline(&linea, &capacidad, stdin);

    if (longitud == -1) {
        fprintf(stderr, "Error leyendo desde teclado.\n");
        free(linea);
        return 0;
    }

    int resultado = procesarLinea(linea, numeros, cantidad);

    free(linea);

    if (!resultado || *cantidad == 0) {
        if (*cantidad == 0 && resultado) {
            fprintf(stderr, "No se ingresaron numeros.\n");
        }

        free(*numeros);
        *numeros = NULL;
        *cantidad = 0;
        return 0;
    }

    return 1;
}

// Leer numeros desde un archivo CSV
int leerDesdeCSV(const char *ruta, int **numeros, size_t *cantidad) {
    *numeros = NULL;
    *cantidad = 0;

    FILE *archivo = fopen(ruta, "r");

    if (archivo == NULL) {
        perror("Error abriendo el archivo CSV");
        return 0;
    }

    char *linea = NULL;
    size_t capacidad = 0;
    ssize_t longitud;
    int resultado = 1;

    while ((longitud = getline(&linea, &capacidad, archivo)) != -1) {
        if (!procesarLinea(linea, numeros, cantidad)) {
            resultado = 0;
            break;
        }
    }

    if (ferror(archivo)) {
        perror("Error leyendo el archivo CSV");
        resultado = 0;
    }

    free(linea);
    fclose(archivo);

    if (!resultado || *cantidad == 0) {
        if (*cantidad == 0 && resultado) {
            fprintf(stderr, "El archivo no contiene numeros.\n");
        }

        free(*numeros);
        *numeros = NULL;
        *cantidad = 0;
        return 0;
    }

    return 1;
}

