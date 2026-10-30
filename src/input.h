#ifndef INPUT_H
#define INPUT_H

#include <stddef.h>

// Leer números ingresados por teclado
int leerDesdeTeclado(int **numeros, size_t *cantidad);

// Leer números desde un archivo CSV
int leerDesdeCSV(const char *ruta, int **numeros, size_t *cantidad);

#endif
