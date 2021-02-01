#ifndef GESTION_AFFICHAGE
#define GESTION_AFFICHAGE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-----------------------------------------PROTOTYPE DES FONCTIONS-------------------------------------------------//

void afficherCommandes(const char nameFile[], const char version[]);
void affichageCodeHexaAvecInterface(const int* nbOctetParLigne, char adresseFichierLu[], FILE* fichierLu);
void afficherInterfaceCodeHexa(const int* nbOctetParLigne, const char adresseFichier[]);
void afficherCodeHexa(FILE* fichierLu, const int* nbOctetParLigne);



void verifNbParametresSuffisant(const int* nbParam, const int nbParamMin, const char nomFichier[], const char version[]);
void allocationDynamiqueTest(char* tableau);




void ouvreFichierLectureEtTest(FILE** fichierLecture, const char adresseEntree[]);
void ouvreFichierEcritureEtTest(FILE** fichierEcriture, const char adresseEcriture[], const char nomFichiers[], const int* numFichier, const char extension[]);
void rechercherFichiersDansFichier(FILE* fichierLu, const char adresseEcritureFichiers[], const unsigned char chaineEnTete[], const int tailleChaineEnTete, const unsigned char chaineEnFin[], const int tailleChaineEnFin, const char nomFichiers[], const char extension[]);



int compareTableaux(const unsigned char tab[], const unsigned char tabLue[], const int tailleTableaux);
void pileDecalage(unsigned char tab[], unsigned char* nouvelleValeur, int tailleTab);


#endif
