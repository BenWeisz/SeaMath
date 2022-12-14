#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "seamath.h"

int main() {
    init_seamath();

    Matrix* mat1 = create_matrix(3, 3);
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++)
            mat1->data[(i * 3) + j] = (i * 3) + j;
    }

    Vector* vec = create_vector(3);
    vec->data[0] = 1;
    vec->data[1] = 1;
    vec->data[2] = 1;

    Vector* res = matrix_vec_mult(mat1, vec);
    printf("%f %f %f\n", res->data[0], res->data[1], res->data[2]);

    print_vector(vec);
    print_matrix(mat1);

    // int N = 10000000;

    // Vector* v1 = random_vector(N, 0, 10);
    // Vector* v2 = random_vector(N, 0, 10);

    // clock_t start, end;

    // start = clock();
    // float r = dot(v1, v2);
    // end = clock();

    // double time_taken = (double)(end - start);

    // printf("%f\n", time_taken);

    // start = clock();
    // float res = 0.0;
    // for (int i = 0; i < N; i++)
    //     res += v1->data[i] * v2->data[i];
    // end = clock();

    // time_taken = (double)(end - start);

    // printf("%f\n", time_taken);

    // free_vector(v1);
    // free_vector(v2);
    return 0;
}