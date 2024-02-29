#pragma once
#include "menue.h"
#include "player.h"
#include "Insel.h"




class Movement :   public Menue, public Player
{
private:

	//Pointer um Funktionen aus anderen Klassen benutzen zu können

	Insel* m_Insel_Pointer;


public:

	

	Movement(); //Konstruktor

	void Npc_Move(); // Funktion mit Zufallsalgorithmus(Würfelalgorithmus) damit der NPC(Nicht-Spieler-Charakter) einen Zug macht

	void Player_Move(); // Funktion die dem Spieler ein Menü gibt und ihn auf eine Insel bewegen lässt.

	void Wechselde_Auswahl(int zahl);

	
};

