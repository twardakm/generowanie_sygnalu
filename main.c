#define _USE_MATH_DEFINES
#include "include/header.h"

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
