#include "Verbindung.h"
#include "menue.h"



vector<Verbindung*> Verbindung::s_Verbindungen; // Vector der alle Verbindungen speichert und deswegen als static deklariert ist


Verbindung::Verbindung(Node& rSrc, Node& rDst, Link_t type)	: Edge(rSrc, rDst), m_LinkType(type)
{ }

/*
 * Das Kantengewicht entspricht hier den Zollkosten.
 * Je nachdem ob eine Verbindung Tunnel, Bruecke oder Fähre ist,
 * müssen 8, 5 oder 3 Taler bezahlt werden.
 * Dies ist wichtig für den Dijkstra-Algorithmus.
 */
double Verbindung::getWeight()
{
	switch (m_LinkType)
	{
	case Tunnel:
		return 8.0;
		break;
	case Bruecke:
		return 5.0;
		break;
	case Faehre:
		return 3.0;
		break;
	}
}

vector<Verbindung*> Verbindung::s_get_Verbindungen() // gibt den vector wieder
{
	return s_Verbindungen;
}

Link_t Verbindung::get_LinkType() //funktion um mit dem Verbindungstypen zu arbeiten
{
	return m_LinkType;
}

string Verbindung::Linktype() // damit ich in der to_string funktion die art der verbindung in einen string umwandeln aknn
{
	switch (m_LinkType)
	{
	case Tunnel:
		return "Tunnel";
			break;
	case Bruecke:
		return "Bruecke";
			break;
	case Faehre:
		return "Faehre";
			break;
	}
}

void Verbindung::Show_Connections() // to_string damit man im Menü die Ausgabe der Verbindungen nuzen kann
{
	cout << getSrcNode().getId() << " "<<"\t<-->\t"<< " " << getDstNode().getId();
	cout << " " <<"\t Verbindungstyp:\t" << Linktype() << "\n";
}