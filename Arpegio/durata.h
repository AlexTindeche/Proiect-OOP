#pragma once

#include <iostream>
using namespace std;

/**
 * @class Durata
 * Reprezentare durata sub forma de fractiune din nota intreaga
 * Folosita pentru a specifica durata unui element, dar si
*/
class Durata
{
private:
	int numarator,
		numitor;
	//bool punct;

public:
	/**
	 * Instantiere durata (perceputa ca fractie din nota intreaga)
	 * @param numarator - numaratorul fractiei
	 * @param numitor - numitorul fractiei
	 * @param simplificare - daca sa fie simplificata fractia
	*/
	Durata(int numarator, int numitor, bool simplificare = true);

	/**
	 * Initializare durata ca si fractiune din nota intreaga
	 * @param fractiune - fractiunea din nota intreaga
	*/
	Durata(int fractiune);

	Durata();

	/**
	 * Getter fractiune durata
	 * @return int
	*/
	int get_numarator();

	/**
	 * Getter intregi durata
	 * @return int
	*/
	int get_numitor();

	/**
	 * Intregii din fractie
	 * @return int
	*/
	int get_intregi();

	/**
	 * Fractia ramasa dupa scoaterea intregilor din fractie
	 * @return Durata
	*/
	Durata get_fractiune();

	/**
	 * Inversul fractiei
	 * @return Durata
	*/
	Durata get_invers();

	/**
	 * @brief 
	*/
	void simplificare();

	/**
	 * Adunare doua durate returneaza o durata
	 * @param d operandul din stanga
	 * @return
	*/
	Durata operator+(Durata d) const;

	/**
	 * Scadere a doua durate ce returneaza tot o durata
	 * @param d operandul din stanga
	 * @return
	*/
	Durata operator-(Durata d);

	/**
	 * Inmultirea a doua durate
	 * @param d operandul din stanga
	 * @return Durata
	*/
	Durata operator*(Durata d);

	/**
	 * Impartirea unei durate la alta durata returneaza de cate ori durata din stanga o cuprinde pe cea din dreapta
	 * @param d operandul din stanga
	 * @return int
	*/
	int operator/(Durata d);

	/**
	 * 
	 * @param d operandul din stanga
	 * @return Durata
	*/
	Durata operator%(Durata d);

	/**
	 * Operatorul de comparatie > (mai mare)
	 * @param d operandul din stanga
	 * @return Durata
	*/
	bool operator>(Durata d);

	/**
	 * Operatori de comparatie < (mai mic)
	 * @param d operandul din stanga
	 * @return Durata
	*/
	bool operator<(Durata d);

	/**
	 * Operatori de comparatie == (egal)
	 * @param d operandul din stanga
	 * @return Durata
	*/
	bool operator==(Durata d);

	/**
	 * Operatori de comparatie <= (mai mic sau egal)
	 * @param d operandul din stanga
	 * @return Durata
	*/
	bool operator<=(Durata d);

	/**
	 * Operatori de comparatie >= (mai mare sau egal)
	 * @param d operandul din stanga
	 * @return Durata
	*/
	bool operator>=(Durata d);

	/**
	 * Operatori de comparatie != (diferit)
	 * @param d operandul din stanga
	 * @return Durata
	*/
	bool operator!=(Durata d);

	friend istream& operator>> (istream& in, Durata &d);
	friend ostream& operator<< (ostream& out, Durata d);
};