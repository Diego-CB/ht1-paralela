#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main(int argc, char* argv[]) {
    int n = 10e6;
    double a, b;

    if (argc > 1) {
        a = strtol(argv[1], NULL, 10);
        b = strtol(argv[2], NULL, 10);
    } else {
        printf("ERROR: parameters missing\n");
    }

    long double h = (b - a) / n;
    long double suma = 0;

    for (int i = 0; i < n; i++)
    {
        long double x = a + (h * i);
        long double fx = 2 * (x * x * x); // f(x) = 2x^3
        // long double fx = x * x; // f(x) = x^2
        // long double fx = sin(x); // f(x) = sin(x)


        if (i == 0 || i == n) {
            fx /= 2;
            // fx *= h/2;
        }

        suma += fx;
    }
    suma *= h;

    // suma = suma * (h/2);
    
    printf("Con n = %d trapezoides\n", n);
    printf("nuestra aproximacion de la integral de %f a %f es\n> %Lf\n", a, b, suma);
    return 0;
}
