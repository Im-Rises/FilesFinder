#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VER "1.0"

/*--------------------------------------------LES TYPES D'ERREUR D'ARRET DU PROGRAMME------------------------------
* Les différents types de code erreur :
* 0 - Fonctionnement normal du programme.
* 1 - Manque de paramètres en entrée du programme pour un fonctionnement minmimum (-h).
* 2 - Mode/option inconnue.
* 3 - Fichier impossible à lire ou impossible d'écrire les fichiers (pas de fichier à lire ou de destination).
* 4 - Fichier impossible à créer et/ou ecrire dedans.
* 5 - S'il y a un manque de mémoire alors le programme s'arrête avec cette erreur (allocation dynamique).
*/


/*-----------------------------------------------FONCTION MAIN-------------------------------------------------------------------*/

int main(int argc, char* argv[])
{
	//----------------------------------------------Initalisation------------------------------------------//

	//argc est le nombre de paramètre en entrée du programme
	//argv est un tableau de char qui à chaque case contient un des paramètres en entrée

	const int nbOctetParLigne = 16;	//Nombre d'octet affiché par ligne
	FILE* fichierLu = NULL;			//Création d'un objet de typer ficher 
	char* adresseFichier = NULL;	//Adresse du fichier à lire
	char* adresseDestination = NULL;//Adresse du répertoire ou écrire les fichiers trouvés



	//-------------------------------------En-tête et en-fin des types de fichier à extraire------------------------------------------//

	//Fichiers sans problème d'extraction :
	const unsigned char chainePngEnTete[] = { 0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A,'\0' };	// 89 50 4E 47 0D 0A 1A 0A
	const unsigned char chainePngEnFin[] = { 0x49,0x45,0x4E,0x44,0xAE,0x42,0x60,0x82, '\0' };	// 49 45 4E 44 AE 42 60 82

	//Fichiers avec problèmes d'extraction :
	const unsigned char chaineJpgEnTete[] = { 0xFF,0xD8,0xFF,'\0' };							// FF D8 FF
	const unsigned char chaineJpgEnFin[] = { 0xFF,0xD9, '\0' };									// FF D9

	const unsigned char chaineGifEnTete[] = { 0x47,0x49,0x46,0x38,'\0' };						// 47 49 46 38
	const unsigned char chaineGifEnFin[] = { 0x00,0x00,0x3B,'\0' };								// 00 00 3B



	//----------------------------Code pour le test du programme sans envoyer de paramètres en entrée-----------------------//
	////////TEST
	//ouvreFichierLectureEtTest(&fichierLu, "C:\\Users\\Quent\\Downloads\\Téléchargement\\poubelle\\WinRAR.exe");//Ouverture fichier
	//rechercherFichiersDansFichier(fichierLu, "C:\\Users\\Quent\\Downloads\\Téléchargement", chaineGifEnTete, sizeof(chaineGifEnTete) ,chaineGifEnFin,sizeof(chaineGifEnFin), "gifFile", ".gif");
	//rechercherFichiersDansFichier(fichierLu, "C:\\Users\\Quent\\Downloads\\Téléchargement", chainePngEnTete, sizeof(chainePngEnTete), chaineGifEnFin, sizeof(chaineGifEnFin), "gifFile", ".gif");
	//rechercherFichiersDansFichier(fichierLu, "C:\\Users\\Quent\\Downloads\\Téléchargement", chaineJpgEnTete, sizeof(chaineJpgEnTete), chaineGifEnFin, sizeof(chaineGifEnFin), "gifFile", ".gif");
	////////TEST



	//------------------------------------------------------Début de traitement--------------------------------------------------------------//

	verifNbParametresSuffisant(&argc, 2, argv[0],VER);//Verification du nombre de paramètre en entrée supérieur ou égal à 2

	//Le code suivant s'occupe de la gestion des options
	if (argv[1][0] == '-')
	{
		switch (argv[1][1])
		{
		case ('h'):
			//Affiche l'aide du programme et le mode d'emploi
			afficherCommandes(argv[0],VER);
			break;

		case ('d'):
			//Affiche le code hexadécimale du code analysé uniquement

			//Vérfie que le nombre de paramètre est supérieur ou égal à 3
			verifNbParametresSuffisant(&argc, 3, argv[0]);

			//Allocation d'un tableau dynamique qui contient l'adresse du fichier lu copié de argv[2]
			adresseFichier = malloc((strlen(argv[2]) + 1) * sizeof(char));
			allocationDynamiqueTest(adresseFichier);
			strcpy(adresseFichier, argv[2]);

			ouvreFichierLectureEtTest(&fichierLu, adresseFichier);//Ouverture fichier à lire
			affichageCodeHexaAvecInterface(&nbOctetParLigne, adresseFichier, fichierLu);//Affichage du code hexa du fichier lu
			
			//Fermerture du fichier lu (inspecté)
			fclose(fichierLu);

			//Libéraiton de la mémoire allouée
			free(adresseFichier);

			break;

		case ('e'):
			//Effectue une extraction du fichier donné

			//Vérfie que le nombre de paramètre est supérieur ou égal à 4
			verifNbParametresSuffisant(&argc, 4, argv[0]);

			//Allocation d'un tableau dynamique qui contient l'adresse du fichier lu copié de argv[2]
			adresseFichier = malloc((strlen(argv[2]) + 1) * sizeof(char));
			allocationDynamiqueTest(adresseFichier);
			strcpy(adresseFichier, argv[2]);

			//Allocation d'un tableau dynamique qui contient l'adresse de destination d'ou écrire les fichiers trouvé dans le code du fichier lu
			//La donnée est copié de argv[3] dans adressseDestination_e
			adresseDestination = malloc((strlen(argv[3]) + 1) * sizeof(char));
			allocationDynamiqueTest(adresseDestination);
			strcpy(adresseDestination, argv[3]);

			ouvreFichierLectureEtTest(&fichierLu, adresseFichier);//Ouverture fichier

			//Recherche des fichiers connus
			rechercherFichiersDansFichier(fichierLu, adresseDestination, chainePngEnTete, sizeof(chainePngEnTete), chainePngEnFin, sizeof(chainePngEnFin), "pngFile", ".png");
			rechercherFichiersDansFichier(fichierLu, adresseDestination, chaineJpgEnTete, sizeof(chaineJpgEnTete), chaineJpgEnFin, sizeof(chaineJpgEnFin), "jpgFile", ".jpg");
			rechercherFichiersDansFichier(fichierLu, adresseDestination, chaineGifEnTete, sizeof(chaineGifEnTete), chaineGifEnFin, sizeof(chaineGifEnFin), "gifFile", ".gif");

			//Fermerture du fichier lu (inspecté)
			fclose(fichierLu);

			//Libéraiton de la mémoire allouée
			free(adresseFichier);
			free(adresseDestination);

			break;

		default:
			//Cas où l'option donnée est inconnue
			printf("Error option %c unknown.\n", argv[1][1]);
			afficherCommandes(argv[0], VER);
			exit(2);
			break;
		}
	}
	else
	{
		//Dans le cas où l'option est mal écrite on affiche la liste des commandes et l'usage
		afficherCommandes(argv[0], VER);
	}
	return 0;
}
