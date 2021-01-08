#pragma once

#include "durata.h"

enum class TipElement {
	PAUZA,
	NOTA
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