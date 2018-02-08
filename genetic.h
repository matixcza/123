#include "chromosome.h"
#include "algorithm.h"
#include "Item.h"

#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include <algorithm>

using namespace std;

class genetic : public algorithm
{
    public:
	// KRZYZOWANIE 
	virtual vector <Chromosome> Crossover(vector <Chromosome> najlepsze, vector <Item> spis_rzeczy)
	{
		vector <Chromosome> krzyzowanie; // utworzenie wektora 
		Chromosome obiekt_1, obiekt_2; // utworzenie dwóch obiektów klasy Chromoson

		srand(time(0));

		int los; // zmienna los przechowywaæ bêdzie wylosowan¹ liczbê
		int wspl_krzyzowania = 0.5 * najlepsze.size(); // wspó³czynnik krzyzowania ustawiony jako 0.5; 

		if (wspl_krzyzowania % 2 == 0) // sprawdzenie czy liczba jest parzysta
		{
			for (int i = 0; i < najlepsze.size(); i++)
			{
				if (i < wspl_krzyzowania) // kopiowanie najlepszych osobników (po³owa) a druga po³owa otrzymana z krzyzowania
					krzyzowanie.push_back(najlepsze[i]);
				else // po skopiowaniu po³owy najlepszych - krzy¿owanie
				{
					for (int j = 0; j < spis_rzeczy.size(); j++) // po kolejnych genach
					{
						los = rand() % 10; // losowanie 
						if (los < 5)
						{
							obiekt_1.geny[j] = najlepsze[i - wspl_krzyzowania].geny[j];     // poprzez krzyzowanie tworzenie 2 nowych osobników. W zale¿noœci od wylosowanej
							obiekt_2.geny[j] = najlepsze[i + 1 - wspl_krzyzowania].geny[j]; // liczby raz kopiujemy gen z danego, a raz z nastepnego rozwiazania (z puli najlepszych)
						}
						else
						{
							obiekt_1.geny[j] = najlepsze[i + 1 - wspl_krzyzowania].geny[j];
							obiekt_2.geny[j] = najlepsze[i - wspl_krzyzowania].geny[j];
						}
					}

					krzyzowanie.push_back(obiekt_1); // wrzucenie otrzymanych osobników do wektora
					krzyzowanie.push_back(obiekt_2);
					i++;
				}
			}
		}
		else // przypadek nieparzysty
		{
			for (int i = 0; i < najlepsze.size(); i++)
			{
				if (i < wspl_krzyzowania - 1) // kopiowanie najlepszych osobników
					krzyzowanie.push_back(najlepsze[i]);
				else // krzyzowanie
				{
					for (int j = 0; j < spis_rzeczy.size(); j++)
					{
						los = rand() % 10; // losowanie
						if (los < 5)
						{
							obiekt_1.geny[j] = najlepsze[i - wspl_krzyzowania + 1].geny[j];
							obiekt_2.geny[j] = najlepsze[i + 1 - wspl_krzyzowania + 1].geny[j];
						}
						else
						{
							obiekt_1.geny[j] = najlepsze[i + 1 - wspl_krzyzowania + 1].geny[j];
							obiekt_2.geny[j] = najlepsze[i - wspl_krzyzowania + 1].geny[j];
						}
					}
					krzyzowanie.push_back(obiekt_1);
					krzyzowanie.push_back(obiekt_2);
					i++;
				}
			}
		}

		return krzyzowanie;
	}



	// MUTACJA 
	virtual vector <Chromosome> Mutation(vector <Chromosome> najlepsze, vector <Item> spis_rzeczy, double prwd_mutacji)
	{
		vector <Chromosome> mutacja; // tworzenie wektora 

		srand(time(0));

		int los, nr_genu; // zmienna los przechowuje wylosowan¹ liczbê; zmienna ktory_gen odpoiwada za numer genu(przedmiotu) do zmiany

		for (int i = 0; i < najlepsze.size(); i++)
		{
			los = rand() % 1000;
			if (prwd_mutacji * 1000 >= los) // do mutacji dochodzi tylko, jeœli podane  prawdopodbieñstwo po przemno¿eniu jest wiêksze od wylosowanej liczby
			{
				nr_genu = rand() % spis_rzeczy.size(); // losujemy z zakresu do rozmiaru wektora spis_rzeczy
				if (najlepsze[i].geny[nr_genu] == '1') // mutacja polega na zmianie genu (obecnoœci danego przemiotu) z 0 na 1 lub odwrotnie
					najlepsze[i].geny[nr_genu] = '0';
				else
					najlepsze[i].geny[nr_genu] = '1';
			}
		}

		return najlepsze;
	}



	// NAJLEPSZE ROZWIAZANIE 
	bool porownanie(const pair<int, int>& pierwszy, const pair<int, int>& drugi) // bêdzie s³u¿yæ jako komparator do sortowania
	{
		return pierwszy.first > drugi.first;
	}

	virtual vector <Chromosome> Fitness(vector <Chromosome> spis_chromosomow, vector <Item> spis_rzeczy, double waga_plecaka)
	{
		vector <Chromosome> najlepszy; // utworzenie wektora bêdzie przechowywa³ osobniki w kolejnoœci od najlepszego
		vector <pair<int, int>> ocena; // utworzenie wektora do obliczenia wartosci poszczegolnych osobnikow

		int wartosc_plecaka = 0; // zmienne przechowuj¹ce kolejno aktualn¹ wartoœæ i wagê plecaka
		double waga = 0;

		for (int i = 0; i < spis_chromosomow.size(); i++) // po osobnikach
		{
			for (int j = 0; j < spis_rzeczy.size(); j++) // po kazdym genie
			{
				if (spis_chromosomow[i].geny[j] == '1') // sprawdzenie, czy przedmiot jest w plecaku
				{
					wartosc_plecaka += spis_rzeczy[j].value; // zwiêkszanie aktualnej wartosci plecaka o wartoœæ przedmiotu
					waga += spis_rzeczy[j].weight; // zwiêkszanie aktualnej wagi plecaka o wagê przedmiotu
				}
			}
			if (waga > waga_plecaka) // jesli waga jest przekroczona
				ocena.push_back(make_pair(0 - wartosc_plecaka, i)); // dodajemy wynik do spisu oceny, pierwsza wartoœæ ujemna w celu wykluczenia z populacji
			else
				ocena.push_back(make_pair(wartosc_plecaka, i)); // dodajemy wynik do spisu oceny

			waga = 0;
			wartosc_plecaka = 0;
		}

		sort(ocena.begin(), ocena.end(), porownanie); // posortowanie uzyskanych wyników malejaco z  wykorzystaniem komparatora

		for (int i = 0; i < ocena.size(); i++) // pêtla po osobnikach
		{
			najlepszy.push_back(spis_chromosomow[ocena[i].second]); // wrzucenie do wektora najlepszych osobników na podstawie posortowanego wektora oceny; druga wartoœæ pary to indeks 
		}

		return najlepszy;
	}



	// POKOLENIE POCZATKOWE 
	virtual vector <Chromosome> Generation(int rozmiar_populacji, int waga_plecaka, vector <Item> spis_rzeczy)
	{
		double aktualna_waga_plecaka = 0; //zmienna przechowuj¹ca aktualn¹ wagê plecaka
		int los; // przechowywaæ bêdzie wylosowan¹ liczbê
		vector <Chromosome> spis_chromosomow; // tworzenie wektora spis_chromosonow do którego bêd¹ dodawane kolejne osobniki pocz¹tkowe
		Chromosome chromosomy;

		int odliczanie = 0; // zlicza wybory przedmiotów

		srand(time(0)); // ustawienie do losowania liczby pseudolosowej

		for (int i = 0; i < rozmiar_populacji; i++) // po wielkoœci populacji
		{
			for (int j = 0; j < spis_rzeczy.size(); j++) // ustawienie wszystkich przedmiotow na niewziête, czyli dany gen na "0" (resetowanie)
				chromosomy.geny[j] = '0';

			do
			{
				do
				{
					los = rand() % spis_rzeczy.size(); // losowanie przedmiotu ze spisu rzeczy
					if (chromosomy.geny[los] == '0' && aktualna_waga_plecaka + spis_rzeczy[los].weight <= waga_plecaka) // gdy przedmiot niewziêty oraz waga nie przekroczy maksymalnej
					{
						chromosomy.geny[los] = '1'; // ustawienie przedmiotu na wziêty (zmiana danego genu na "1")
						aktualna_waga_plecaka += spis_rzeczy[los].weight; // uaktualnienie wagi plecaka o wagê wziêtego przedmiotu
					}
					else
						break; // przerwanie petli
				} while (true); // petla nieskonczona, przerywa siê w momencie gdy nie uda siê wylosowaæ pasuj¹cego przedmiotu

				odliczanie++;
			} while (odliczanie < spis_rzeczy.size() && aktualna_waga_plecaka < waga_plecaka);

			spis_chromosomow.push_back(chromosomy); // dodanie chromosomu(osobnika) do spisu populacji pocz¹tkowej
			odliczanie = 0; //zerowanie zmiennych
			aktualna_waga_plecaka = 0;
		}

		return spis_chromosomow;
	}
};
