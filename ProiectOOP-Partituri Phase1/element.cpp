#include "element.h"

Element::Element(TipElement t, Durata d) :
	tip{ t },
	d{ d }
{}
	
Durata Element::get_durata()
{
	return d;
}

TipElement Element::get_tip_element()
{
	return tip;
}

Pauza::Pauza(Durata d) :
	Element(TipElement::PAUZA, d)
{}

Nota::Nota(Durata d, Inaltime i, int o) :
	Element(TipElement::NOTA, d),
	inaltime{i},
	octava{o}
{}

Inaltime Nota::get_inaltime()
{
	return inaltime;
}

int Nota::get_octava()
{
	return octava;
}
