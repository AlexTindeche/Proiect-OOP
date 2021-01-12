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

	void destroy_array();

public:
	/**
	 * Constructor
	 * @param m masura portativului
	 * @param c cheia portativului
	*/
	Portativ(Durata m = Durate::NOTA_INTREAGA, Cheie c = Cheie::SOL);

	/**
	 * Constructor de copiere
	 * @param p 
	*/
	Portativ(const Portativ &p);

	/**
	 * Destructor
	*/
	~Portativ();

	/**
	 * Suprascriere operator de atribuire
	 * @param p operandul din dreapta
	 * @return Portativ
	*/
	Portativ& operator= (const Portativ &p);

	/**
	 * Numarul de elemente (pauze si note) din portativ
	 * @return 
	*/
	int get_nr_elemente();

	/**
	 * Numarul de masuri din portativ
	 * @return int
	*/
	int get_nr_masuri();

	/**
	 * Numarul de note din portativ 
	 * @return int
	*/
	int get_nr_note();

	/**
	 * Aflam cat de "plina" este ultima masura in carea vrem sa adaugam/stergem
	*/
	Durata get_ultima_masura();

	/**
	 * Numarul de pauze din portativ
	 * @return int
	*/
	int get_nr_pauze();

	/**
	 * Adauga element la finalul portativului
	 * @param e elementul de adaugat
	 * @return true daca s-a adaugat cu succes, false pentru erori
	*/
	bool add_element(Element e);

	/**
	 * Sterge element la finalul portativului
	*/
	void remove_element();

	/**
	 * Inlocuire element de la pozitia data cu un alt element
	 * @param e elementul de inlocuit
	 * @param pos pozitia la care se inlocuieste
	 * @return true daca s-a adaugat cu succes, false pentru erori
	*/
	bool replace_element(Element e, int pos);

	/**
	 * 
	*/
	void afisare();
};
