#pragma once
#include "Node.h"
#include <vector>



using namespace std;

class Insel : public Node
{

	
private:
	

	int m_Besucher; //Die Variable soll die Anzahl an Besucher auf der Insel speichern. Das dient dazu um zu sehen,w elcher Spieler zuerst die Insel bereist.

	bool m_Schatz; // Variable die anzeigt ob der Schatz auf dieser Insel liegt

	int m_Lager;// Variable die das Gold des Lagers enthält enthält

	//Pointer um Funktionen aus anderen Klassen benutzen zu können

	
	

	

public:
	// globale Variablen um in anderen Klassen darauf zugreifen zu können
	static Insel* s_get_Island(Node* insel);

	static Insel* s_get_Island(string insel);

	static Insel* s_Schatzinsel;

	static vector<Insel*> s_Inseln;

	Insel(std::string id);  // Konstruktor	

	void setSchatz() { m_Schatz = true; } //Setzt die Schatzvariable auf True

	bool getSchatz() { return m_Schatz; } // Gibt den Wert der Membervariablen zurück

	void setLager(double Taler) { m_Lager = Taler; } //setz den Lagerbestand auf einen gegebenen Wert

	double getLager(); //Gibt uns den Lagerbestand zurück, damit wir dem Spieler sein gefundenen "Schatz" übergeben können	

	void Visit_Island(string Id); //Erhöht die Membervariable der Insel für Besucher

	void Finde_Schatz(string Id); //Überprüft ob der Schatz auf der Insel liegt. Falls ja wird das Programm beendet

	static vector<Insel*> get_Insel() { return s_Inseln; } //Gibt den Vector zurück	

	Node* get_Treasure_Island() const; // Funktion um die Schatzinsel aufrufen zu können



};

