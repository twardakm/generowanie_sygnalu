#ifndef HEADER_H
#define HEADER_H

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
    int rozmiar_tablicy;
} dane_do_wyswietlenia;

int wybierz_dzialanie_powitalne(parametry *p, dane_do_wyswietlenia *dane);
void wiadomosc_powitalna();
void utworz_tablice(dane_do_wyswietlenia *dane);
void usun_tablice(dane_do_wyswietlenia *dane);
void generuj_sygnal(parametry *p, dane_do_wyswietlenia *dane);
void pobierz_dane(parametry *p);
void wyswietl_sygnal(parametry *p, dane_do_wyswietlenia *dane);

int wybierz_dzialanie_powitalne(parametry *p, dane_do_wyswietlenia *dane)
{
    wiadomosc_powitalna();
    int c;

    while (c = getchar())
    {
      switch (c)
      {
      case '1':
          pobierz_dane(p);
          utworz_tablice(dane);
          generuj_sygnal(p, dane);
          wyswietl_sygnal(p, dane);
          break;
      case '2':
          break;
      case '0':
          printf("Dziękuję za skorzystanie z programu!\nMarcin Twardak\n");
          return 0;
      default:
          wiadomosc_powitalna();
          break;
      }
    }
}

void wiadomosc_powitalna()
{
    printf("\n-------------------\nGENEROWANIE SYGNAŁU\n-------------------\n\n");
    printf("Wybierz:\n"
           "1 - Wygeneruj sygnał\n"
           "2 - Wyświetl sygnał z pliku\n"
           "0 - Wyjdź\n");
}

void utworz_tablice(dane_do_wyswietlenia *dane)
{
    int i = 0;
    dane->tab = (int)((double *)malloc(sizeof(double) * dane->rozmiar_tablicy)); //rozmiar tablicy częstotliwość próbkowania * czas
}

void usun_tablice(dane_do_wyswietlenia *dane)
{
    free(dane->tab);
}

void wyswietl_sygnal(parametry *p, dane_do_wyswietlenia *dane)
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

void ustaw_kodowanie()
{
    #ifdef WIN32
    system("chcp 65001");
    #endif
}

void pobierz_dane(parametry *p)
{
    printf("Podaj dane\n");
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

#endif // HEADER_H
