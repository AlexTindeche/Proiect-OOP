#pragma once

#include "durata.h"
#include "durate.h"
#include "element.h"
#include <vector>
#include <string>
using namespace std;

enum class Cheie {SOL, FA};

class Portativ
{
	Cheie cheie;
	Durata masura;
	vector<Element*> elemente;
	vector<int> masuri;
	wstring titlu;

	void destroy_array();

public:
	/**
	 * Constructor
	 * @param m masura portativului
	 * @param c cheia portativului
	*/
	Portativ(wstring t = L"", Durata m = Durata(4, 4, false), Cheie c = Cheie::SOL);

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
	 * Titlul portativului
	 * @return
	*/
	wstring get_titlu();

	/**
	 * @brief 
	 * @return 
	*/
	Durata get_masura();

	/**
	 * @brief 
	 * @return 
	*/
	Cheie get_cheie();

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
	 * @brief 
	 * @param pos 
	 * @return 
	*/
	Element* get_element(int pos);

	/**
	 * @brief 
	 * @param t 
	*/
	void set_titlu(wstring t);

	/**
	 * @brief 
	 * @param c 
	*/
	void set_cheie(Cheie c);

	/**
	 * Elementele dintr-o masura
	 * @param m masura de returnat
	 * @return 
	*/
	vector<Element*> get_elem_masura(int m);

	/**
	 * Adauga element la finalul portativului
	 * @param e elementul de adaugat
	 * @return true daca s-a adaugat cu succes, false pentru erori
	*/
	template<typename T>
	bool add_element(T e)
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
		Element* n = new T(e);
		elemente.push_back(n);

		// daca e masura noua modificam vectorul de masuri
		if (um == Durata())
			masuri.push_back((int)elemente.size() - 1);

		return true;
	}

	/**
	 * Inlocuire element de la pozitia data cu un alt element
	 * @param e elementul de inlocuit
	 * @param pos pozitia la care se inlocuieste
	 * @return true daca s-a adaugat cu succes, false pentru erori
	*/
	template<typename T>
	bool replace_element(T& e, int pos)
	{
		// verificam daca pozitia e valida si
		// verificam daca elementul dat are aceeasi durata ca cel inlocuit
		if (pos < 0 || pos >= elemente.size() || elemente[pos]->get_durata() != e.get_durata())
			return false;

		// stergem elementul vechi si il punem pe cel nou
		delete elemente[pos];
		elemente[pos] = new T(e);

		return true;
	}

	/**
	 * Sterge element la finalul portativului
	*/
	void remove_element();


	/**
	 * 
	*/
	void afisare();
};
