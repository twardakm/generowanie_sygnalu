#define _USE_MATH_DEFINES

#include "include/wektor.h"
#include "include/header.h"

int main(void)
{
    parametry *p;
    dane_do_wyswietlenia *dane;
    p = (parametry *)malloc(sizeof(parametry));
    dane = (dane_do_wyswietlenia *)malloc(sizeof(dane_do_wyswietlenia));

    srand (time(NULL)); //inicjalizacja pseudolosowania

    ustaw_kodowanie(); //konieczne do wyświetlenia polskich znaków w Windows
    init_dane_do_wyswietlenia(dane); //inicjalizuje dynamiczną tablicę

    wybierz_dzialanie_powitalne(p, dane);

    usun_tablice(dane);
    free(dane);
    free(p);

    return 0;
}
