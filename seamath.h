#ifndef SEAMATH_H
#define SEAMATH_H

#define NUM_THREADS 8

#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void init_seamath();

typedef struct Vector {
    float* data;
    int size;
} Vector;

Vector* create_vector(int size);
void free_vector(Vector* vec);
Vector* random_vector(int size, float min, float max);

typedef struct DotTask {
    float* v1;
    float* v2;
    int size;
    int thread_id;
    int threads_used;
    float result;
} DotTask;

float dot(Vector* v1, Vector* v2);

#endif