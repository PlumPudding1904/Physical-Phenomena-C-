#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
using namespace std;

// Stałe
const int N = 1000;          // Liczba punktów dyskretyzacji kąta
const double L = 1.0;       // Długość igły (ustaw dowolną wartość)
const int itermax = 100;   // Liczba iteracji dla każdej wartości rho
const double dtheta = 2.0 * M_PI / (double)N;  // Krok kątowy

// Funkcja generująca liczbę losową z przedziału [a, b]
double losowa(double a, double b)
{
    double u = (double)(rand() % RAND_MAX) / (double)RAND_MAX;
    return a + (b - a) * u;
}

// Funkcja obliczająca całkę V_excl * f(θ1) * f(θ2)
double obliczCalke(double f[])
{
    double calka= 0.0;
    for(int i = 0; i < N; i++)
    {
        double theta1 = (double)i / (double)N * 2.0 * M_PI;
        for(int j = 0; j < N; j++)
        {
            double theta2 = (double)j / (double)N * 2.0 * M_PI;
            double V_excl = fabs(sin(theta1 - theta2));  // Wykluczona objętość
            calka += V_excl * f[i] * f[j] * dtheta * dtheta;
        }
    }
    return calka;
}

int main()
{
    srand(time(NULL));  // Inicjalizacja generatora liczb losowych
    double f[N], fpom[N];
    FILE *plik = fopen("wynik.txt", "w");
    fprintf(plik, "rho\tS\tbetaP\n");  // Nagłówki kolumn w pliku wynikowym
    double betaP=0.0;

    // Główna pętla po wartościach rho
    for(double rho = 0.1; rho <= 10.0; rho += 0.1)
    {
        // Inicjalizacja f(θ) losowymi wartościami
        for(int i = 0; i < N; i++)
            f[i] = losowa(0.0, 1.0);

        // Normalizacja f(θ)
        double fsuma = 0.0;
        for(int i = 0; i < N; i++)
            fsuma += f[i] * dtheta;
        for(int i = 0; i < N; i++)
            f[i] /= fsuma;

        // Iteracyjne rozwiązanie równania Onsagera
        for(int iter = 0; iter < itermax; iter++)
        {
            for(int i = 0; i < N; i++)
            {
                double suma = 0.0;
                double thetai = (double)i / (double)N * 2.0 * M_PI;
                for(int j = 0; j < N; j++)
                {
                    double thetaj = (double)j / (double)N * 2.0 * M_PI;
                    suma += fabs(sin(thetai - thetaj)) * f[j] * dtheta;
                }
                fpom[i] = exp(-rho * suma);
            }

            // Normalizacja fpom(θ)
            fsuma = 0.0;
            for(int i = 0; i < N; i++)
                fsuma += fpom[i] * dtheta;
            for(int i = 0; i < N; i++)
                fpom[i] /= fsuma;

            // Aktualizacja f(θ)
            for(int i = 0; i < N; i++)
                f[i] = fpom[i];
        }

        // Obliczenie parametru porządku S
        int indeksmax = 0;
        double maximum = 0.0;
        for(int i = 0; i < N; i++)
        {
            if(f[i] > maximum)
            {
                maximum = f[i];
                indeksmax = i;
            }
        }

        double S = 0.0;
        for(int i = 0; i < N; i++)
            S += cos(2.0 * (double)(i - indeksmax) / (double)N * 2.0 * M_PI) * f[i] * dtheta;

        // Obliczenie równania stanu βP
        double calka = obliczCalke(f);
        betaP = rho/(L*L) + 0.5 * (rho/(L*L))*(rho/(L*L)) * calka;  // d = 2 (2D)

        // Zapis wyników do pliku
         printf("rho = %.2lf, calka = %.10lf, betaP = %.10lf\n", rho, calka, betaP);
        fprintf(plik, "%.2lf\t%.10lf\t%.10lf\n", rho, S, betaP);
    }

    fclose(plik);
    cout << "Obliczenia zakończone. Wyniki zapisano w pliku wynik.txt." << endl;
    return 0;
}
