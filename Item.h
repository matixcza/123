#pragma once

#include <string>

class Item
{
public:
	std::string name;
	double weight;
	double value;
};

Item List(std::string dana);
