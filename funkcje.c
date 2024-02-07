// Projekt nr. 8 Gra Master Mind
// Autorzy: Aleksandra Talaga, Jakub P¹czek

#include<stdio.h>  // biblioteka dodaje funkcje wejscia/wyjscia
#include<time.h>   // dodaje funkcje time(), ktora zwraca aktualny czas
#include<stdlib.h> // dodaje funkcje srand(), rand() ktore posluza do generowania liczb pseudolosowych
                   // oraz dodaje funkcje system()
#include <string.h>  // dodaje funkcje getchar()


/*Definicja makr -  kodow kolorów dla wyswietlania tekstu w konsoli w roznych kolorach.
Pozniej uzywamy ich w funkcji printf(), gdzie przy wywolaniu zostanie wstawiona odpowiadajaca im sekwencja znakow.
Spowoduje to wyswietlanie tekstu w konsoli w odpowiednich kolorach*/
#define CZERWONY     "\x1b[31m"    // sekwencje znakw reprezentujace odpowiedni kolor
#define ZIELONY   "\x1b[32m"
#define ZOLTY  "\x1b[33m"
#define GRANATOWY   "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define NIEBIESKI    "\x1b[36m"
#define BIALY   "\x1b[37m"
#define CZARNY   "\x1b[90m"

#define LICZBA_PIONK0W 4  // zdefiniowanie stalej, ktora jest liczba pionkow

// funkcja, ktora wyswietla zasady gry
void wyswietl_zasady_gry() {
	printf("=== ZASADY GRY MASTERMIND ===\n"
		"Mastermind to gra logiczna, w ktorej gracz ma za zadanie odgadnac ukryty kod skladajacy sie z kolorowych pionkow.\n"
		"Gracz moze wybrac poziom gry, czyli liczbe kolorow, z ktorych sklada sie kod.\n"
		"Na poczatku gry losowany jest ukryty kod, a gracz ma 10 prob na jego odgadniecie.\n"
		"Po kazdej próbie gracz otrzymuje podpowiedzi w postaci czarnych i bialych szpilek:\n"
		"- Czarna szpilka oznacza poprawny kolor na poprawnej pozycji.\n"
		"- Biala szpilka oznacza poprawny kolor, ale na zlej pozycji.\n"
		"Celem gracza jest odgadniecie ukrytego kodu maksymalnie w 10 probach\n"
		"Powodzenia!\n\n"
		"Aby przejsc do rozgrywki, nacisnij ENTER");
	while (getchar() != '\n');  // petla zakonczy swoje dzialanie po otrzymaniu znaku nowej linii, czyli po wcisnieciu klawisza enter
}

// funkcja ktora sluzy do wyczyszczenia ekranu w konsoli
void wyczyszczenie_konsoli()
{
	system("cls");    // funckcja system() z biblioteki stdlib.h
				     // umozliwia wywolanie polecen systemowych z poziomu programu
					// system("cls") dziala dla systemu Windows
}

// funkcja zwraca numer poziomu wybranego przez gracza lub 0, gdy wybrany poziom jest bledny
// gracz wybiera poziom, czyli to z ilu kolorów bêdzie zgadywa³
int wybor_poziomu() {
	int n;
	printf("Wybierz z ilu kolorow chcesz zgadywac (4/5/6): ");
	if ((scanf_s("%d", &n) < 1 || (n != 4 && n != 5 && n != 6))) {   // poziom moze byc tylko liczba 4, 5 lub 6
		return 0;
	}
	return n;
}

// funkcja która drukuje oznaczenia kolorów
void oznaczenia_kolorow(n) {
	printf("Oznaczenia kolorow:\n");
	printf("Czerwony - 0\n");
	printf("Zielony - 1\n");
	printf("Zolty - 2\n");
	printf("Granatowy - 3\n");
	if (n == 5) { printf("Magenta - 4\n"); }
	if (n == 6) { printf("Magenta - 4\n"); printf("Niebieski - 5\n"); }
}

/* Funkcja, ktora wyswietla kolorowe pionki w zaleznosci od liczb znajdujacych sie w tablicy zgadywane.
Jako parametr przyjmuje czteroelementowa tablice , w ktorej sa liczby podawane przez gracza.
Liczby z tablicy w grze sa reprezentowane przez kolory. */
void kolorowe_drukowanie(int zgadywane[])
{
	for (int i = 0; i < LICZBA_PIONK0W; ++i) {
		switch (zgadywane[i]) {
		case 0: printf(CZERWONY "@ "); break;
		case 1: printf(ZIELONY "@ "); break;
		case 2: printf(ZOLTY "@ "); break;
		case 3: printf(GRANATOWY "@ "); break;
		case 4: printf(MAGENTA "@ "); break;
		case 5: printf(NIEBIESKI "@ "); break;
		}
	}
}

/* Funkcja zwraca liczbe czarnych szpilek, czyli liczbe pionkow na poprawnych pozycjach.
Jako parametry przyjmuje: tablice wylosowane z liczbami wylosowanymi,
tablice zgadywane z liczbami podawanymi przez gracza
oraz tablice poprawne do zapisywania, na ktorych pozycjach pionki sa poprawnie umieszczone. */
int ile_czarnych(int wylosowane[], int zgadywane[], int poprawne[]) {
	int czarne = 0;
	for (int j = 0; j < LICZBA_PIONK0W; ++j) {
		if (wylosowane[j] == zgadywane[j]) {
			czarne++;
			poprawne[j] = 1;
		}
	}
	return czarne;
}

/* Funkcja zwraca liczbe bialych szpilek, czyli liczbe pionkow o poprawnych kolorach, ale na niepoprawnych pozycjach.
Jako parametry funkcja przyjmuje: tablice wylosowane, tablice zgadywane oraz tablice poprawne. */
int ile_bialych(int wylosowane[], int zgadywane[], int poprawne[]) {
	int biale = 0, pomijane[LICZBA_PIONK0W] = { 0 };   // inicjalizujemy i definiujemy czteroelementowa tablice pomijane z wartosciami 0
	                                                   // sluzy do œledzenia, ktore pionki zostaly ju¿ uwzglednione przy obliczaniu liczby bialych szpilek
	for (int j = 0; j < LICZBA_PIONK0W; ++j) {
		if (poprawne[j]) { continue; }  // jesli pionek na danej pozycji jest poprawny(wartosc 1 w tablicy poprawne),
		                                // to wykonuje sie instrukcja skoku do naglowka petli
		                                // te pionki nie sa juz tutaj liczone, bo ich przypadek rozwiazuje funkcja ile_czarnych()
		for (int k = 0; k < LICZBA_PIONK0W; ++k) {
			if (zgadywane[j] == wylosowane[k] && poprawne[k] == 0 && pomijane[k] == 0) {
				pomijane[k] = 1;
				biale++;
				break;
			}
		}
	}
	return biale;
}

// funkcja sluzaca do drukowania napisow w kolorze bialym
void biale_drukowanie()
{
	printf(BIALY "  ");
}


// funkcja mastermind() odpowiada za wykonywanie wiekszosci gry
void mastermind() {
	int wylosowane[LICZBA_PIONK0W], zgadywane[LICZBA_PIONK0W];       // inicjalizacja tablic wylosowane i zgadywane
	srand((unsigned int)time(0));      // inicjalizacja generatora liczb pseudolosowych
	int poziom = wybor_poziomu();     //najpierw gracz wybiera poziom
	if (poziom == 0) {               // jesli funkcja wybor_poziomu zwrocila 0, znaczy to ze wybrany poziom byl niepoprawny,
		                            // wtedy program konczy swoje dzialanie
		printf("Niepoprawny poziom");
		return;
	}
	for (int i = 0; i < LICZBA_PIONK0W; ++i) {
		wylosowane[i] = rand() % poziom;              // losowanie i zapisywanie kodu (numerow czterech pionkow) do tablicy wylosowane
	}
	oznaczenia_kolorow(poziom);
	printf("\n");
	for (int proba = 1; proba <= 10; ++proba) {
		for (int i = 0; i < LICZBA_PIONK0W; ++i) { // wypisywanie numeru aktualnej proby
			printf("Proba nr % d: Kolor pionka nr %d: ", proba, i + 1);
			if ((scanf_s("%d", &zgadywane[i]) < 1) || (zgadywane[i] < 0) || (zgadywane[i] >(poziom - 1))) {  // gracz wpisuje numer pionka
				--i;                            // jesli wpisany znak jest niepoprawny,  daje mozliwosc wpisania ponownie
				fseek(stdin, 0, SEEK_END);
				fputs("\033[A\033[2K", stdout);    // wyczyszczenie biezacej linii w konsoli i przesuniêcie kursora na poprzednia linie
				rewind(stdout);
				printf("Niepoprawne dane, podaj ponownie.  ");
				continue;  // instrukcja skoku do naglowka petli
			}
			fputs("\033[A\033[2K", stdout);    // wyczyszczenie biezacej linii w konsoli i przesuniêcie kursora na poprzednia linie
			rewind(stdout);      // przesuwa wskaznik pozycji zapisu strumienia wyjsciowego na pocz¹tek strumienia
		}

		kolorowe_drukowanie(zgadywane);   // drukuje kolorowe pionki, ktore podaje gracz
		biale_drukowanie();     // przywraca drukowanie na kolor bialy
		int poprawne[LICZBA_PIONK0W] = { 0 };   // inicjalizacja i definicja tablicy poprawne z zerami
		int czarne = ile_czarnych(wylosowane, zgadywane, poprawne);  // inicjalizacja zmiennej czarne
		// i przypisanie do niej wartosci, ktora zwraca funkcja ile_czarnych
		int biale = ile_bialych(wylosowane, zgadywane, poprawne);  // inicjalizacja zmiennej biale
		// i przypisanie do niej wartosci, ktora zwraca funkcja ile_bialych

		if (czarne == 4) {
			printf("Wygrales! Udalo ci sie w %d probach\n", proba);    // jesli cztery szpilki sa czarne,
			// znaczy to ze wszystkie pionki sa na poprawnych miejscach,
			// wiec gracz wygrywa gre
			return;
		}
		else if (proba == 10) {             // jesli gracz przekroczy 10 prob, to przegrywa gre
			printf("Przegrales! Kod to: ");
			kolorowe_drukowanie(wylosowane);
			biale_drukowanie();
			return;
		}
		else {
			printf("Podpowiedz: ");         // wyswietlanie podpowiedzi czyli bialych lub czarnych szpilek
			for (int j = 0; j < czarne; j++) {
				printf(CZARNY "@ ");
			}
			printf(BIALY);
			for (int j = 0; j < biale; j++) {
				printf(BIALY "@ ");
			}
			printf("\n");
		}
	}
}

