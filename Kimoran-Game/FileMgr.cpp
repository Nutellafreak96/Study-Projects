#include <iostream>
#include <fstream>
#include "FileMgr.h"
#include "Insel.h"
#include "Verbindung.h"
#include "player.h"
#include "Node.h"

#define DEBUG

Player* FileMgr::s_Spieler;
Player FileMgr::s_Menschlicher_Spieler;

Player* FileMgr::s_Computer;
Player FileMgr::s_Computer_Spieler;



void FileMgr::getFilename()
{
	std::string filenameTemp;					//Temporäre String Variable
	std::cout << "Dateipfad: " << std::flush;	//Aufforderung zur eingabe des Dateipfades
	std::cin >> filenameTemp;					//Einlesen der Nutzereingabe
	std::ifstream file;							//Neuer InputFileStream wird angelegt (lesender Zugriff)
	file.open(filenameTemp);					//Datei wird geöffnet
	if (file.is_open())							//wenn Datei gelesen werden konnte
	{
		file.close();							//Datei wird wieder geschlossen
		m_filename = filenameTemp;				//Dateiname wird in MemberVariable geschrieben
	}
	else										//Falls Datei nicht gelesen werden kann, wirf eine FileMgrException
	{
		throw FileMgrException("Datei ist nicht vorhanden oder konnte nicht gelesen werden.");
		exit(EXIT_SUCCESS);
	}
}

void FileMgr::readFromFile()
{
	std::ifstream file;				//Neuer Inputfilestream, lesender Zugriff

	file.open(m_filename);			//Datei wird geöffnet

	std::string word;

	while (file >> word)			//hier folgt die Verarbeitung der Datei
	{
		/*
		 * Leider ist hier kein Switch-Statement möglich, da
		 * dies nicht mit strings arbeiten kann.
		 */
		if (word == "Insel")
		{
			std::string inselName;
			file >> inselName;							//nächstes Word wird eingelesen
			handleInsel(inselName);						//Insel wird angelegt
		}
		else if (word == "Tunnel")
		{
			std::string insel1, insel2;
			file >> insel1;								//nächstes Word wird eingelesen
			file >> insel2;								//nächstes Word wird eingelesen
			handleVerbindung(insel1, insel2, Tunnel);	//Verbindung wird angelegt
		}
		else if (word == "Bruecke")
		{
			std::string insel1, insel2;
			file >> insel1;								//nächstes Word wird eingelesen
			file >> insel2;								//nächstes Word wird eingelesen
			handleVerbindung(insel1, insel2, Bruecke);	//Verbindung wird angelegt
		}
		else if (word == "Faehre")
		{
			std::string insel1, insel2;
			file >> insel1;								//nächstes Word wird eingelesen
			file >> insel2;								//nächstes Word wird eingelesen
			handleVerbindung(insel1, insel2, Faehre);	//Verbindung wird angelegt
		}
		else if (word == "Schatz")
		{
			std::string schatzInsel;        
			file >> schatzInsel;                //Wort wird eingelesen
			handleSchatz(schatzInsel);          //Der Schatz wird auf der Insel versteckt
		}
		else if (word == "Lager")
		{
			std::string LagerInsel, TalerAmount, TalerWord;
			file >> LagerInsel;					//nächstes Word wird eingelesen
			file >> TalerAmount;				//nächstes Word wird eingelesen
			file >> TalerWord;					//nächstes Word wird eingelesen
			if (TalerWord != "Taler")
			{
				throw FileMgrException("Die Datei entspricht nicht dem notwendigen Format");
				exit(EXIT_SUCCESS);
			}
			handleLager(LagerInsel, TalerAmount);	//Auf einigen Inseln wir ein Lager mit Talern angelegt
		}
		else if (word == "Mensch")  // Funktion um das Start Kapital und die Startinsel des Menschlichen Spielers zu setzen
		{
			string StartInselSpieler,TalerWord;
			int StartKapitalSpieler;

			file >> StartInselSpieler;			//nächstes Word wird eingelesen
			file >> StartKapitalSpieler;		//nächstes Word wird eingelesen
			file >> TalerWord;					//nächstes Word wird eingelesen
			if (TalerWord != "Taler")
			{
				throw FileMgrException("Die Datei entspricht nicht dem notwendigen Format");
				exit(EXIT_SUCCESS);
			}
			handlePlayer(StartInselSpieler, StartKapitalSpieler);	//Dem Spieler wird seine Startinsel gesetzt und ihm wird sein Startguthaben zugewiesen

		}
		else if (word == "Computer") // Funktion um as Start Kapital und die Startinsel des Computer Spielers zu setzen
		{ 
			string StartInselComputer,TalerWord;
			int StartKapitalComputer;

			file >> StartInselComputer;				//nächstes Word wird eingelesen
			file >> StartKapitalComputer;			//nächstes Word wird eingelesen
			file >> TalerWord;						//nächstes Word wird eingelesen
			if (TalerWord != "Taler")
			{
				throw FileMgrException("Die Datei entspricht nicht dem notwendigen Format");
				exit(EXIT_SUCCESS);
			}				
			handleComputer(StartInselComputer, StartKapitalComputer);		//Dem Computer wird seine Startinsel gesetzt und ihm wird sein Startguthaben zugewiesen
		}
		else
		{
			file.close(); //Datei wird geschlossen bevor Programm abbricht
			throw FileMgrException("Die Datei entspricht nicht dem notwendigen Format");
			exit(EXIT_SUCCESS);
		}
	}
	Menue::s_Safe_Island();
	file.close();
}

void FileMgr::handleInsel(std::string name)
{
	Insel* pInsel = new Insel(name);			//die Insel trägt den Namen des nächsten Wortes
	Insel::s_Inseln.push_back(pInsel);			//Speichert die Insel in einem Vector ab um diese weiter zu verwenden.
	           

	Node* pNode = dynamic_cast<Node*>(pInsel);	//Upcast der Insel zu Node
	pg->addNode(pNode);						//Insel wird dem Graph hinzugefügt
#ifdef DEBUG
	std::cout << "DEBUG: " << "Eine Insel des Namens \"" << name << "\" wurde erstellt." << std::endl;
#endif /* DEBUG */
}

void FileMgr::handleVerbindung(std::string insel1, std::string insel2, Link_t verbindugsArt)
{
	Node* pNode1 = pg->findNode(insel1);		//ein Pointer auf die 1. Insel wird gespeichert
	Node* pNode2 = pg->findNode(insel2);		//ein Pointer auf die 2. Insel wird gespeichert

	// Die Verbindung muss in beide Richtungen angelegt werden
	Verbindung* pVerbindung = new Verbindung(*pNode1, *pNode2, verbindugsArt);	
	pg->addEdge(pVerbindung);					//Verbindung in Richtung pNode2 wird dem Graph hinzugefügt

	// Verbindung wird in einer Extralsite gespeichert damit man das für die Spieler ausgegeb kann
	
	Verbindung::s_Verbindungen.push_back(pVerbindung); 

	pVerbindung = new Verbindung(*pNode2, *pNode1, verbindugsArt);
	pg->addEdge(pVerbindung);					//Verbindung in Richtung pNode1 wird dem Graph hinzugefügt

#ifdef DEBUG
	switch (verbindugsArt)
	{
	case Tunnel:
		std::cout << "DEBUG: " << "Ein neuer Tunnel zwischen " << insel1 << " und " << insel2 << " wurde angelegt." << std::endl;
		break;
	case Bruecke:
		std::cout << "DEBUG: " << "Eine neue Bruecke zwischen " << insel1 << " und " << insel2 << " wurde angelegt." << std::endl;
		break;
	case Faehre:
		std::cout << "DEBUG: " << "Eine neue Faehrverbindung zwischen " << insel1 << " und " << insel2 << " wurde angelegt." << std::endl;
		break;
	default:
		break;
	}
#endif /* DEBUG */
}



void FileMgr::handleSchatz(std::string insel)
{
	if (!m_schatzSetOnce)	//Wenn der Schatz noch nicht eingelesen wurde
	{
		Insel::s_get_Island(insel)->setSchatz();									//Schatz wir auf die Insel gesetzt
		Insel::s_Schatzinsel=Insel::s_Schatzinsel->Insel::s_get_Island(insel);	// Die Schatzinsel wir nochmal als Variable gesetzt		  
		m_schatzSetOnce = true;													//Schatz als gesetzt markieren um mehrfachsetzen des Schatzes zu verhindern
#ifdef DEBUG
		std::cout << "DEBUG: " << "Ein Schatz wurde auf einer Insel versteckt." << std::endl;
#endif /* DEBUG */
	}
	else					//Wenn der Schatz bereits eingelesen wurde
	{
		throw FileMgrException("Der Schatz darf nur einmalig eingelesen werden.");
		exit(EXIT_SUCCESS);
	}
}


void FileMgr::handleLager(std::string insel, std::string taler)
{
	Node* pLagerNode = pg->findNode(insel);					//Node pointer
	Insel* pLagerInsel = dynamic_cast<Insel*>(pLagerNode);	//Downcast des Nodes zur Insel
	try
	{
		double i = std::stoi(taler);							//Der string wird in einen integer überführt
		pLagerInsel->setLager(i);							//das Lager wird mit den Talern befüllt

	}
	catch (std::invalid_argument const& e)
	{
		std::cout << "Bad input: std::invalid_argument thrown" << '\n';
	}
	catch (std::out_of_range const& e)
	{
		std::cout << "Integer overflow: std::out_of_range thrown" << '\n';
	}
#ifdef DEBUG
	std::cout << "DEBUG: " << "Ein Lager mit Talern wurde auf einer Insel versteckt." << std::endl;
#endif // DEBUG
}

//Ein Spieler wird erstellt und ihm wird eine Startinsel zugewiesen und sein Startguthaben überwiesen
void FileMgr::handlePlayer(std::string StartInsel, int StartGuthaben)
{	

	s_Menschlicher_Spieler = Player(Spieler);
	s_Spieler = &s_Menschlicher_Spieler;
	Node* pInselNode = pg->findNode(StartInsel);
	m_Player_Pointer = &s_Menschlicher_Spieler;

	m_Player_Pointer->set_Start_Island(pInselNode);
	m_Player_Pointer->set_Money(StartGuthaben);

#ifdef DEBUG
	std::cout << "DEBUG: " << "Ein Spieler mit " << StartGuthaben << " Taler wurde auf die Startinsel" << StartInsel << " gesetzt." << std::endl;
#endif // DEBUG
}
//Ein Computer-Spieler wird erstellt und ihm wird eine Startinsel zugewiesen und sein Startguthaben überwiesen
void FileMgr::handleComputer(string StartInsel, int StartGuthaben)
{	
	s_Computer_Spieler = Player(Computer);
	s_Computer = &s_Computer_Spieler;
	Node* pInselNode = pg->findNode(StartInsel);
	m_Player_Pointer = &s_Computer_Spieler;

	m_Player_Pointer->set_Start_Island(pInselNode);
	m_Player_Pointer->set_Money(StartGuthaben);

#ifdef DEBUG
	std::cout << "DEBUG: " << "Der Computer wurde mit " << StartGuthaben << " Taler wurde auf die Startinsel" << StartInsel << " gesetzt." << std::endl;
#endif // DEBUG
}

