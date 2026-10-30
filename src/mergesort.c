
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stddef.h>

#include "mergesort.h"

// Estructura para indicar que parte del arreglo ordenara cada hilo
typedef struct {
    int *arreglo;
    size_t inicio;
    size_t fin;
} DatosHilo;


// Fusionar dos segmentos ordenados
static void fusionar(int arreglo[], size_t inicio,
                     size_t medio, size_t fin) {

    size_t cantidad = fin - inicio;

    int *temporal = malloc(cantidad * sizeof(int));

    if (temporal == NULL) {
        perror("Error reservando memoria para la fusion");
        exit(EXIT_FAILURE);
    }

    size_t i = inicio;
    size_t j = medio;
    size_t k = 0;

    // Comparar los elementos de ambos segmentos
    while (i < medio && j < fin) {
        if (arreglo[i] <= arreglo[j]) {
            temporal[k++] = arreglo[i++];
        } else {
            temporal[k++] = arreglo[j++];
        }
    }

    // Copiar los elementos restantes del primer segmento
    while (i < medio) {
        temporal[k++] = arreglo[i++];
    }

    // Copiar los elementos restantes del segundo segmento
    while (j < fin) {
        temporal[k++] = arreglo[j++];
    }

    // Copiar el resultado al arreglo original
    for (size_t x = 0; x < cantidad; x++) {
        arreglo[inicio + x] = temporal[x];
    }

    free(temporal);
}


// Merge Sort secuencial
// Ordena los elementos desde inicio hasta fin (sin incluir fin)
void mergeSort(int arreglo[], size_t inicio, size_t fin) {

    if (fin - inicio <= 1) {
        return;
    }

    size_t medio = inicio + (fin - inicio) / 2;

    mergeSort(arreglo, inicio, medio);
    mergeSort(arreglo, medio, fin);

    fusionar(arreglo, inicio, medio, fin);
}


// Funcion ejecutada por cada hilo
static void *ordenarSegmento(void *arg) {

    DatosHilo *datos = (DatosHilo *)arg;

    mergeSort(datos->arreglo, datos->inicio, datos->fin);

    return NULL;
}


// Merge Sort paralelo
void mergeSortParalelo(int arreglo[], size_t cantidad) {

    if (cantidad <= 1) {
        return;
    }

    // No crear mas hilos que elementos
    size_t numHilos = NUM_HILOS;

    if (cantidad < numHilos) {
        numHilos = cantidad;
    }

    pthread_t hilos[NUM_HILOS];
    DatosHilo datos[NUM_HILOS];

    size_t base = cantidad / numHilos;
    size_t extra = cantidad % numHilos;

    size_t inicio = 0;

    // Crear los hilos y distribuir el arreglo
    for (size_t i = 0; i < numHilos; i++) {

        size_t elementos = base;

        // Distribuir los elementos sobrantes
        if (i < extra) {
            elementos++;
        }

        datos[i].arreglo = arreglo;
        datos[i].inicio = inicio;
        datos[i].fin = inicio + elementos;

        inicio = datos[i].fin;

        int resultado = pthread_create(
            &hilos[i],
            NULL,
            ordenarSegmento,
            &datos[i]
        );

        if (resultado != 0) {
            fprintf(stderr,
                    "No se pudo crear el hilo %zu. Se ordenara directamente.\n",
                    i);

            // Si falla la creacion, ordenar ese segmento en el hilo principal
            ordenarSegmento(&datos[i]);
            hilos[i] = (pthread_t)0;
        }
    }

    // Esperar a que terminen todos los hilos
    for (size_t i = 0; i < numHilos; i++) {

        if (hilos[i] != (pthread_t)0) {
            pthread_join(hilos[i], NULL);
        }
    }

    // Fusionar los segmentos que ya estan ordenados
    size_t finAnterior = datos[0].fin;

    for (size_t i = 1; i < numHilos; i++) {

        fusionar(
            arreglo,
            0,
            finAnterior,
            datos[i].fin
        );

        finAnterior = datos[i].fin;
    }
}

