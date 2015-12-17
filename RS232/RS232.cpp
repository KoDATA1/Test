// RS232.cpp: implementation of the CRS232 class.
//
//////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#include "RS232.h"
#include <stdexcept>

using namespace std;

	//********************************************************************
	//!  CONSTRUCTEUR
	//********************************************************************
	CRS232::CRS232(int NoPort)
	{
		char NomPort[50+1];

		noErreur=0;
		sprintf(NomPort,"\\\\.\\COM%d",NoPort);
		hCom=CreateFileA(NomPort , GENERIC_READ | GENERIC_WRITE , 0, NULL,
														OPEN_EXISTING , 0 ,NULL);

		if (hCom == INVALID_HANDLE_VALUE)
			noErreur=1;		// noErreur = 1 => Erreur d'ouverture de port
	}

	//********************************************************************
	//! DESTRUCTEUR
	//********************************************************************
	CRS232::~CRS232()
	{
		BOOL retour;

		noErreur=0;
		retour = CloseHandle(hCom);		// Fermeture du port s�rie
		if (!retour) noErreur=7;
	}

	//********************************************************************
	//! INITIALISATION DU PORT
	//! \param Vitesse Debit en bauds
	//! \param NbDonnees Nombre de bits de donn�es
	//! \param NbStop Nombre de bits de stop (utiliser les constantes ONESTOPBIT ou TWOSTOPBITS)
	//! \param Parite Parite paire (EVENPARITY) ou impaire (ODDPARITY) ou sans parite (NOPARITY)
	//********************************************************************
	void CRS232::initialiser(DWORD Vitesse,BYTE NbDonnees, BYTE NbStops, BYTE Parite)
	{
		BOOL retour;
		DCB dcb;

		noErreur=0;
		retour = GetCommState(hCom , &dcb);
		if (retour == 0) noErreur=2;
		else
		{
			dcb.ByteSize=NbDonnees;
			dcb.StopBits=NbStops;
			dcb.Parity=Parite;
			dcb.BaudRate=Vitesse;
			retour = SetCommState(hCom , &dcb);
			if (!retour) noErreur=3;
		}
		PurgeComm(hCom,PURGE_TXCLEAR & PURGE_RXCLEAR);
	}

	//********************************************************************
	//! ECRITURE D'UN CARACTERE SUR LE PORT
	//! \param octet L'octet � �crire sur le port s�rie
	//********************************************************************
	void CRS232::ecrire(BYTE octet)
	{
		BOOL retour;
		DWORD NbOctetsEcrits;

		noErreur=0;
		retour=WriteFile(hCom,&octet,1,&NbOctetsEcrits,NULL);
		if (! retour ) noErreur=4;
	}

	//********************************************************************
	//! ECRITURE DE NB CARACTERES STOCKES DANS UN BUFFER SUR LE PORT
	//! \param NbOctets Nombre d'octets � �crire
	//! \param Buffer Tableau contenant les octets � �crire
	//! \return : Nombre d'octets effectivement �crits
	//********************************************************************
	int CRS232::ecrire(int NbOctets, BYTE *Buffer)
	{
		BOOL retour;
		DWORD NbOctetsEcrits;

		noErreur=0;
		retour=WriteFile(hCom,Buffer,NbOctets,&NbOctetsEcrits,NULL);
 		if (! retour )
		{
			noErreur=4;
			return 0;
		}
		else return NbOctetsEcrits;
	}

	//********************************************************************
	//! TEST DE PRESENCE D'UN CARACTERE SUR LE PORT
	// Cette fonction renvoie le nb de caract�res en attente de lecture
	//! \return Nombre de caract�res en attente d'�tre lus 
	//********************************************************************
	int CRS232::presenceCar()
	{
		DWORD CodeErreur;
		COMSTAT EtatCom;
		BOOL retour;

		noErreur=0;
		retour=ClearCommError(hCom,&CodeErreur,&EtatCom);
		if ( ! retour)
		{
			noErreur=5;
			return 0;
		}

		else
			return EtatCom.cbInQue;

	}

	//********************************************************************
	//! Cette fonction lit un caract�re sur le port
	//! \return Caract�re lu
	//********************************************************************
	BYTE CRS232::lire()
	{
		BOOL retour;
		BYTE OctetLu;
		DWORD NbOctetsLus;

		noErreur=0;
		retour=ReadFile(hCom , &OctetLu ,1 , &NbOctetsLus , NULL);
		if (!retour)
		{
			noErreur = 6;
			return 0;
		}
		else return OctetLu;
	}

	//********************************************************************
	//! Cette fonction lit Nb caract�res sur le port et les stocke dans Buffer
	//! \param NbOctets Nombre d'octets � lire
	//! \param Buffer Tableau qui contiendra � la fin de cette fonction les octets lus
	//! \return : Nombre d'octets effectivement lus
	//********************************************************************
	int CRS232::lire(int NbOctets, BYTE *Buffer)
	{
		BOOL retour;
		DWORD NbOctetsLus;

		noErreur=0;
		retour=ReadFile(hCom , Buffer , NbOctets , &NbOctetsLus , NULL);
		if (!retour)
		{
			noErreur = 6;
			return 0;
		}
		else return NbOctetsLus;
	}

	//********************************************************************
	//! Cette fonction retourne le num�ro de l'erreur
	//! \return Numero d'erreur
	//********************************************************************
	int CRS232::getErreur()
	{
		return noErreur;
	}

	

	//********************************************************************
	//! Cette fonction retourne le message d'erreur
	//! \param szMessage Chaine de caract�res contenant � la fin de la m�thode le message d'erreur
	//********************************************************************
	void CRS232::getMessageErreur(char *szMessage)
	{
		switch(noErreur)
		{
		case 0 : strcpy(szMessage , "Aucune Erreur");break;
		case 1 : strcpy(szMessage , "Pb ouverture du port");break;
		case 2 : strcpy(szMessage ,"Pb utilisation fonction GetCommstate");break;
		case 3 : strcpy(szMessage , "Pb Utilisation SetCommState");break;
		case 4 : strcpy(szMessage ,"Pb d'�criture");break;
		case 5 : strcpy(szMessage ,"Pb avec fonction ClearCommError");break;
		case 6 : strcpy(szMessage ,"Pb de lecture");break;
		case 7 : strcpy(szMessage ,"Pb fermeture port");break;
		default : strcpy(szMessage ,""); break;
		}
			noErreur=0;     // R�initialisation des erreurs apr�s lecture
	}




