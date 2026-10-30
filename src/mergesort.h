#ifndef MERGESORT_H
#define MERGESORT_H

#include <stddef.h>

// Cantidad predeterminada de hilos
#define NUM_HILOS 4

// Merge Sort tradicional
void mergeSort(int arreglo[], size_t inicio, size_t fin);

// Merge Sort paralelo
void mergeSortParalelo(int arreglo[], size_t cantidad);

#endif

