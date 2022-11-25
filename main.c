#include <stdio.h>
#include <stdlib.h>

#include "seamath.h"

int main() {
    init_seamath();

    int N = 1000000;

    Vector* v1 = random_vector(N, 0, 10);
    Vector* v2 = random_vector(N, 0, 10);

    float r = dot(v1, v2);

    printf("%f\n", r);

    float res = 0.0;
    for (int i = 0; i < N; i++)
        res += v1->data[i] * v2->data[i];

    printf("%f\n", res);

    free_vector(v1);
    free_vector(v2);
    return 0;
}