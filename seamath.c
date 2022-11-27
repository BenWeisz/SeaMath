#include "seamath.h"

void init_seamath() {
    time_t t;
    srand((unsigned) time(&t));
    // Prime the pump
    rand();
}

Vector* create_vector(int size) {
    Vector* vec = malloc(sizeof(Vector));
    vec->data = (float*)malloc(sizeof(float) * size);
    vec->size = size;
    return vec;
}

void free_vector(Vector* vec) {
    free(vec->data);
    free(vec);
}

Vector* random_vector(int size, float min, float max) {
    Vector* vec = create_vector(size);
    float max_min = max - min;
    for (int i = 0; i < size; i++)
        vec->data[i] = (((float)rand() / RAND_MAX) * max_min) + min;
    return vec;
}

Matrix* create_matrix(int m, int n) {
    Matrix* mat = (Matrix*)malloc(sizeof(Matrix));
    mat->data = (float*)malloc(sizeof(float) * m * n);
    mat->m = m;
    mat->n = n;
    return mat;
}

void free_matrix(Matrix* mat) {
    free(mat->data);
    free(mat);
}

Matrix* random_matrix(int m, int n, float min, float max) {
    Matrix* mat = create_matrix(m, n);
    float max_min = max - min;
    for (int i = 0; i < m * n; i++)
        mat->data[i] = (((float)rand() / RAND_MAX) * max_min) + min;
    return mat;
}

float get_matrix_element(Matrix* mat, int i, int j) {
    return mat->data[(i * mat->n) + j];
}

void* __dot(void *arg) {
    DotTask* task = (DotTask*)arg;
    int partition_size = task->size / task->threads_used;
    int start_pos = task->thread_id * partition_size;
    int end_pos = (task->thread_id + 1) * partition_size;

    if (task->thread_id == task->threads_used - 1)
        end_pos = task->size;

    float result = 0.0;
    for (int i = start_pos; i < end_pos; i++)
        result += task->v1[i] * task->v2[i];

    task->result = result;

    pthread_exit(NULL);
}

float dot(Vector* v1, Vector* v2) {
    assert(v1->size == v2->size);

    // Determine number of threads to use
    int threads_used = 1;
    if (v1->size > NUM_THREADS)
        threads_used = NUM_THREADS;

    // Parcel Tasks
    DotTask* tasks = (DotTask*)malloc(sizeof(DotTask) * threads_used);
    pthread_t threads[NUM_THREADS];

    int rc;
    for (int i = 0; i < threads_used; i++) {
        DotTask* task = &(tasks[i]);
        task->v1 = v1->data;
        task->v2 = v2->data;
        task->size = v1->size;
        task->thread_id = i;
        task->threads_used = threads_used;
        task->result = 0.0;

        rc = pthread_create(&threads[i], NULL, __dot, (void*)task);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    // Wait for threads to finish
    void* status;
    for (int i = 0; i < threads_used; i++) {
        rc = pthread_join(threads[i], &status);
        if (rc) {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
    }
    
    // Compute the final sum
    float result = 0.0;
    for (int i = 0; i < threads_used; i++)
        result += tasks[i].result;

    free(tasks);
    return result;
}

void* __matrix_vec_mult(void* arg) {
    MatrixVectorMultTask* task = (MatrixVectorMultTask*)arg;
    Matrix* mat = task->mat;
    Vector* vec = task->vec;
    int rows_per_thread = mat->m / task->threads_used;
    int start_row = task->thread_id * rows_per_thread;
    int end_row = (task->thread_id + 1) * rows_per_thread;

    if (task->thread_id == task->threads_used - 1)
        end_row = mat->m;

    for (int i = start_row; i < end_row; i++) {
        float row_dot = 0.0;
        for (int j = 0; j < mat->n; j++) {
            row_dot += get_matrix_element(mat, i, j) * vec->data[j];
        }

        task->result->data[i] = row_dot;
    }

    pthread_exit(NULL);
}

Vector* matrix_vec_mult(Matrix* mat, Vector* vec) {
    assert(mat->n == vec->size);
    
    // Determine the number of threads to use
    int threads_used = 1;
    if (mat->n > NUM_THREADS) {
        threads_used = NUM_THREADS;
    }

    MatrixVectorMultTask* tasks = (MatrixVectorMultTask*)malloc(sizeof(MatrixVectorMultTask) * threads_used);
    pthread_t threads[NUM_THREADS];

    Vector* result = create_vector(mat->m);

    int rc;
    for (int i = 0; i < threads_used; i++) {
        MatrixVectorMultTask* task = &tasks[i];
        task->mat = mat;
        task->vec = vec;
        task->thread_id = i;
        task->threads_used = threads_used;
        task->result = result;

        rc = pthread_create(&threads[i], NULL, __matrix_vec_mult, (void*)task);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    void* status;
    for (int i = 0; i < threads_used; i++) {
        rc = pthread_join(threads[i], &status);
        if (rc) {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
    }

    free(tasks);
    return result;
}

void print_vector(Vector* vec) {
    printf("---\n");
    for (int i = 0; i < vec->size; i++)
        printf("%f\n", vec->data[i]);
    printf("---\n");
}

void print_matrix(Matrix* mat) {
    for (int i = 0; i < mat->n * 9; i++)
        printf("-");
    printf("-\n");

    for (int i = 0; i < mat->m; i++) {
        for (int j = 0; j < mat->n; j++)
            printf(" %f", get_matrix_element(mat, i, j));
        printf("\n");
    }

    for (int i = 0; i < mat->n * 9; i++)
        printf("-");
    printf("-\n");   
}
