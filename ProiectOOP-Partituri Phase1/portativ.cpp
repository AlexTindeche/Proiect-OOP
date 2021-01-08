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
 * - constructor & destructor (!! atentie la alocarile dinamice) (copiere, atribuire)
 * 
 * - get numar de masuri
 * - get numar note
 * - get numar elemente
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
	masura{ m },
	cheie{ c },
{
	vector<Element*> elemente();
	vector<int> masuri();
};

Portativ::Portativ(const Portativ& p)
{
	cheie = p.cheie;
	masura = p.masura;

	// eliberam memoria elementelor anterioare
	for (int i = 0; i < elemente.size(); i++)
		delete elemente[i];
	elemente.empty();

	// duplicam vectorul de elemente din obiectul sursa
	for (int i = 0; i < p.elemente.size(); i++)
	{
		Element *src = p.elemente[i];
		Element *e = new Element(src->get_tip_element(), src->get_durata());
		elemente.push_back(e);
	}

	// actualizam vectorul de masuri
	masuri.empty();
	for (int mas : p.masuri) {
		masuri.push_back(mas);
	}
}

//TODO: vazut daca merge chiar asa
void Portativ::operator= (const Portativ& p)
{
	cheie = p.cheie;
	masura = p.masura;

	// eliberam memoria elementelor anterioare
	for (int i = 0; i < elemente.size(); i++)
		delete elemente[i];
	elemente.empty();

	// duplicam vectorul de elemente din obiectul sursa
	for (int i = 0; i < p.elemente.size(); i++)
	{
		Element* src = p.elemente[i];
		Element* e = new Element(src->get_tip_element(), src->get_durata());
		elemente.push_back(e);
	}

	// actualizam vectorul de masuri
	masuri.empty();
	for (int mas : p.masuri) {
		masuri.push_back(mas);
	}
}

Portativ::~Portativ()
{
	for (int i = 0; i < elemente.size(); i++)
		delete elemente[i];
}