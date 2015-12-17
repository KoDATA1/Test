#include <iostream>			// inclusion pour la gestion des E/S
#include <conio.h>			// Gestion console (ici pour _getch)
#include <Windows.h>			// Pour SetConsoleOutput
#include <string>				// PAS DE ".H" pour utiliser les string de la STL

#include "RS232.h"

using namespace std;			// Utilisation de l’espace de nommage standard

/*****************************************************************
FONCTION PRINCIPALE
****************************************************************/
int main()
{
	SetConsoleOutputCP(1252);

	//port: 9600, 8 , 1, sans parité
	int Port=1;
	char chaine[100+1], err[1];

	//cout << "n° Port : ";
	//cin >> Port;

	CRS232 com(1);

		cout << "envoi : ";
		cin >> chaine;

		for (int i = 0; i < sizeof(chaine); i++)
		{
			com.initialiser(9600, 8, 1, NOPARITY);
			com.ecrire((byte)chaine);
			com.presenceCar();

			if (com.getErreur() != 0)
			{
				com.getMessageErreur(err);
				cout << err << endl;
			}
			else
			{
				cout << "reçu: " << com.lire() << endl;
			}
		}
	
	_getch(); // demande d'appui de touche
	return 0;
}