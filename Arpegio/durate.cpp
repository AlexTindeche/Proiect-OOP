#include "pch.h"
#include "durate.h"

const Durata Durate::NOTA_INTREAGA = Durata(1);
const Durata Durate::DOIME = Durata(2);
const Durata Durate::PATRIME = Durata(4);
const Durata Durate::OPTIME = Durata(8);
const Durata Durate::SAISPREZECIME = Durata(16);
const vector<wstring> Durate::nume_durate = { L"Notă întreagă", L"Doime", L"Pătrime", L"Optime", L"Șaisprezecime" };

int Durate::GetPos(Durata d)
{
	if (d == Durate::NOTA_INTREAGA)
		return 0;
	else if (d == Durate::DOIME)
		return 1;
	else if (d == Durate::PATRIME)
		return 2;
	else if (d == Durate::OPTIME)
		return 3;
	else if (d == Durate::SAISPREZECIME)
		return 4;

	return 0;
}

Durata Durate::GetDurata(int pos)
{
	switch (pos)
	{
	case 0:
		return Durate::NOTA_INTREAGA;

	case 1:
		return Durate::DOIME;

	case 2:
		return Durate::PATRIME;

	case 3:
		return Durate::OPTIME;

	case 4:
		return Durate::SAISPREZECIME;
	}

	return Durata();
}