#pragma once
#include "menue.h"
#include "Graph.h"
#include "Verbindung.h"
#include "Insel.h"


enum PlayerType {Spieler,Computer};


class Player   
{

private:	

	double m_Money_Player; // Variable um das Geld des Spielers

	double	m_Money_Computer; // Variable um das Geld des NPC zu speichern

	PlayerType m_Spielertyp; // Variable um den Spielertypen für eineige Funktionen festzulegen


	// Pointer um auf Funktionen der Klasse zugreifen zu können
	Node *m_Startinsel_Computer;

	Node *m_Startinsel_Player;	

	Verbindung* m_Verbindungs_Pointer;

	Insel* m_Insel_Pointer;



public:

	Player();//Standart-Konstruktor

	Player(PlayerType Spieler); //Konstruktor

	Node* get_Start_Island() const;  //Funktion die für den Spieler und dem Computer die Startinsel als auch die Momentane Insel zurück gibt

	void set_Start_Island(Node *anfangs_insel); // Setzt die Startinsel als auch die Insel auf der die Spieler reisen

	void set_Money( double geld);  // Geld welches die Spieler zur verfügung haben wird durch diese Funktion gesetzt

	double get_Money() const;	// Durch diese Funktion wird das Geld, welches den Spielern zur Verfügung steht abgerufen

	void addjust_Money(deque<Edge*> reiseroute); // Diese Funktion soll das Geld der Spieler verändern jenachdem ob sie Reisen oder ein Lager finden.

	void addjust_Player_Money(Verbindung* current, int index, string vector_node); // Diese Funktion umfasst die Geldanpassungen des Menschlichen Spielers

	void addjust_Money_Computer(Verbindung* current, int index, string vector_node);// Diese Funktion umfasst die Geldanpassungen des Computer Spielers



};

