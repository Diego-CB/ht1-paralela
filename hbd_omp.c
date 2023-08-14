#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void hello_world(void);

int main(int argc, char* argv[]) {
    int thread_count = 10;
    if (argc > 1) {
        thread_count = strtol(argv[1], NULL, 10);
    }

    #pragma omp parallel num_threads(thread_count)
    hello_world();

    return 0;
}

void hello_world(void) {
    int actual_thread = omp_get_thread_num();

    if (actual_thread % 2 == 0) {
        printf("Saludos del hilo %d\n", actual_thread);
    } else {
        int thread_count = omp_get_num_threads();
        printf("Feliz cumpleanos numero %d\n", thread_count);
    }
}