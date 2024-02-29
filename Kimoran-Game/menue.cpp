#include "menue.h"





Menue::Menue() : m_Verbindungs_Pointer(nullptr), m_Insel_Pointer(nullptr) // Konstruktor den ich zum initialisieren der Membervariablen nutze.
{


}


map<int, Node*> Menue::s_Inseln;


void Menue::s_Safe_Island() //Funktion zum abspeichern der Inseln in einer Map
{

	int i = 0;

	for (auto island : pg->getNodes()) //Schleife die alle Nodes/Inseln in einer Map speichert.
	{
		

		s_Inseln.insert(pair<int, Node*>(i, island)); //Speicher die Inseln mit der Zahl unter i mit dem Pointer der Node in der Map ab

		i++;
	}
}


void Menue::Showmenue()  //Soll dem NPC eine Liste aller Inseln geben die er bereisen kann.
{

	//Ausgabe die sein Momentanes Vermögen ausgibt (Noch nicht geschrieben, da der Spieler sowas auch benötigen könnte)
	

	// Gibt die Map mit den inseln aus.
	for (auto ins=s_Inseln.begin();ins != s_Inseln.end();ins++)
	{
		if (s_Inseln.size())
		{

			//ins.first steht für den integer der Map |ins.second für den Node* der Map

			cout << "\n[" << ins->first << "]: " << ins->second->getId() << endl;
		}
	}
	cout << "\nListe der Verbindungen zwischen den Inseln.\n\n";

	for (auto verbindung : Verbindung::s_Verbindungen)
	{
		verbindung->Show_Connections();
	}
	

}




