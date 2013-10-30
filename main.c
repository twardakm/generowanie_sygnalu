#define _USE_MATH_DEFINES
#include "include/header.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
    double amplituda;
    double fs; //częstotliwość sygnału
    double fp; //częstotliwość próbkowania
    double fi; //przesunięcie fazowe
} parametry;

typedef struct
{
    double *tab; //tablica przechowywania danych
    int czas;
    double rozmiar_tablicy;
    FILE *plik;
    int czy_udalo_plik;
} dane_do_wyswietlenia;

void utworz_tablice(dane_do_wyswietlenia *dane);
void usun_tablice(dane_do_wyswietlenia *dane);
void generuj_sygnal(parametry *p, dane_do_wyswietlenia *dane);
void pobierz_dane(parametry *p);
void wyswietl_sygnal(parametry *p, dane_do_wyswietlenia *dane, FILE * plik, int czy_do_pliku);

int main(void)
{
    parametry *p;
    dane_do_wyswietlenia *dane;
    p = (parametry *)malloc(sizeof(parametry));
    dane = (dane_do_wyswietlenia *)malloc(sizeof(dane_do_wyswietlenia));

    pobierz_dane(p);
    utworz_tablice(dane);
    generuj_sygnal(p, dane);
    wyswietl_sygnal(p, dane, NULL, 0);

    usun_tablice(dane);
    free(dane);
    free(p);

    return 0;
}

void utworz_tablice(dane_do_wyswietlenia *dane)
{
    int i = 0;
    dane->tab = (double *)malloc(sizeof(double) * dane->rozmiar_tablicy); //rozmiar tablicy częstotliwość próbkowania * czas
}

void usun_tablice(dane_do_wyswietlenia *dane)
{
    free(dane->tab);
}

void wyswietl_sygnal(parametry *p, dane_do_wyswietlenia *dane, FILE * plik, int czy_do_pliku)
{
    int i;
    printf("Podaj czas generowania sygnału [s]:");
    scanf("%d", &dane->czas);
    dane->rozmiar_tablicy = p->fp * (double)dane->czas;

    for (i = 0; i < dane->rozmiar_tablicy; i++)
    {
        printf("%lf \n", dane->tab[i]);
    }
}

void pobierz_dane(parametry *p)
{
    #ifdef WIN32
    system("chcp 65001");
    #endif
    printf("GENEROWANIE SYGNAŁU\n--------------------\nPodaj dane\n");
    printf("Amplituda: ");
    scanf("%lf", &p->amplituda);
    printf("Częstotliwość sygnału: ");
    scanf("%lf", &p->fs);
    printf("Częstotliwość próbkowania: ");
    scanf("%lf", &p->fp);
    printf("Przesunięcie fazowe: ");
    scanf("%lf", &p->fi);

}

void generuj_sygnal(parametry *p, dane_do_wyswietlenia *dane)
{
    int i = 0;
    for (i = 0; i < dane->rozmiar_tablicy; i++)
    {
        dane->tab[i] = p->amplituda * sin((M_PI*p->fs/p->fp)*i + p->fi);
    }
}
