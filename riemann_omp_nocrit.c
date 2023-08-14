#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main(int argc, char* argv[]) {
    double a, b, n;

    if (argc > 1) {
        a = strtol(argv[1], NULL, 10);
        b = strtol(argv[2], NULL, 10);
        n = strtol(argv[3], NULL, 10);

    } else {
        printf("ERROR: parameters missing\n");
    }

    long double h = (b - a) / n;
    int thread_count = 100;

    // Declaracion de arreglo como variable global
    long double suma_array[thread_count];

    // Parte paralela del prograsma
    # pragma omp parallel num_threads(thread_count)
    {
        // Calculo de variables locales
        int actual_thread = omp_get_thread_num();
        long double n_local = n / omp_get_num_threads();
        long double a_local = a + (actual_thread * n_local * h);
        long double b_local = a_local + (n_local * h);
        long double h_local = (b_local - a_local) / n_local;

        // Iteracuines de la sumatoria
        long double suma_local = 0;
        for (int i = 0; i < n_local; i++)
        {
            long double x = a_local + (h_local * i);

            // f(x) = 2x^3
            // long double fx = 2 * (x * x * x);

            // f(x) = x^2
            // long double fx = x * x;

            // f(x) = sin(x)
            long double fx = sin(x);


            if (i == 0 || i == n_local - 1) {
                fx /= 2;
            }

            suma_local += fx;
        }

        suma_local *= h;

        // Asignacion a array
        suma_array[actual_thread] = suma_local;
    }

    // Reduce de los datos en array
    long double suma = 0;
    for (int i = 0; i < thread_count; i++)
    {
        suma += suma_array[i];
    }

    printf("Con n = %f trapezoides\n", n);
    printf("nuestra aproximacion de la integral de %f a %f es\n> %Lf\n", a, b, suma);
    return 0;
}
