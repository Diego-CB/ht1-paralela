// compilacion: mpicc -g ./riemannMPI.c -o rie
// uso: mpirun -np 4 rie

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

// Generacion de vector de numeros aleatorios
int* randGen() {
    srand(1);
    int* numeros = (int*)malloc(5000 * sizeof(int));

    for (int i = 0; i < 5000; i++) {
        int val = rand() % 1000;
        val = val < 0 ? val * -1 : val;
        // printf("> %d\n", val);
        numeros[i] = val;
    }
    return numeros;
}

int main(int argc, char** argv) {
    // Iniciar ambiente de MPI
    int world_size;
    int world_rank;
    float suma;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Division de trabajo
    int n = 10e6;
    float a = 0;
    float b = 100000000;
    float h = (b - a) / n;
    double tstart, tend;

    // Calculo de variables locales
    float n_local = n / world_size;
    float a_local = a + (world_rank * n_local * h);
    float b_local = a_local + (n_local * h);
    float h_local = (b_local - a_local) / n_local;

    // Iteracuines de la sumatoria
    float* numeros = (float*) malloc(n_local * sizeof(float));
    for (int i = 0; i < n_local; i++)
    {
        float x = a_local + (h_local * i);

        // f(x) = 2x^3
        // float fx = 2 * (x * x * x);

        // f(x) = x^2
        float fx = x * x;

        // f(x) = sin(x)
        // float fx = sin(x);


        if (i == 0 || i == n_local - 1) {
            fx /= 2;
        }

        numeros[i] = fx;
        if (world_rank == 0){
            // printf("> id %d i %d: %f\n", world_rank, i, fx);
        }
    }

    for (int i = 0; i < n_local; i++){
        if (world_rank == 0){
            // printf("> i %d %f\n", i, numeros[i]);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if(world_rank==0){
        tstart = MPI_Wtime();
    }

    // Reduccion de suma
    MPI_Reduce(&numeros, &suma, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    // Cada proceso imprime los resultados
    if (world_rank == 0) {
        printf("----------------------------------\n");
        printf("suma: %lf\n", (suma  > 0) ? (double) suma : (double) -suma);
        tend = MPI_Wtime();
        printf("Took %f ms to run\n", (tend-tstart) * 1000);
    }

    MPI_Finalize(); // Finalizar el ambiente
}
