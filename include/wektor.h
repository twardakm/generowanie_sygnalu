#ifndef dane_do_wyswietlenia_H
#define dane_do_wyswietlenia_H

#include <stdio.h>
#include <stdlib.h>

#include "header.h"

#define MAKS_rozmiar_tablicy 15

void push(dane_do_wyswietlenia *dane, double t)
{
    if (dane->pozycja == dane->rozmiar_tablicy) //zwiększ rozmiar_tablicy tablicy
    {
        dane->rozmiar_tablicy *= 1.5;
        dane->tab = realloc(dane->tab, sizeof(double) * (dane->rozmiar_tablicy));
    }
    dane->tab[dane->pozycja++] = t;
}

double pop(dane_do_wyswietlenia *dane)
{
    return dane->tab[--dane->pozycja];
}

void init_dane_do_wyswietlenia(dane_do_wyswietlenia *dane)
{
    dane->rozmiar_tablicy = MAKS_rozmiar_tablicy;
    dane->tab = malloc(sizeof (double) * dane->rozmiar_tablicy);
    dane->pozycja = 0;

    return;
}

int size(dane_do_wyswietlenia *dane)
{
    return dane->pozycja;
}

double at(dane_do_wyswietlenia *dane, int pos)
{
    return dane->tab[pos];
}

#endif // dane_do_wyswietlenia_H
