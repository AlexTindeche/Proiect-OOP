#pragma once

#include "durata.h"

#include <vector>
#include <string>
using namespace std;

enum class TipElement {
	NOTA,
	PAUZA,
};

enum class Inaltime {
	DO,
	RE,
	MI,
	FA,
	SOL,
	LA,
	SI
};

const vector<wstring> nume_inaltimi = { L"Do", L"Re", L"Mi", L"Fa", L"Sol", L"La", L"Si" };

class Element
{
	TipElement tip;
	Durata d;

public:
	Element(TipElement t, Durata d);

	Durata get_durata();
	TipElement get_tip_element();
};

class Pauza : public Element
{
public:
	Pauza(Durata d);
};

class Nota : public Element
{
	Inaltime inaltime;
	int octava;

public:
	Nota(Durata d, Inaltime i, int o);

	Inaltime get_inaltime();
	int get_octava();
};