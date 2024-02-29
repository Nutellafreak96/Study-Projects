#pragma once
#include "Graph.h"
#include "Node.h"
#include <vector>
#include <map>
#include <iostream>
#include "time.h"
#include "Verbindung.h"

class Insel;

using namespace std;

extern Graph* pg; //damit wir den Graphen in allen Header-/CPP-Dateien nutzen können (menue.h muss inkludiert werden)

class Menue : public Graph    
{
private:        

    //Pointer um Funktionen aus anderen Klassen benutzen zu können

    Verbindung *m_Verbindungs_Pointer;

    Insel* m_Insel_Pointer;

    

protected:

    static map<int, Node*> s_Inseln; //Map in der die Inseln nochmals gespeichert werden

public:

    Menue(); //Konstruktor    

    static void s_Safe_Island(); //Funktion zum speichern der Inseln in einer Map

    void Showmenue(); // Menü,welches Angezeigt wird, damit der NPC seinen nächsten zu planen kann


};

