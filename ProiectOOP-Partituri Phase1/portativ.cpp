#pragma once

#include "portativ.h"

/**
 * atribute
 * - lista elemente
 * - masura
 * - cheie
 * - (~) armatura (in etapa 2)
 * 
 * functii
 * x constructor & destructor (!! atentie la alocarile dinamice) (copiere, atribuire)
 * 
 * x get numar de masuri
 * x get numar note
 * x get numar elemente
 * 
 * - adaugare note la portativ (+ verificam daca se poate adauga)
 * - scoatere ultima nota
 * - inlocuire element cu altul de durata egala
 * 
 * - operatorul [] -> elementul de pe pozitia respectiva
 * - operatorul + -> adaugare element la lista
 * - operatorul -- -> stergerea ultimului element
 * 
 * - play()
*/

Portativ::Portativ(Durata m, Cheie c) :
	masura { m },
	cheie { c }
{};

Portativ::Portativ(const Portativ& p) 
{
	// ne folosim de implementarea din operatorul de atribuire
	*this = p;
}

Portativ& Portativ::operator= (const Portativ& p)
{
	// daca este acelasi obiect nu mai e nevoie de copiere
	if (this == &p) {
		return *this;
	}

	// copiere atribute statice
	cheie = p.cheie;
	masura = p.masura;
	masuri = p.masuri;

	// eliberam memoria elementelor anterioare
	destroy_array();

	// duplicam vectorul de elemente din obiectul sursa
	for (Element* e : p.elemente)
		elemente.push_back(new Element(*e));

	/*int len = p.elemente.size();
	for (int i = 0; i < len; i++)
		elemente.push_back(new Element(*(p.elemente[i])));*/

	return *this;
}

Portativ::~Portativ()
{
	destroy_array();
}

void Portativ::destroy_array()
{
	for (Element* e : elemente)
		delete e;
	elemente.empty();
}

int Portativ::get_nr_elemente()
{
	return (int) elemente.size();
}

int Portativ::get_nr_masuri()
{
	return (int) masuri.size();
}

int Portativ::get_nr_note()
{
	int contor = 0;
	for (Element* e : elemente)
		if (e->get_tip_element() == TipElement::NOTA)
			contor++;

	return contor;
}

int Portativ::get_nr_pauze()
{
	int contor = 0;
	for (Element* e : elemente)
		if (e->get_tip_element() == TipElement::PAUZA)
			contor++;

	return contor;
}

Durata Portativ::get_ultima_masura()
{
	Durata lungime;
	if (masuri.size() == 0)
		return lungime;

	for(int i = masuri[masuri.size() - 1]; i < elemente.size(); i++)
		lungime = lungime + elemente[i]->get_durata();

	return lungime;
}

bool Portativ::add_element(Element e)
{
	// aflam durata ultimei masuri
	Durata um = get_ultima_masura();
	if (um == masura)
		um = Durata();

	// verificam daca incape in masura
	Durata scadere = masura - um;
	if (e.get_durata() > masura - um)
		return false;

	// adaugam elementul
	Element* n = new Element(e);
	elemente.push_back(n);

	// daca e masura noua modificam vectorul de masuri
	if (um == Durata())
		masuri.push_back((int) elemente.size() - 1);

	return true;
}

void Portativ::remove_element()
{
	Element* e = elemente[elemente.size() - 1];
	// Element* e = elemente.back() - ?! verificat daca merge asa mai simplu
	Durata um = get_ultima_masura();
	if (e->get_durata() == um)
		masuri.pop_back();

	// sau masuri.back() == elemente.size() - 1;

	delete e;
	elemente.pop_back();
}

bool Portativ::replace_element(Element e, int pos)
{
	// verificam daca pozitia e valida si
	// verificam daca elementul dat are aceeasi durata ca cel inlocuit
	if (pos < 0 || pos >= elemente.size() || 
		elemente[pos]->get_durata() != e.get_durata())
		return false;

	// elementul nou si cel vechi
	Element* vechi = elemente[pos];
	Element* nou = new Element(e);

	// stergem elementul vechi si il punem pe cel nou
	elemente[pos] = nou;
	delete vechi;

	return true;
}

void Portativ::afisare()
{
	int mas_len = (int) masuri.size();
	int elem_len = (int) elemente.size();
	for (int i = 0; i < mas_len; i++)
	{
		cout << "-- Masura " << i + 1 << " --\n";
		int stop = elem_len;
		if (i < mas_len - 1)
			stop = masuri[i + 1];

		for (int j = masuri[i]; j < stop; j++)
			cout << elemente[j]->get_durata() << " ";

		cout << '\n';
	}
}