#pragma once
#include "menue.h"
#include "player.h"
#include "Insel.h"




class Movement :   public Menue, public Player
{
private:

	//Pointer um Funktionen aus anderen Klassen benutzen zu k�nnen

	Insel* m_Insel_Pointer;


public:

	

	Movement(); //Konstruktor

	void Npc_Move(); // Funktion mit Zufallsalgorithmus(W�rfelalgorithmus) damit der NPC(Nicht-Spieler-Charakter) einen Zug macht

	void Player_Move(); // Funktion die dem Spieler ein Men� gibt und ihn auf eine Insel bewegen l�sst.

	void Wechselde_Auswahl(int zahl);

	
};

