#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define MAX_FILE_LENGHT 30

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
    double czas;
    int rozmiar_tablicy;
    int czy_zaszumiony;
    int czy_odfiltrowany;

    //obsługa plików
    FILE *plik;
    char nazwa_pliku[MAX_FILE_LENGHT];
} dane_do_wyswietlenia;

void wybierz_dzialanie_powitalne(parametry *p, dane_do_wyswietlenia *dane);
void wybierz_dzialanie_sygnal(parametry *p, dane_do_wyswietlenia *dane);
void wiadomosc_powitalna();
void wiadomosc_dzialanie_sygnal();
void utworz_tablice(parametry *p, dane_do_wyswietlenia *dane);
void usun_tablice(dane_do_wyswietlenia *dane);
void generuj_sygnal(parametry *p, dane_do_wyswietlenia *dane);
void pobierz_dane(parametry *p);
void wyswietl_sygnal(parametry *p, dane_do_wyswietlenia *dane);
void wygeneruj_date(char *dzien_miesaca, char *miesiac, char *rok);
int zaszum_sygnal(parametry *p, dane_do_wyswietlenia *dane);
int zapisz_sygnal(parametry *p, dane_do_wyswietlenia *dane);

void wybierz_dzialanie_sygnal(parametry *p, dane_do_wyswietlenia *dane)
{
    utworz_tablice(p, dane);
    generuj_sygnal(p, dane);

    while (getchar() != '\n') {}
    wiadomosc_dzialanie_sygnal();
    int c;

    while (c = getchar())
    {
        switch (c)
        {
        case '1':
            wyswietl_sygnal(p, dane);
            break;
        case '2':
            zapisz_sygnal(p, dane);
            break;
        case '3':  //zaszumianie sygnału
            if (dane->czy_zaszumiony)
                printf("Sygnał został już zaszumiony. Niedozwolona operacja.");
            else
                dane->czy_zaszumiony = zaszum_sygnal(p, dane);
            break;
        case '0':
            while (getchar() != '\n') {}
            wiadomosc_powitalna();
            return;
        default:
            wiadomosc_dzialanie_sygnal();
            break;
        }
    }
}

void wybierz_dzialanie_powitalne(parametry *p, dane_do_wyswietlenia *dane)
{
    wiadomosc_powitalna();
    int c;

    while (c = getchar())
    {
        switch (c)
        {
        case '1':
            pobierz_dane(p);
            wybierz_dzialanie_sygnal(p, dane);
            break;
        case '2':
            break;
        case '0':
            printf("Dziękuję za skorzystanie z programu!\nMarcin Twardak\n");
            return;
        default:
            wiadomosc_powitalna();
            break;
        }
    }
}

void wiadomosc_dzialanie_sygnal()
{
    printf("\nCo zrobić z sygnałem?\n"
           "1 - Wyświetl sygnał\n"
           "2 - Zapisz do pliku\n"
           "3 - Zaszum sygnał\n"
           "4 - Odfiltruj sygnał\n"
           "0 - Powrót\n");
}

void wiadomosc_powitalna()
{
    struct tm czas;
    printf("\n-------------------\nGENEROWANIE SYGNAŁU\n-------------------\n\n");
    printf("Wybierz:\n"
           "1 - Wygeneruj sygnał\n"
           "2 - Wyświetl sygnał z pliku\n"
           "0 - Wyjdź\n");

    printf("Generowanie daty\n");

    time_t *tp = malloc (sizeof(time_t));
    time(tp);
    /*char *dzien_miesiaca = malloc(sizeof(char) * 50);
    strftime (dzien_miesiaca, 50, "%S", czas);*/

    printf("Dzień: %s\n", ctime(tp));
}

int zapisz_sygnal(parametry *p, dane_do_wyswietlenia *dane)
{
    char dzien_miesiaca[2];
    char miesiac[2];
    char rok[4];

    printf("Podaj nazwę pliku do którego zapisać sygnał (maksymalnie %d znaków):\n", MAX_FILE_LENGHT);
    while (getchar() != '\n') {}
    int i = 0;

    fgets(dane->nazwa_pliku, MAX_FILE_LENGHT, stdin);
    *(strchr(dane->nazwa_pliku, '\n')) = '\0';

    if ((dane->plik = fopen(dane->nazwa_pliku, "w")) == NULL)
    {
        fprintf(stderr, "Nie udało się utworzyć podanego pliku\n"); //zamiast perror, żeby sprawdzić jak działa
        return 1;
    }
    //wygeneruj_date(&dzien_miesiaca, &miesiac, &rok);


    //fprintf(dane->plik, "# Sygnał wygenerowano dnia:\n%s", dzien_miesiaca);
    fclose(dane->plik);
    if (ferror(dane->plik))
    {
        fprintf(stderr, "Błąd zapisu do pliku\n");
        return 2;
    }
    printf("Zapisano do pliku: %s", dane->nazwa_pliku);
    return 0;
}

/*void wygeneruj_date(char *dzien_miesiaca, char *miesiac, char *rok)
{
    struct tm czas;
    strftime(dzien_miesiaca, 2, "%d", &czas);
}*/

int zaszum_sygnal(parametry *p, dane_do_wyswietlenia *dane)
{
    double procent, temp; //jak bardzo ma być zaszumiony sygnał
    int i;
#ifdef WIN32
    printf("Podaj w %c wartość zaszumienia: (powyżej 0)\n", 37);
#else
    printf("Podaj w % wartość zaszumienia: (powyżej 0)\n");
#endif
    scanf("%lf", &procent);
    if (procent < 0)
    {
        printf("Miało być większe od 0!\n");
        while (getchar() != '\n') {}
        return 0;
    }

    procent /= 100.;

    temp = p->amplituda * procent;
    for (i = 0; i < dane->rozmiar_tablicy; i++)
    {
        dane->tab[i] += temp / 2.;
        dane->tab[i] -= (double)((rand() % (int)(temp*1000))/1000.);
    }
    return 1;
}

void utworz_tablice(parametry *p, dane_do_wyswietlenia *dane)
{
    printf("Podaj czas generowania sygnału [s]:");
    scanf("%lf", &dane->czas);
    dane->rozmiar_tablicy = (int)(p->fp * dane->czas);
    dane->tab = ((double *)malloc(sizeof(double) * dane->rozmiar_tablicy)); //rozmiar tablicy częstotliwość próbkowania * czas
    dane->czy_odfiltrowany = 0;
    dane->czy_zaszumiony = 0;
}

void usun_tablice(dane_do_wyswietlenia *dane)
{
    free(dane->tab);
}

void wyswietl_sygnal(parametry *p, dane_do_wyswietlenia *dane)
{
    int i;
    dane->rozmiar_tablicy = p->fp * (double)dane->czas;

    for (i = 0; i < dane->rozmiar_tablicy; i++)
    {
        printf("%.4f \n", dane->tab[i]);
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
