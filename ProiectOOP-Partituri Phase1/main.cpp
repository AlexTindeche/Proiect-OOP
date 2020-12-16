#include <iostream>
using namespace std;

/**
 * Partituri
 * - Element (durata)
 *   - Nota (inaltime, durata) -> enum class -> Do, Re, Mi, Fa, Sol, La, Si; + octava (raportat la do central); enum class alteratie: Diez, Bemol, Becar
 *     - Patrime, Doime, Nota intreaga, Optime, Saisprezecime -> dam numia inaltimea la instantiere
 *   - Pauze (durata)
 * 
 * - Portativ (colectie de Elemente) (masura -> sus si jos, cheie, ..., armatura (in etapa 2; eventual ii gama), cheie -> sol / fa)
 *   - verificam daca notele intra corect in masuri
 *   - numarul de masuri
 *   - numarul de note (nu si pauze) / numarul de note de un anumit tip
 *   - ()canta -> afiseaza in terminal modul in care ar fi cantat (nota, inaltime, bemol / diez) -> tinand cont de armatura + afisare impartita pe masuri
 * 
 *   - operatii
 *     - adunam note la portativ (le adauga la spate) !! verificare la adaugare
 *     - inlocuire element
 *     - stergere element
 *     - operatorul de decrementare (--) scoate ultima nota
 *     - scoate toate aparitiile unei note sau a unei secvente de portativ (cu / ) (inlocuire cu pauza de durata corespunzatoare)
 *     - multiplicare portativ (cu *): adauga la portativ tot portativul de mai multe ori
 *     - operatorul [] - ia elementul de pe pozitia respectiva
 *     - metoda .size()
 * 
 *   - important
 *     -> constructor si destructor (!! eliberarea de memorie)
 * 
 * - Partitura  - colectie de portative
 *   - titlu
 *   - bpm
 *   
 *   - operatii
 *     - salvare si citire din fisier (eventual genereaza binar)
 *     - operator [] pentru portative + .size()
 *     - canta() -> apeleaza metoda pentru fiecare portativ
*/

int main()
{
	int x;
	cin >> x;
	return 0;
}