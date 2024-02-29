#pragma once
#include "Node.h"
#include <vector>



using namespace std;

class Insel : public Node
{

	
private:
	

	int m_Besucher; //Die Variable soll die Anzahl an Besucher auf der Insel speichern. Das dient dazu um zu sehen,w elcher Spieler zuerst die Insel bereist.

	bool m_Schatz; // Variable die anzeigt ob der Schatz auf dieser Insel liegt

	int m_Lager;// Variable die das Gold des Lagers enth�lt enth�lt

	//Pointer um Funktionen aus anderen Klassen benutzen zu k�nnen

	
	

	

public:
	// globale Variablen um in anderen Klassen darauf zugreifen zu k�nnen
	static Insel* s_get_Island(Node* insel);

	static Insel* s_get_Island(string insel);

	static Insel* s_Schatzinsel;

	static vector<Insel*> s_Inseln;

	Insel(std::string id);  // Konstruktor	

	void setSchatz() { m_Schatz = true; } //Setzt die Schatzvariable auf True

	bool getSchatz() { return m_Schatz; } // Gibt den Wert der Membervariablen zur�ck

	void setLager(double Taler) { m_Lager = Taler; } //setz den Lagerbestand auf einen gegebenen Wert

	double getLager(); //Gibt uns den Lagerbestand zur�ck, damit wir dem Spieler sein gefundenen "Schatz" �bergeben k�nnen	

	void Visit_Island(string Id); //Erh�ht die Membervariable der Insel f�r Besucher

	void Finde_Schatz(string Id); //�berpr�ft ob der Schatz auf der Insel liegt. Falls ja wird das Programm beendet

	static vector<Insel*> get_Insel() { return s_Inseln; } //Gibt den Vector zur�ck	

	Node* get_Treasure_Island() const; // Funktion um die Schatzinsel aufrufen zu k�nnen



};

