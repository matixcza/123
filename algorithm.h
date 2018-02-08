#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "chromosome.h"
#include "Item.h"


class algorithm
{
public:
	virtual std::vector <Chromosome> Crossover(std::vector <Chromosome> najlepsze, std::vector <Item> spis_rzeczy) = 0;
	virtual std::vector <Chromosome> Mutation(std::vector <Chromosome> najlepsze, std::vector <Item> spis_rzeczy, double prwd_mutacji) = 0;
	virtual std::vector <Chromosome> Fitness(std::vector <Chromosome> spis_chromosonow, std::vector <Item> spis_rzeczy, double waga_plecaka) = 0;
	virtual std::vector <Chromosome> Generation(int rozmiar_populacji, int waga_plecaka, std::vector <Item> spis_rzeczy) = 0;

};
