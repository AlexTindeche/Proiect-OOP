#pragma once

#include "durata.h"
#include "durate.h"
#include "element.h"
#include <vector>
using namespace std;

enum class Cheie {SOL, FA};

class Portativ
{
	Cheie cheie;
	Durata masura;
	vector<Element*> elemente;
	vector<int> masuri;

public:
	Portativ(Durata m = Durate::NOTA_INTREAGA, Cheie c = Cheie::FA);
	Portativ(const Portativ &p);
	void operator= (const Portativ &p);
	~Portativ();
};
