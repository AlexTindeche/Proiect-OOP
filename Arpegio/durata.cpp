#include "pch.h"

#include "durata.h"

int cmmdc(int a, int b)
{
	while (b != 0) {
		int t = b;
		b = a % b;
		a = t;
	}
	if (a == 0)
		return 1;
	return a;
}

Durata::Durata(int numarator, int numitor) :
	numarator{ numarator },
	numitor{ numitor }
{
	simplificare();
};

Durata::Durata() :
	Durata(0, 1)
{}

Durata::Durata(int fractiune) :
	Durata(1, fractiune)
{}

int Durata::get_numarator()
{
	return numarator;
}

int Durata::get_numitor()
{
	return numitor;
}

int Durata::get_intregi()
{
	return numarator / numitor;
}

Durata Durata::get_fractiune()
{
	return Durata(numarator % numitor, numitor);
}

Durata Durata::get_invers()
{
	return Durata(numitor, numarator);
}

void Durata::simplificare()
{
	int div = cmmdc(numarator, numitor);
	numarator /= div;
	numitor /= div;
}

Durata Durata::operator+ (Durata d) const
{
	int div = cmmdc(this->numitor, d.numitor);
	int amp_1 = d.numitor / div; // amplificare fractie 1
	int amp_2 = this->numitor / div; // amplificare fractie 2

	int numarator = this->numarator * amp_1 + d.numarator * amp_2;
	int numitor = this->numitor * amp_1;

	return Durata(numarator, numitor);
}

Durata Durata::operator- (Durata d)
{
	int div = cmmdc(this->numitor, d.numitor);
	int amp_1 = d.numitor / div; // amplificare fractie 1
	int amp_2 = this->numitor / div; // amplificare fractie 2

	int numarator = this->numarator * amp_1 - d.numarator * amp_2;
	int numitor = this->numitor * amp_1;

	return Durata(numarator, numitor);
}

Durata Durata::operator* (Durata d)
{
	return Durata(numarator * d.numarator, numitor * d.numitor);
}

int Durata::operator/ (Durata d)
{
	Durata prod = *this * d.get_invers();
	// Durata prod = Durata(numarator * d.numitor, numitor * d.numarator)
	return prod.get_intregi();
}

Durata Durata::operator% (Durata d)
{
	Durata prod = *this * d.get_invers();
	// Durata prod = Durata(numarator * d.numitor, numitor * d.numarator)
	return prod.get_fractiune() * d; //TODO: fix %
}

bool Durata::operator>(Durata d)
{
	if (this->numarator * d.numitor > this->numitor * d.numarator)
		return true;
	return false;
}

bool Durata::operator<(Durata d)
{
	if (this->numarator * d.numitor < this->numitor * d.numarator)
		return true;
	return false;
}

bool Durata::operator==(Durata d)
{
	if (this->numarator * d.numitor == this->numitor * d.numarator)
		return true;
	return false;
}

bool Durata::operator<=(Durata d)
{
	return *this < d || *this == d;
}

bool Durata::operator>=(Durata d)
{
	return *this > d || *this == d;
}
bool Durata::operator!=(Durata d)
{
	return !(*this == d);
}

ostream& operator<< (ostream& out, Durata d)
{
	out << d.numarator << "/" << d.numitor;
	return out;
}

istream& operator>> (istream& in, Durata &d)
{
	in >> d.numarator >> d.numitor;
	return in;
}