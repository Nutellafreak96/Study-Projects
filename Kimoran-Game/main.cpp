#include "Graph.h"
#include "menue.h"
#include "FileMgr.h"
#include "player.h"
#include "movement.h"
#include "Insel.h"
#include "Verbindung.h"

Graph* pg = 0; //Graph existiert zum Start des Programms noch nicht

void spieler_auswahl()    //Funktions soll den Spieler, welcher beginnen darf bestimmen
{
	int wuerfel;

	Movement beginner;

	srand(time(NULL));

	wuerfel = rand();

	if (wuerfel % 2 == 0)
	{
		cout << "\n Der Spieler darf den ersten Zug machen.\n" << endl;
		beginner.Wechselde_Auswahl(1);
	}
	else
	{
		cout << "\nDer Computer darf den ersten Zug machen.\n" << endl;
		beginner.Wechselde_Auswahl(2);

	}
}


int main()
{
	pg = new Graph;	//Anlegen des Graphen

	

	try
	{
		FileMgr fm;
		fm.getFilename();
		fm.readFromFile();
	}
	catch (const FileMgrException& e)
	{
		std::cout << "Filemanager Exception: " << e.what() << std::endl;
		exit(EXIT_SUCCESS);
	}

	


	spieler_auswahl(); //Die Funktion bringt das Spiel zum laufen. 






	delete pg; //Graph muss am Ende des Programms wieder vom Heap gelöscht werden


}