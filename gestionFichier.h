#ifndef GESTION_AFFICHAGE
#define GESTION_AFFICHAGE


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*---------------------------------------------------------FONCTION D'AFFICHAGE AIDE-----------------------------------------*/

void afficherCommandes(const char nameFile[], const char version[]);


/*-----------------------------------------------------FONCTION AFFICHAGE CODE FICHIER LU---------------------------------------------------------*/

void affichageCodeHexaAvecInterface(const int* nbOctetParLigne, char adresseFichierLu[], FILE* fichierLu);
void afficherInterfaceCodeHexa(const int* nbOctetParLigne, const char adresseFichier[]);
void afficherCodeHexa(FILE* fichierLu, const int* nbOctetParLigne);


/*------------------------------------------------------FONTIONS TEST BON FONCTIONNEMENT---------------------------------------*/

void verifNbParametresSuffisant(const int* nbParam, const int nbParamMin, const char nomFichier[], const char version[]);
void allocationDynamiqueTest(char* tableau);


/*-----------------------------------------------FONCTION DE TENTATIVE DE LECTURE/ECRITURE FICHIER-----------------------------------*/

void ouvreFichierLectureEtTest(FILE** fichierLecture, const char adresseEntree[]);
void ouvreFichierEcritureEtTest(FILE** fichierEcriture, const char adresseEcriture[], const char nomFichiers[], const int* numFichier, const char extension[]);


/*----------------------------------------------FONCTION DE RECHERCHE DE FICHIERS DANS UN FICHIER---------------------------------*/

void rechercherFichiersDansFichier(FILE* fichierLu, const char adresseEcritureFichiers[], const unsigned char chaineEnTete[], const int tailleChaineEnTete, const unsigned char chaineEnFin[], const int tailleChaineEnFin, const char nomFichiers[], const char extension[]);


/*-----------------------------------------------------------FONCTION AUXILIAIRES------------------------------------------------------*/

int compareTableaux(const unsigned char tab[], const unsigned char tabLue[], const int tailleTableaux);
void pileDecalage(unsigned char tab[], unsigned char* nouvelleValeur, int tailleTab);


#endif
