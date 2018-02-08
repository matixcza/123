#include "Item.h"

#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

Item List(string data) // funkcja zwracaj¹ca obiekt klasy przedmiot na podstawie wczytanej linii z pliku tekstowego; jako argument przekazana jest zmienn¹ ze zczytan¹ lini¹
{
	Item thing;
	string name, weight, value;
	istringstream issa(data); // wykorzystanie strumienia napisowego istringstream
	getline(issa, name, ' '); // zczytywanie nazwy przedmiotu do znaku spacji (' ')
	getline(issa, weight, ' '); // zczytywanie wagi
	getline(issa, value, '\n'); // zczytywanie wartoœci przedmiotu
	thing.name = name; // przypisanie wartoœci zmiennych do odpowiednich pól
	thing.weight = atof(weight.c_str()); // konwersja na double; c_str zwraca ³añcuch do odczytu
	thing.value = atof(value.c_str());

	return thing;
}