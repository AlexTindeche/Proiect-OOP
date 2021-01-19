#pragma once

#include "durata.h"
#include <string>
#include <vector>
using namespace std;

class Durate
{
public:
	static const Durata NOTA_INTREAGA;
	static const Durata DOIME;
	static const Durata PATRIME;
	static const Durata OPTIME;
	static const Durata SAISPREZECIME;

	static const vector<wstring> nume_durate;
	static int GetPos(Durata d);
	static Durata GetDurata(int pos);
};