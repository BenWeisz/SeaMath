#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "seamath.h"

int main() {
    init_seamath();

    int N = 10000000;

    Vector* v1 = random_vector(N, 0, 10);
    Vector* v2 = random_vector(N, 0, 10);

    clock_t start, end;

    start = clock();
    float r = dot(v1, v2);
    end = clock();

    double time_taken = (double)(end - start);

    printf("%f\n", time_taken);

    start = clock();
    float res = 0.0;
    for (int i = 0; i < N; i++)
        res += v1->data[i] * v2->data[i];
    end = clock();

    time_taken = (double)(end - start);

    printf("%f\n", time_taken);

    free_vector(v1);
    free_vector(v2);
    return 0;
}