#pragma once
#include <Edge.h>
#include <sstream>
#include <vector>


using namespace std;

enum Link_t { Tunnel, Bruecke, Faehre }; //Enumeration f�r die Art der Verbindung

class Verbindung : public Edge
{
public:
	
	

	Verbindung(Node& rSrc, Node& rDst, Link_t type);

	
	/*
	 * Das Kantengewicht entspricht hier den Zollkosten.
	 * Je nachdem ob eine Verbindung Tunnel, Bruecke oder F�hre ist,
	 * m�ssen 8, 5 oder 3 Taler bezahlt werden.
	 * Dies ist wichtig f�r den Dijkstra-Algorithmus.
	 */
	virtual double getWeight();	

	vector<Verbindung*> s_get_Verbindungen(); // gibt den vector wieder

	string Linktype(); // damit ich in der to_string funktion die art der verbindung in einen string umwandeln kann

	void Show_Connections(); // to_string damit man im Men� die Ausgabe der Verbindungen nuzen kann	

	Link_t get_LinkType(); //funktion um mit dem Verbindungstypen zu arbeiten

	static vector<Verbindung*> s_Verbindungen; //Vector der alle Verbindungen zwischen den Inseln enth�lt

private:

	Link_t m_LinkType;

	//Pointer um Funktionen aus anderen Klassen benutzen zu k�nnen

	

};

