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
		Chromosome obiekt_1, obiekt_2; // utworzenie dw�ch obiekt�w klasy Chromoson

		srand(time(0));

		int los; // zmienna los przechowywa� b�dzie wylosowan� liczb�
		int wspl_krzyzowania = 0.5 * najlepsze.size(); // wsp�czynnik krzyzowania ustawiony jako 0.5; 

		if (wspl_krzyzowania % 2 == 0) // sprawdzenie czy liczba jest parzysta
		{
			for (int i = 0; i < najlepsze.size(); i++)
			{
				if (i < wspl_krzyzowania) // kopiowanie najlepszych osobnik�w (po�owa) a druga po�owa otrzymana z krzyzowania
					krzyzowanie.push_back(najlepsze[i]);
				else // po skopiowaniu po�owy najlepszych - krzy�owanie
				{
					for (int j = 0; j < spis_rzeczy.size(); j++) // po kolejnych genach
					{
						los = rand() % 10; // losowanie 
						if (los < 5)
						{
							obiekt_1.geny[j] = najlepsze[i - wspl_krzyzowania].geny[j];     // poprzez krzyzowanie tworzenie 2 nowych osobnik�w. W zale�no�ci od wylosowanej
							obiekt_2.geny[j] = najlepsze[i + 1 - wspl_krzyzowania].geny[j]; // liczby raz kopiujemy gen z danego, a raz z nastepnego rozwiazania (z puli najlepszych)
						}
						else
						{
							obiekt_1.geny[j] = najlepsze[i + 1 - wspl_krzyzowania].geny[j];
							obiekt_2.geny[j] = najlepsze[i - wspl_krzyzowania].geny[j];
						}
					}

					krzyzowanie.push_back(obiekt_1); // wrzucenie otrzymanych osobnik�w do wektora
					krzyzowanie.push_back(obiekt_2);
					i++;
				}
			}
		}
		else // przypadek nieparzysty
		{
			for (int i = 0; i < najlepsze.size(); i++)
			{
				if (i < wspl_krzyzowania - 1) // kopiowanie najlepszych osobnik�w
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

		int los, nr_genu; // zmienna los przechowuje wylosowan� liczb�; zmienna ktory_gen odpoiwada za numer genu(przedmiotu) do zmiany

		for (int i = 0; i < najlepsze.size(); i++)
		{
			los = rand() % 1000;
			if (prwd_mutacji * 1000 >= los) // do mutacji dochodzi tylko, je�li podane  prawdopodbie�stwo po przemno�eniu jest wi�ksze od wylosowanej liczby
			{
				nr_genu = rand() % spis_rzeczy.size(); // losujemy z zakresu do rozmiaru wektora spis_rzeczy
				if (najlepsze[i].geny[nr_genu] == '1') // mutacja polega na zmianie genu (obecno�ci danego przemiotu) z 0 na 1 lub odwrotnie
					najlepsze[i].geny[nr_genu] = '0';
				else
					najlepsze[i].geny[nr_genu] = '1';
			}
		}

		return najlepsze;
	}



	// NAJLEPSZE ROZWIAZANIE 
	bool porownanie(const pair<int, int>& pierwszy, const pair<int, int>& drugi) // b�dzie s�u�y� jako komparator do sortowania
	{
		return pierwszy.first > drugi.first;
	}

	virtual vector <Chromosome> Fitness(vector <Chromosome> spis_chromosomow, vector <Item> spis_rzeczy, double waga_plecaka)
	{
		vector <Chromosome> najlepszy; // utworzenie wektora b�dzie przechowywa� osobniki w kolejno�ci od najlepszego
		vector <pair<int, int>> ocena; // utworzenie wektora do obliczenia wartosci poszczegolnych osobnikow

		int wartosc_plecaka = 0; // zmienne przechowuj�ce kolejno aktualn� warto�� i wag� plecaka
		double waga = 0;

		for (int i = 0; i < spis_chromosomow.size(); i++) // po osobnikach
		{
			for (int j = 0; j < spis_rzeczy.size(); j++) // po kazdym genie
			{
				if (spis_chromosomow[i].geny[j] == '1') // sprawdzenie, czy przedmiot jest w plecaku
				{
					wartosc_plecaka += spis_rzeczy[j].value; // zwi�kszanie aktualnej wartosci plecaka o warto�� przedmiotu
					waga += spis_rzeczy[j].weight; // zwi�kszanie aktualnej wagi plecaka o wag� przedmiotu
				}
			}
			if (waga > waga_plecaka) // jesli waga jest przekroczona
				ocena.push_back(make_pair(0 - wartosc_plecaka, i)); // dodajemy wynik do spisu oceny, pierwsza warto�� ujemna w celu wykluczenia z populacji
			else
				ocena.push_back(make_pair(wartosc_plecaka, i)); // dodajemy wynik do spisu oceny

			waga = 0;
			wartosc_plecaka = 0;
		}

		sort(ocena.begin(), ocena.end(), porownanie); // posortowanie uzyskanych wynik�w malejaco z  wykorzystaniem komparatora

		for (int i = 0; i < ocena.size(); i++) // p�tla po osobnikach
		{
			najlepszy.push_back(spis_chromosomow[ocena[i].second]); // wrzucenie do wektora najlepszych osobnik�w na podstawie posortowanego wektora oceny; druga warto�� pary to indeks 
		}

		return najlepszy;
	}



	// POKOLENIE POCZATKOWE 
	virtual vector <Chromosome> Generation(int rozmiar_populacji, int waga_plecaka, vector <Item> spis_rzeczy)
	{
		double aktualna_waga_plecaka = 0; //zmienna przechowuj�ca aktualn� wag� plecaka
		int los; // przechowywa� b�dzie wylosowan� liczb�
		vector <Chromosome> spis_chromosomow; // tworzenie wektora spis_chromosonow do kt�rego b�d� dodawane kolejne osobniki pocz�tkowe
		Chromosome chromosomy;

		int odliczanie = 0; // zlicza wybory przedmiot�w

		srand(time(0)); // ustawienie do losowania liczby pseudolosowej

		for (int i = 0; i < rozmiar_populacji; i++) // po wielko�ci populacji
		{
			for (int j = 0; j < spis_rzeczy.size(); j++) // ustawienie wszystkich przedmiotow na niewzi�te, czyli dany gen na "0" (resetowanie)
				chromosomy.geny[j] = '0';

			do
			{
				do
				{
					los = rand() % spis_rzeczy.size(); // losowanie przedmiotu ze spisu rzeczy
					if (chromosomy.geny[los] == '0' && aktualna_waga_plecaka + spis_rzeczy[los].weight <= waga_plecaka) // gdy przedmiot niewzi�ty oraz waga nie przekroczy maksymalnej
					{
						chromosomy.geny[los] = '1'; // ustawienie przedmiotu na wzi�ty (zmiana danego genu na "1")
						aktualna_waga_plecaka += spis_rzeczy[los].weight; // uaktualnienie wagi plecaka o wag� wzi�tego przedmiotu
					}
					else
						break; // przerwanie petli
				} while (true); // petla nieskonczona, przerywa si� w momencie gdy nie uda si� wylosowa� pasuj�cego przedmiotu

				odliczanie++;
			} while (odliczanie < spis_rzeczy.size() && aktualna_waga_plecaka < waga_plecaka);

			spis_chromosomow.push_back(chromosomy); // dodanie chromosomu(osobnika) do spisu populacji pocz�tkowej
			odliczanie = 0; //zerowanie zmiennych
			aktualna_waga_plecaka = 0;
		}

		return spis_chromosomow;
	}
};
