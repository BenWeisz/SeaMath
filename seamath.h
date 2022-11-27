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

typedef struct Matrix {
    float* data;
    int m; // Rows
    int n; // Columns
} Matrix;

Vector* create_vector(int size);
void free_vector(Vector* vec);
Vector* random_vector(int size, float min, float max);

Matrix* create_matrix(int m, int n);
void free_matrix(Matrix* mat);
Matrix* random_matrix(int m, int n, float min, float max);
float get_matrix_element(Matrix* mat, int i, int j);

typedef struct DotTask {
    float* v1;
    float* v2;
    int size;
    int thread_id;
    int threads_used;
    float result;
} DotTask;

typedef struct MatrixVectorMultTask {
    Matrix* mat;
    Vector* vec;
    int thread_id;
    int threads_used;
    Vector* result;
} MatrixVectorMultTask;

float dot(Vector* v1, Vector* v2);
Vector* matrix_vec_mult(Matrix* mat, Vector* vec);

void print_vector(Vector* vec);
void print_matrix(Matrix* mat);

#endif