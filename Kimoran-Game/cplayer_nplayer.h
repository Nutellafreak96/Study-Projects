#pragma once
#include "movement.h"
#include "movement.h"
#include "menue.h"

class cplayer_nplayer :    public movement  //cplayer=Computer-Spieler und nplayer= Normaler-Spieler (Menschlciher-Spieler)
{

private:

	int m_treasure; // Variable um den Schatz zu speichern

	int m_money; // Variable um das Geld des Spielers/NPC zu speichern




public:

	cplayer_nplayer(); //Konstruktor

	void set_money(int geld);  // Geld welches die Spieler zur verfügung haben wird durch diese Funktion gesetzt

	int get_money() const;	// Durch diese Funktion wird das Geld, welches den Spielern zur Verfügung steht abgerufen

	void addjust_money(); // Diese Funktion soll das Geld der Spieler verändern jenachdem ob sie Reisen oder ein Lager finden.

};

// Alternativvorschlag:

enum Player_t { Human, Computer };

class Player
{
public:
	Player(Player_t playerType) : m_playerType(playerType), m_money(0), m_currentLocation(nullptr)
		{ }

	//getter
	Player_t getPlayerType() const { return m_playerType; }
	int getMoney() const { return m_money; }
	Insel* getCurrentLocation() const { return m_currentLocation; }

	//setter
	void setMoney(int money) { m_money = money; }
	void adjustMoney(int moneyDifference) { m_money += moneyDifference; }
	void setLocation(Insel* newLocation) { m_currentLocation = newLocation; }

private:
	Player_t m_playerType;
	int m_money;
	Insel* m_currentLocation;
};