#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "wektor.h"

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
    int pozycja;
    int czy_zaszumiony;
    int czy_odfiltrowany;

    //obsługa plików
    FILE *plik;
    char nazwa_pliku[MAX_FILE_LENGHT];
    struct tm *data;
} dane_do_wyswietlenia;

void init_dane_do_wyswietlenia(dane_do_wyswietlenia *dane);
//-----------------------------------------------------------------------
void push(dane_do_wyswietlenia *dane, double t); //wrzucam t na koniec tablicy
double pop(dane_do_wyswietlenia *dane); //odczytuje i usuwa ostatni element
int size(dane_do_wyswietlenia *dane); //zwraca ilość elementów (pozycję)
double at(dane_do_wyswietlenia *dane, int pos); //zwróć element na danej pozycji
//-----------------------------------------------------------------------

void wybierz_dzialanie_powitalne(parametry *p, dane_do_wyswietlenia *dane);
void wybierz_dzialanie_sygnal(parametry *p, dane_do_wyswietlenia *dane);
void wiadomosc_powitalna();
void wiadomosc_dzialanie_sygnal();
void pobierz_czas_generowania(parametry *p, dane_do_wyswietlenia *dane);
void usun_tablice(dane_do_wyswietlenia *dane);
void generuj_sygnal(parametry *p, dane_do_wyswietlenia *dane);
int filtruj_sygnal(parametry *p, dane_do_wyswietlenia *dane);
void pobierz_dane(parametry *p);
void wyswietl_sygnal(parametry *p, dane_do_wyswietlenia *dane);
void wygeneruj_date(char *dzien_miesaca, char *miesiac, char *rok, dane_do_wyswietlenia *dane);
int zaszum_sygnal(parametry *p, dane_do_wyswietlenia *dane);
int zapisz_sygnal(parametry *p, dane_do_wyswietlenia *dane);
int odczytaj_sygnal(parametry *p, dane_do_wyswietlenia *dane); //0 - przy poprawnym odczytaniu
/*sprawdza czy dana linia jest komentarzem, jeśli tak przechodzi do nowej linii.
 *Jeśli nie wraca na dawną pozycję.
 *W przypadku błędu zwraca wartość różną od 0*/
int sprawdz_czy_komentarz(dane_do_wyswietlenia *dane);

void wybierz_dzialanie_sygnal(parametry *p, dane_do_wyswietlenia *dane)
{
    pobierz_czas_generowania(p, dane);
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
        case '4':
            if (dane->czy_odfiltrowany)
                printf("Sygnał został już odfiltrowany. Niedozwolona operacja.");
            else
            {
                dane->czy_odfiltrowany = filtruj_sygnal(p, dane);
                wyswietl_sygnal(p, dane);
            }
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
            if (!odczytaj_sygnal(p, dane))
                wybierz_dzialanie_sygnal(p, dane);
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
    time_t *tp = malloc (sizeof(time_t));
    time(tp);

    printf("\n-------------------\nGENEROWANIE SYGNAŁU\n-------------------\n");
    printf("%s\n\n", ctime(tp));
    printf("Wybierz:\n"
           "1 - Wygeneruj sygnał\n"
           "2 - Wyświetl sygnał z pliku\n"
           "0 - Wyjdź\n");
    free(tp);
}

int odczytaj_sygnal(parametry *p, dane_do_wyswietlenia *dane)
{
    printf("Podaj nazwę pliku z którego odczytać sygnał (maksymalnie %d znaków):\n", MAX_FILE_LENGHT);
    while (getchar() != '\n') {}
    fgets(dane->nazwa_pliku, MAX_FILE_LENGHT, stdin);
    *(strchr(dane->nazwa_pliku, '\n')) = '\0';
    printf("Otwieranie pliku... %s ", dane->nazwa_pliku);
    //otwarcie podanego pliku w trybie do odczytu

    if((dane->plik = fopen(dane->nazwa_pliku, "r")) == NULL)
    {
        perror("Nie udało się otworzyć podanego pliku\n");
        return 1;
    }
    else
        printf("OK\n");
    printf("Odczytano dane:\n");
    /*LINIA 1 - data
     *LINIA 2 - amplituda
     *LINIA 3 - częstotliwość sygnału
     *LINIA 4 - częstotliwość próbkowania
     *LINIA 5 - przesunięcie
     *# - komentarz, linia pomijalna*/
    char ctemp;

    //odczyt daty
    if (sprawdz_czy_komentarz(dane)) // różna od 0 to błąd
        return;
    printf("Data wygenerowania sygnału:\t");
    while ((ctemp = fgetc(dane->plik)) != '\n')
        printf("%c", ctemp);
    //odczyt amplitudy
    if (sprawdz_czy_komentarz(dane)) // różna od 0 to błąd
        return;
    printf("\nAmplituda:\t\t\t");
    while(fgetc(dane->plik) != '\t') {} //usunięcie z pliku
    fscanf(dane->plik, "%lf", &p->amplituda);
    printf("%.4f", p->amplituda);
    //odczyt częstotliwości sygnału
    if (sprawdz_czy_komentarz(dane)) // różna od 0 to błąd
        return;
    printf("\nCzęstotliwość sygnału:\t\t");
    while(fgetc(dane->plik) != '\t') {} //usunięcie z pliku
    fscanf(dane->plik, "%lf", &p->fs);
    printf("%.4f", p->fs);
    //odczyt częstotliwości próbkowania
    if (sprawdz_czy_komentarz(dane)) // różna od 0 to błąd
        return;
    printf("\nCzęstotliwość próbkowania:\t");
    while(fgetc(dane->plik) != '\t') {} //usunięcie z pliku
    fscanf(dane->plik, "%lf", &p->fp);
    printf("%.4f", p->fp);
    //odczyt przesunięcia fazowego
    if (sprawdz_czy_komentarz(dane)) // różna od 0 to błąd
        return;
    printf("\nPrzesunięcie fazowe:\t\t");
    while(fgetc(dane->plik) != '\t') {} //usunięcie z pliku
    fscanf(dane->plik, "%lf", &p->fi);
    printf("%.4f\n", p->fi);

    fclose(dane->plik);

    return 0;
}

int sprawdz_czy_komentarz(dane_do_wyswietlenia *dane)
{
    while (fgetc(dane->plik) == '#')
    {
        while(fgetc(dane->plik) != '\n') {} // żeby przeszkoczyć całą linię
    }
    if (!fseek(dane->plik, ftell(dane->plik) - 1, SEEK_SET))
        return 0;
    else
    {
        printf("Błąd odczyt z pliku\n");
        return 1;
    }
}

int zapisz_sygnal(parametry *p, dane_do_wyswietlenia *dane)
{
    char *dzien_miesiaca = malloc(sizeof(char) * 3);
    char *miesiac = malloc(sizeof(char) * 3);
    char *rok = malloc(sizeof(char) * 5);

    printf("Podaj nazwę pliku do którego zapisać sygnał (maksymalnie %d znaków):\n", MAX_FILE_LENGHT);
    while (getchar() != '\n') {}

    fgets(dane->nazwa_pliku, MAX_FILE_LENGHT, stdin);
    *(strchr(dane->nazwa_pliku, '\n')) = '\0';

    if ((dane->plik = fopen(dane->nazwa_pliku, "w")) == NULL)
    {
        fprintf(stderr, "Nie udało się utworzyć podanego pliku\n"); //zamiast perror, żeby sprawdzić jak działa
        return 1;
    }
    wygeneruj_date(dzien_miesiaca, miesiac, rok, dane);

    fprintf(dane->plik, "# Sygnał wygenerowano dnia:\n%s.%s.%s\n"
            "amplituda:\t%.4f\n"
            "czest. sygn.:\t%.4f\n"
            "czest. prob.:\t%.4f\n"
            "przesuniecie:\t%.4f\n",
            dzien_miesiaca, miesiac, rok, p->amplituda, p->fs, p->fp, p->fi);
    fclose(dane->plik);
    if (ferror(dane->plik))
    {
        fprintf(stderr, "Błąd zapisu do pliku\n");
        return 2;
    }
    printf("Zapisano do pliku: %s", dane->nazwa_pliku);

    free(rok);
    free(miesiac);
    free(dzien_miesiaca);
    return 0;
}

void wygeneruj_date(char *dzien_miesiaca, char *miesiac, char *rok, dane_do_wyswietlenia *dane)
{
    time_t *tp = malloc(sizeof(time_t));
    time(tp); //wygenerowanie czasu
    dane->data = localtime(tp); //przekopiowanie do struktury tm

    strftime(dzien_miesiaca, 3, "%d", dane->data);
    strftime(miesiac, 3, "%m", dane->data);
    strftime(rok, 5, "%Y", dane->data);

    free(tp);
}

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

    procent /= 100.; //później procent używany jako tymczasowa zmienna

    temp = p->amplituda * procent;
    for (dane->pozycja = 0; dane->pozycja < dane->rozmiar_tablicy; dane->pozycja++)
    {
        procent = at(dane, dane->pozycja);
        push(dane, procent + temp / 2. - (double)((rand() % (int)(temp*1000))/1000.));
    }
    return 1;
}

int filtruj_sygnal(parametry *p, dane_do_wyswietlenia *dane) //do przepisania
{
    int wsp, miejsce, i, j; //współczynnik uśredniania
    printf("Filtrowanie sygnału\nZ ilu próbek uśrednić: ");
    scanf ("%d", &wsp);
    printf("Na którym miejscu wstawiać uśrednioną wartość: ");
    scanf("%d", &miejsce);
    if (miejsce < 1 || miejsce > wsp)
    {
        printf("Musisz podać z przedziału od 1 do %d\n", wsp);
        return 0;
    }
    miejsce--; //ponieważ użytkownik podaje od 1 a tablica liczona jest od 0
    int ilosc_danych = size(dane);

    for (i = ilosc_danych - 1; i >= 0; i--) //zerowanie miejsca
        pop(dane);

    double temp = 0;

    for (i = 0 ; i < miejsce ; i++) //pętla do wstawiania nietypowych wartości na początku
    {
        temp = 0;
        for (j = 0; j < wsp; j++)
            temp += at(dane, j);
        temp /= (double)wsp;
        push(dane, temp);
    }
    //pętla główna
    for (i; i < ilosc_danych - wsp + miejsce; i++)
    {
        temp = 0;
        for (j = i - miejsce; j < i - miejsce + wsp; j++)
            temp += at(dane, j);
        temp /= (double)wsp;
        push(dane, temp);
    }
    //pętla końcowa
    for (i ; i < ilosc_danych ; i++)
    {
        temp = 0;
        for (j = ilosc_danych - wsp; j < ilosc_danych; j++)
            temp += at(dane, j);
        temp /= (double)wsp;
        push(dane, temp);
    }

    return 1;
}

void pobierz_czas_generowania(parametry *p, dane_do_wyswietlenia *dane)
{
    printf("Podaj czas generowania sygnału [s]:");
    scanf("%lf", &dane->czas);
    dane->data = malloc(sizeof(struct tm));
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
    if (!dane->czy_odfiltrowany)
        dane->rozmiar_tablicy = p->fp * (double)dane->czas;

    for (i = 0; i < dane->rozmiar_tablicy; i++)
    {
        printf("%.4f \n", at(dane, i));
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
    int ile = dane->czas * p->fp;
    for (i = 0; i < ile; i++)
    {
        push(dane,p->amplituda * sin((2*M_PI*p->fs/p->fp)*i + p->fi));
    }
}

#endif // HEADER_H
