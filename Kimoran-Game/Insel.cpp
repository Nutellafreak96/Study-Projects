#include "Insel.h"
#include "menue.h"
#include <iostream>
#include "Graph.h"

vector<Insel*> Insel::s_Inseln; // Vector der alle Inseln speichert und deswegen als static deklariert ist

Insel* Insel::s_Schatzinsel;

Insel::Insel(std::string id) : Node(id), m_Besucher(0), m_Schatz(false), m_Lager(0) // Konstruktor
{
	
}  


double Insel::getLager()
{
	double Lager = m_Lager;
	m_Lager = 0;
	return Lager;
}

Insel* Insel::s_get_Island(Node* insel) //Gibt uns die passende Insel zur Node zurück
{
	for (auto island : Insel::s_Inseln)
	{
		if (island == insel)
		{
			return island;
		}
	}

}

Insel* Insel::s_get_Island(string insel)//Gibt uns die passende Insel zur Node-Id/zum Inselnamen zurück
{
	for (auto island : Insel::s_Inseln)
	{
		if (island->getId() == insel)
		{
			return island;
		}
	}

}

Node* Insel::get_Treasure_Island()const // Gibt die INsel mit dem Schatz zurück
{
	return s_Schatzinsel;
}

void Insel::Visit_Island(string Id) // Zählt die Besucher auf einer Insel
{
	if (s_Inseln.size() == 0)
		throw domain_error("Keine Inseln vorhanden");

	for (int i = 0; i < s_Inseln.size(); i++)
	{
		if (s_Inseln[i]->getId() == Id)
		{
			m_Besucher = m_Besucher + 1;
		}
	}

	
}
void Insel::Finde_Schatz(string Id) //Überprüft ob der Schatz auf der Insel liegt. Falls ja wird das Programm beendet
{
	if (s_Inseln.size() == 0)
		throw domain_error("Keine Inseln vorhanden");

	for (int i = 0; i < s_Inseln.size(); i++)
	{
		if (s_Inseln[i]->getId() == Id)
		{
			switch (getSchatz())
			{
			case true:
				cout << "Du hast Gewonnen!\n";
				exit(EXIT_SUCCESS);
				break;
			}
		}
	}
}
