#include "movement.h"
#include "FileMgr.h"



Movement::Movement() : m_Insel_Pointer(nullptr) //Konstruktor 
{

}

void Movement::Wechselde_Auswahl(int zahl) // Funktion die für das abwechselnde Bewegen der Spieler zuständig ist
{
	switch (zahl)
	{
	case 1:
		Player_Move();
		cout << "\n\n" << "Der Computer ist am Zug." << "\n\n";		
		zahl = 2;
		Wechselde_Auswahl(zahl);
		break;
	case 2:
		Npc_Move();
		cout << "\n\n" << "Sie sind am Zug." << "\n\n";
		zahl = 1;
		Wechselde_Auswahl(zahl);
		break;
	}
}

void Movement::Npc_Move()
{
	bool next_player = false; //  Dient zum verlassend er While-Schleife	

	int eingabe = -1;  // Damit die While-Schleife immer läuft

	srand(time(NULL)); // Nutzt die Zeit als Zufallsgenerator(Zeit wird 0 gesetzt)

	while (eingabe != pg->getNodes().size()&&!next_player)
	{
		Showmenue(); // Zeig dem NPC das Menü		

		eingabe = rand() % pg->getNodes().size()+1; //Generiert eine Zufallszahl die zu eine Zahl der Map gehört

		if (eingabe < 0|| eingabe >=6)
		{
			eingabe = rand() % pg->getNodes().size() + 1; //Generiert eine Zufallszahl die zu eine Zahl der Map gehört
		}
		else
		{
			double t = FileMgr::s_Computer->get_Money();
			cout << "\nWohin moechten Sie Reisen?\t\t"<<" "<< "Sie besitzen "<< t << " Taler\n";

			cout << "\nComputer hat:\t" << eingabe << "\t eingegeben \n\n";
			deque<Edge*> liste_der_verbindungen;//Speichert die Verbindungen zwischen den Inseln

			if (FileMgr::s_Computer->get_Start_Island())
			{
				pg->findShortestPathDijkstra(liste_der_verbindungen, *FileMgr::s_Computer->get_Start_Island(), *(s_Inseln[eingabe]));//findet den Konstengünstigsten Weg

				for (auto verbindungen : liste_der_verbindungen) //Ausgabe der Verbindung zu der Zielinsel bzw der Weg zu der Zielinsel.
				{

					cout << verbindungen->toString();
				}

				FileMgr::s_Computer->addjust_Money(liste_der_verbindungen);//passt das Geld der Spieler an

				FileMgr::s_Computer->set_Start_Island(s_Inseln[eingabe]);//setzt die StartInsel des Spielers neu

				next_player = true;//  Dient zum verlassend er While-Schleife
			}
			else
				cout << "Fehler: der Spieler befindet sich auf keiner Insel." << endl;

		}

	}
}


void Movement::Player_Move()
{
	bool next_player=false;//  Dient zum verlassend er While-Schleife


	cout << "\n\nListe der Bereisbaren Orte.\n";

	Showmenue(); // Zeig dem NPC das Menü

	int eingabe; // Speichert die Eingabe

	double t = FileMgr::s_Spieler->get_Money();
	cout << "\nWohin moechten Sie Reisen?\t\t" << " " << "Sie besitzen " << t << " Taler\n";

	cin >> eingabe;

	deque<Edge*> liste_der_verbindungen; //Speichert die Verbindungen zwischen den Inseln

	while (eingabe >= 0 && eingabe < pg->getNodes().size()&&!next_player) // Solange die Eingabe im möglichen Rahmen liegt wird der Spieler zu der Insel reisen.
	{
		

		if (FileMgr::s_Spieler->get_Start_Island())
		{
			pg->findShortestPathDijkstra(liste_der_verbindungen, *(FileMgr::s_Spieler->get_Start_Island()), *(s_Inseln[eingabe])); //findet den Konstengünstigsten Weg

			for (auto verbindungen : liste_der_verbindungen) //Ausgabe der Verbindung zu der Zielinsel bzw der Weg zu der Zielinsel.
			{

				cout << "\n\n" << verbindungen->toString() << "\n\n";
			}

			FileMgr::s_Spieler->addjust_Money(liste_der_verbindungen); //passt das Geld der Spieler an

			FileMgr::s_Spieler->set_Start_Island(s_Inseln[eingabe]); // Setzt die Starinsel auf die bereiste Insel

			next_player = true;//  Dient zum verlassend er While-Schleife
		}			
		else
			cout << "Fehler: der Spieler befindet sich auf keiner Insel." << endl;
	}

	//Sollte die Eingabe kleiner als 0 sein wird eine Fehlermeldung ausgegeben. 
	//Wenn die Eingaben das doppelte der Map size ist nutzt der Spieler die Cheat Funktion und wird auf die Zielinsel geschickt.

	if (eingabe < 0) 
		throw domain_error("Kein gueltiges Reiseziel.");
	else if (eingabe > (2 * pg->getNodes().size()))
	{		
		// Lässt den Spieler zur Schatzinsel reisen
		pg->findShortestPathDijkstra(liste_der_verbindungen, *FileMgr::s_Spieler->get_Start_Island(), *Insel::s_Schatzinsel);

		FileMgr::s_Spieler->set_Start_Island(Insel::s_Schatzinsel);

		for (auto verbindungen : liste_der_verbindungen) //Ausgabe der Verbindung zu der Zielinsel bzw der Weg zu der Zielinsel.
		{
			
			cout << "\n\n" << verbindungen->toString()<< "\n\n";
		}
		cout << "\n\nDU HAST GEWONNNEN!\n\n";
		exit(EXIT_SUCCESS);
	}
}