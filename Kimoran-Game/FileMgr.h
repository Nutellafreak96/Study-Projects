#pragma once
#include <string>
#include "menue.h" // um zugriff auf den Graphen pg zu haben
#include "Graph.h"
#include "Verbindung.h"
#include "player.h"


using namespace std;

class FileMgr
{
public:

	/*
	 * @brief Nimmt über die Standardeingabe einen Dateinamen entgegen.
	 * Überprüft die Datei und speichert den Daiteinamen in m_filename.
	 *
	 * @throws FileMgrException wenn Datei nicht lesbar oder nicht vorhanden
	 */
	void getFilename();
	void readFromFile();

	// globale Variablen um in anderen Klassen darauf zugreifen zu können
	static Player* s_Spieler;
	static Player* s_Computer;

	static Player s_Computer_Spieler;
	static Player s_Menschlicher_Spieler;
	

private:

	//Pointer um Funktionen aus anderen Klassen benutzen zu können
	Verbindung *m_Verbindung_Pointer = nullptr;

	Player *m_Player_Pointer = nullptr;

	Insel* m_Insel_Pointer = nullptr;

	

	//Wichtige Funktionen damit die Eingelesenen Werte/Informationen verarbeitet werden

	void handleInsel(std:: string name);
	void handleVerbindung(std::string insel1, std::string insel2, Link_t verbindugsArt);
	void handleSchatz(std::string insel);
	void handleLager(std::string insel, std::string taler);
	void handlePlayer(std::string startInsel, int startGuthaben);
	void handleComputer(string startINsel, int startGuthaben);

	bool m_schatzSetOnce = false; //Kontrolle ob der Schatz nur einmal eingelesen wird
	std::string m_filename;
};

class FileMgrException
{
public:
	/*
	 * @brief Erzeugt die FileMgrException Falls beim Handhaben
	 * der Datei etwas schief läuft. Grund kann mit Member-Funktion
	 * what() abgerufen werden.
	 */
	FileMgrException(std::string error) : m_error(error) {}
	inline std::string what() const { return m_error; }
private:
	std::string m_error;
};