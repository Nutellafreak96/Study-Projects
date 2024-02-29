#include "player.h"
#include "movement.h"
#include "menue.h"
#include "Verbindung.h"
#include "Edge.h"
#include "Insel.h"
#include <algorithm>



Player::Player()
	:m_Money_Player(0.0), m_Money_Computer(0.0), m_Startinsel_Computer(nullptr), m_Startinsel_Player(nullptr), m_Insel_Pointer(nullptr), m_Verbindungs_Pointer(nullptr),
	m_Spielertyp(Spieler)
{
}


Player::Player(PlayerType Spieler) 
	:m_Money_Player(0.0),m_Money_Computer(0.0),m_Startinsel_Computer(nullptr),m_Startinsel_Player(nullptr),m_Insel_Pointer(nullptr),m_Verbindungs_Pointer(nullptr),
	m_Spielertyp(Spieler)
{
	

	for (int i = 0; i < pg->getNodes().size(); i++) //Schleife die als Startinsel die erste eingelesene Inseln nimmt
	{
		set_Start_Island(pg->getNodes().front());		
	}
}

Node* Player::get_Start_Island() const  //Gibt die Starinsel zurück und wird benutzt um die aktuelle Insel des Spielers/Computers abzurufen
{
		

	switch (m_Spielertyp)
	{ 
	case Spieler: return m_Startinsel_Player;

	case Computer: return m_Startinsel_Computer;		

	}	
}

void Player::set_Start_Island(Node* anfangs_insel)//Setzt die Starinsel und wird benutzt um die aktuelle Insel des Spielers/Computers zu setzen
{
	switch (m_Spielertyp)
	{
	case Spieler: m_Startinsel_Player= anfangs_insel;

	case Computer: m_Startinsel_Computer= anfangs_insel;

	}
}

void Player::set_Money(double geld)// Geld welches die Spieler zur verfügung haben wird durch diese Funktion gesetzt
{
	switch(m_Spielertyp)
	{
	case Spieler:
		m_Money_Player = geld;
	case Computer:
		m_Money_Computer = geld;
	}
}

double Player::get_Money() const// Durch diese Funktion wird das Geld, welches den Spielern zur Verfügung steht abgerufen
{
	switch(m_Spielertyp)
	{
	case Spieler:
		return m_Money_Player;
	case Computer:
		return m_Money_Computer;
	}
}


void Player::addjust_Player_Money(Verbindung* current, int index, string vector_node)//Funktion um die Veränderung ded Vermögens des Spielers zusammenzufassen und auszuführen
{
	set_Money(m_Money_Player - current->getWeight());
	if (m_Money_Player <= 0)
	{
		throw domain_error("Spieler ist Pleite");
		cout << "Der Computer hat gewonnen\n";
		exit(EXIT_SUCCESS);// Funktion die das Programm beendet, wenn der Spieler kein Geld mehr hat
	}
	set_Start_Island(&current->s_get_Verbindungen()[index]->getDstNode());// setzt die Insel des Spielers

	Insel::s_get_Island(get_Start_Island())->Visit_Island(vector_node); //Die Besucher anzahl auf der Insel wird erhöht

	set_Money(m_Money_Player + Insel::s_get_Island(get_Start_Island())->getLager()); // Gibt dem Spieler das geld aus dem Lager

	Insel::s_get_Island(get_Start_Island())->Finde_Schatz(vector_node); //Überpruft ob der Spieler auf der Schatzinsel ist, fall ja wird das Programm beendet
}

void Player::addjust_Money_Computer(Verbindung* current, int index, string vector_node) //Funktion um die Veränderung ded Vermögens des computers zusammenzufassen und auszuführen
{
	set_Money(m_Money_Computer - current->getWeight());

	if (m_Money_Computer <= 0)
	{
		throw domain_error("Computer ist Pleite");
		cout << "Sie hat gewonnen\n";
		exit(EXIT_SUCCESS); // Funktion die das Programm beendet, wenn der Computer kein Geld mehr hat
	}

	set_Start_Island(&current->s_get_Verbindungen()[index]->getDstNode());// setzt die Insel des Computers

	Insel::s_get_Island(get_Start_Island())->Visit_Island(vector_node);//Die Besucher anzahl auf der Insel wird erhöht

	set_Money(m_Money_Computer + Insel::s_get_Island(get_Start_Island())->getLager());// Gibt dem Computer das geld aus dem Lager

	Insel::s_get_Island(get_Start_Island())->Finde_Schatz(vector_node);//Überpruft ob der Computer auf der Schatzinsel ist, fall ja wird das Programm beendet
}

//Funktion die das vorhandene Geld des Spielers verändert. D.h. wenn er Gold auf einer Insel finden dazu addiert und wenn er reist welches subtrahiert.
//Durch das Vergleichen der Deque und des Vectors mithilfe der  Insel-Id´s der Verbindungen/Edges werden den Spielern Geld durch das Reisen abgezogen und für sie
//wird als neue Startinsel die nächste Insel gesetzt. Das wird solange gemacht bis der Spieler an der gewünschten Insel angekommen ist, also bis die deque Abgearbeitet ist.
//Dabei wird immer wieder überprüft ob der Spieler auf der Schatzinsel war oder nicht. Fall er auf der Schatzinsel war wird das Programm beendet sonst wird nur übprüft 
//ob es ein Lager auf der Insel gab und falls dem so ist wird dem Spieler das Geld zugeteilt.



void Player::addjust_Money(deque<Edge*> reiseroute)  
{
	int vector_size = m_Verbindungs_Pointer->s_get_Verbindungen().size();

	int deque_size = reiseroute.size();

	for (int k = 0; k < deque_size; k++)
	{

		string deque_node1 = reiseroute[k]->getSrcNode().getId(); // Speicher die Id der Knoten/Inseln damit darüber ein Vergleich gezogen werden kann

		string deque_node2 = reiseroute[k]->getDstNode().getId();// Speicher die Id der Knoten/Inseln damit darüber ein Vergleich gezogen werden kann

		for (int l = 0; l < vector_size; l++)
		{
			string vector_node1 = m_Verbindungs_Pointer->s_get_Verbindungen()[l]->getSrcNode().getId();// Speicher die Id der Knoten/Inseln damit darüber ein Vergleich gezogen werden kann

			string vector_node2 = m_Verbindungs_Pointer->s_get_Verbindungen()[l]->getDstNode().getId();// Speicher die Id der Knoten/Inseln damit darüber ein Vergleich gezogen werden kann

			Verbindung* current_verbindung = m_Verbindungs_Pointer->s_get_Verbindungen()[l];

			if (deque_node1 == vector_node1 && deque_node2==vector_node2 ||deque_node1== vector_node2 && deque_node2 == vector_node1)
			{
				switch (m_Spielertyp)
				{
				case Spieler:
					addjust_Player_Money(current_verbindung,l,vector_node2);
					break;
				case Computer:
					addjust_Money_Computer(current_verbindung, l, vector_node2);
					break;
				}
			}
		}
	}
}