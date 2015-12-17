/*! \mainpage Classe RS232
\author Super développeur
\version 1.0
\date    2015
\image html db9.jpg
\warning Fonctionne sous Windows
\bug Aucun bug détecté à ce jour
*/




#ifndef _RS232
#define _RS232

#include <stdio.h>
#include <string.h>  
#include <sys\types.h>
#include <time.h>
#include <windows.h>

//! \class CRS232
//!  \brief Gestion rapide du port série RS232
//!  \author Vincent ROBERT
//!  \version 1.0
//!  \date    2015
//!  \warning La réception peut être bloquante 
class CRS232
{
public:
          void getMessageErreur(char *szMessage);
          int lire(int NbOctets , BYTE *Buffer);
          BYTE lire(void);
          int ecrire(int NbOctets , BYTE *Buffer);
          void ecrire(BYTE octet);
          int getErreur();
          int presenceCar();
          void initialiser(DWORD Vitesse=CBR_9600,BYTE NbDonnees=8 , BYTE NbStops=ONESTOPBIT ,BYTE Parite=NOPARITY);

          CRS232(int NoPort=2);
          ~CRS232();
	 

protected:
	HANDLE hCom;
	int noErreur; // 0 => Aucune Erreur
	              // 1 => Pb ouverture du port
		      // 2 => Pb utilisation fonction GetCommstate
		      // 3 => Pb Utilisation SetCommState
		      // 4 => Pb d'écriture
		      // 5 => Pb avec fonction ClearCommError
		      // 6 => Pb de lecture
		      // 7 => Pb fermeture port
};


#endif
