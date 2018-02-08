#include "algorithm.h"
#include "chromosome.h"
#include "Item.h"
#include "genetic.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <algorithm>

using namespace std;

int main(int argc, char * argv[])
{
	int rozmiar_populacji, liczba_pokolen; // zmienne pobierane od u¿ytkownika
	double waga_plecaka, prwd_mutacji;
	bool blad = false;
	bool pobierz_z_lini_polecen = false;
	string prwd_mutacji_tekst;
	genetic temp;


	do // pêtla powoduje wykonywanie programu od pocz¹tku jeœli podano b³êdne dane
	{

		try
		{
			if (argc == 5) // sprawdzenie czy w wierszu poleceñ podano prawid³ow¹ liczbê zmiennych 
				pobierz_z_lini_polecen = true;

			if (!pobierz_z_lini_polecen) { // jeœli w wierszu poleceñ nie podano w³aœciwych zmiennych wymuszam podanie osobno
				cout << "Podaj pojemnosc plecaka:\n"; // pobranie od u¿ytkownika niezbêdnych zmiennych
				cin >> waga_plecaka;
			}
			else {
				waga_plecaka = atof(argv[1]);
			}

			if (waga_plecaka <= 0) // waga nie mo¿e byæ ujemna
			{
				string x = "Nieprawidlowa waga plecaka.";
				blad = true;
				throw x;
			}

			if (!pobierz_z_lini_polecen) {
				cout << "Podaj rozmiar poczatkowy populacji:\n";
				cin >> rozmiar_populacji;
			}
			else {
				rozmiar_populacji = atoi(argv[2]);
			}
			if (rozmiar_populacji < 2) // wielkoœæ populacji nie mo¿e byæ mniejsza od 2 
			{
				string x = "Nieprawidlowy rozmiar populacji poczatkowej (conajmniej 2 osobniki).";
				blad = true;
				throw x;
			}

			if (!pobierz_z_lini_polecen) {
				cout << "Podaj prawdopodobienstwo mutacji (od 0 do 1): \n";
				cin >> prwd_mutacji_tekst;
			}
			else {
				prwd_mutacji_tekst = argv[3];
			}

			replace(prwd_mutacji_tekst.begin(), prwd_mutacji_tekst.end(), ',', '.'); // zamiana w stringu przecinek na kropke
			prwd_mutacji = stod(prwd_mutacji_tekst); // zamiana string na double
			if (prwd_mutacji < 0 || prwd_mutacji > 1) // wartoœæ prawdopodobieñstwa mutacji w zakresie 0-1
			{
				string x = "Nieprawidlowa wartosc prawdopodobienstwa mutacji.";
				blad = true;
				throw x;
			}

			if (!pobierz_z_lini_polecen) {
				cout << "Podaj liczbe pokolen: \n";
				cin >> liczba_pokolen;
			}
			else {
				liczba_pokolen = atoi(argv[4]);
			}


			if (liczba_pokolen < 1) // liczba pokoleñ nie mniejsza ni¿ 1
			{
				string x = "Nieprawidlowa liczba pokolen. (conajmniej 1)";
				blad = true;
				throw x;
			}

			fstream plik;
			string dana, nazwa, waga, wartosc;
			vector <Item> spis_rzeczy; // tworzenie wektora przechowuj¹cego spis przedmiotow
			Item rzecz;

			plik.open("dane.txt"); // otwarcie pliku tekstowego
			if (!plik.good()) // sprawdzenie poprawnoœci otwarcia pliku
			{
				cout << "Blad otwarcia pliku." << endl;
				system("pause");
				return 0;
			}
			else
			{
				while (getline(plik, dana)) // odczyt linia po lini; linia pobierana z pliku przechowywana jest w zmiennej "dana"
				{
					rzecz = List(dana);
					spis_rzeczy.push_back(rzecz); // dodanie zczytanego przedmiotu do spisu
				}
				plik.close(); // zamkniecie pliku
			}

			// Tworzenie pokolenia pocz¹tkowego
			vector <Chromosome> spis_chromosomow; // wektor spis_chromosonow przechowuj¹cy pokolenie pocz¹tkowe

			spis_chromosomow = temp.Generation(rozmiar_populacji, waga_plecaka, spis_rzeczy);

			vector <Chromosome> najlepsze_rozwiazanie; // przechowuje kolejnoœæ osobników z najwy¿sz¹ wartoœci¹ przedmiotów
			vector <Chromosome> po_krzyzowaniu; // przechowuje osobniki po krzy¿owaniu
			vector <Chromosome> mutacja; // przechowuje osobniki po mutacji

			for (int i = 0; i < liczba_pokolen; i++) // po ilczbie pokolen (liczba wykonañ pêtli)
			{
				najlepsze_rozwiazanie = temp.Fitness(spis_chromosomow, spis_rzeczy, waga_plecaka); // dokonujemy oceny (sortowanie)
				po_krzyzowaniu = temp.Crossover(najlepsze_rozwiazanie, spis_rzeczy); // krzyzowanie
				mutacja = temp.Mutation(najlepsze_rozwiazanie, spis_rzeczy, prwd_mutacji); // mutacja
			}
			najlepsze_rozwiazanie = temp.Fitness(spis_chromosomow, spis_rzeczy, waga_plecaka); // ostatnia ocena osobników po ostatnim pokoleniu

			double wartosc_najlepsza = 0;
			double waga_najlepsza = 0;

			//Wyœwietlenie najlepszego osobnika
			cout << endl << "Najlepsze rozwiazanie:" << endl;
			for (int j = 0; j < spis_rzeczy.size(); j++)
			{
				if (najlepsze_rozwiazanie[0].geny[j] == '1') // gdy przedmiot wziety (dany gen == 1) to dodajemy jego wagê i watoœæ
				{
					wartosc_najlepsza += spis_rzeczy[j].value;
					waga_najlepsza += spis_rzeczy[j].weight;
				}

				cout << najlepsze_rozwiazanie[0].geny[j]; // wypisanie bitu oznaczaj¹cego obecnoœæ danego genu(przedmiotu) w rozwi¹zaniu

			}

			cout << endl;

			//Wyœwietlenie wziêtych wartosci i wag
			for (int j = 0; j < spis_rzeczy.size(); j++)
			{
				if (najlepsze_rozwiazanie[0].geny[j] == '1')
				{
					cout << spis_rzeczy[j].weight << " " << spis_rzeczy[j].value << endl;
				}
			}

			cout << endl;
			cout << "Waga: " << waga_najlepsza << " " << "Wartosc: " << wartosc_najlepsza << endl; // wypisanie wartoœci i wagi najlepszego spakowania plecaka

		}
		catch (string x)
		{
			cout << "Error: " << x << endl;
		}

	} while (blad == true);


	system("pause");

	return 0;

}
