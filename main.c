#define _USE_MATH_DEFINES
#include "include/header.h"

int main(void)
{
    parametry *p;
    dane_do_wyswietlenia *dane;
    p = (parametry *)malloc(sizeof(parametry));
    dane = (dane_do_wyswietlenia *)malloc(sizeof(dane_do_wyswietlenia));

    ustaw_kodowanie(); //konieczne do wyświetlenia polskich znaków w Windows

    wybierz_dzialanie_powitalne(p, dane);

    usun_tablice(dane);
    free(dane);
    free(p);

    return 0;
}
